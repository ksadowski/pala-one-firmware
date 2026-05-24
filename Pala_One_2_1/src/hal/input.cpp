#include "src/hal/input.h"
#include "src/state.h"

static inline uint32_t isrNowMs() {
  return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

void markUserActivity() {
  lastUserActionMs = millis();
}

void clearButtonQueue() {
  noInterrupts();
  btnQHead = 0;
  btnQTail = 0;
  interrupts();
}

void IRAM_ATTR btnISR() {
  uint8_t next = (uint8_t)((btnQHead + 1) % BTN_Q);
  if (next == btnQTail) {
    btnQTail = (uint8_t)((btnQTail + 1) % BTN_Q);
    g_isrDropCount++;
  }
  btnQState[btnQHead] = (digitalRead(BTN) == LOW);
  btnQTimeMs[btnQHead] = isrNowMs();
  btnQHead = next;
}

void ButtonState::poll() {
  resetClicks();

  uint8_t headSnap;
  noInterrupts();
  headSnap = btnQHead;
  interrupts();

  while (btnQTail != headSnap) {
    noInterrupts();
    bool rawPressed = btnQState[btnQTail];
    uint32_t edgeT = btnQTimeMs[btnQTail];
    btnQTail = (uint8_t)((btnQTail + 1) % BTN_Q);
    interrupts();

    if ((uint32_t)(edgeT - lastStableChange) <= DEBOUNCE_MS) continue;
    if (rawPressed == stablePressed) continue;

    bool prevPressed = stablePressed;
    stablePressed = rawPressed;
    lastStableChange = edgeT;

    if (!prevPressed && stablePressed) {
      pressStart = edgeT;
      pressArmed = true;
    }

    if (prevPressed && !stablePressed) {
      if (pressArmed) {
        uint32_t dur = (uint32_t)(edgeT - pressStart);
        if (dur >= LONG_MS) {
          clickCount = 0;
          longClick = true;
        } else {
          rawPressCount++;  // count every short press unconditionally
          clickCount++;
          lastRelease = edgeT;
          if (clickCount == 1) firstClickRelease = edgeT;
          if (clickCount >= 4) {
            clickCount = 0;
            quadClick = true;
          }
        }
      }
      pressArmed = false;
      pressStart = 0;
    }
  }

  if (clickCount > 0) {
    uint32_t now = millis();
    bool emit = false;
    if (clickCount <= 2) emit = (uint32_t)(now - lastRelease) > DOUBLE_MS;
    else if (clickCount == 3) emit = (uint32_t)(now - firstClickRelease) > TRIPLE_MS;

    if (emit) {
      if (clickCount == 1) shortClick  = true;
      else if (clickCount == 2) doubleClick = true;
      else if (clickCount == 3) tripleClick = true;
      clickCount = 0;
    }
  }
}

void resetInputFrontend() {
  // Wait for the button that triggered this transition (wake or triple-click)
  // to be physically released, then debounce. This prevents that single press
  // from leaking into the new mode as an accidental action.
  // We do NOT clear the whole ISR queue — any presses that arrive AFTER
  // release are intentional and should be processed normally.
  uint32_t deadline = millis() + 600; // safety timeout
  while (digitalRead(BTN) == LOW && (uint32_t)(millis()) < deadline) delay(1);
  delay(DEBOUNCE_MS + 2); // minimal debounce after release

  // Discard only events that happened BEFORE this moment (the transition press).
  // Events queued after the release are kept.
  noInterrupts();
  uint8_t headNow = btnQHead;
  interrupts();
  btnQTail = headNow; // advance tail to head = discard old events only
  btns.resetState();
  markUserActivity();
}
