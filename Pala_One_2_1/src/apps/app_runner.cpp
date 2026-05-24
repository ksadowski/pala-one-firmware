#include "src/apps/app_runner.h"
#include "src/state.h"
#include "src/config.h"
#include "src/ui/ui.h"
#include "src/hal/input.h"
#include "pala_app.h"
#include "pala_api.h"
#include <stdarg.h>

// ============================================================================
//  PalaAPI wrapper implementations
// ============================================================================
static void api_clearScreen() {
  prepareMenuFrame();
}

static void api_drawHeader(const char* title) {
  drawSectionHeader(title);
}

static void api_drawTextAt(int x, int y, const char* text, int bold) {
  u8g2.setFont(bold ? BOLD_FONT : MAIN_FONT);
  u8g2.setCursor(x, y);
  u8g2.print(text);
}

static void api_drawCenteredLarge(const char* text) {
  u8g2.setFont(u8g2_font_helvB14_te);
  int w   = u8g2.getUTF8Width(text);
  int asc = u8g2.getFontAscent();
  // centre horizontally; vertically centred in the space below y=20 (approx header height)
  u8g2.setCursor((SCREEN_W - w) / 2, (SCREEN_H + 20 + asc) / 2);
  u8g2.print(text);
  u8g2.setFont(MAIN_FONT);
}

static void api_refreshDisplay() {
  display.update();
}

static uint8_t api_waitForEvent() {
  markUserActivity();
  while (true) {
    btns.poll();
    // Long press fires while the button is still held (not on release).
    if (btns.pressArmed && btns.stablePressed) {
      uint32_t now = (uint32_t)(esp_timer_get_time() / 1000ULL);
      if ((uint32_t)(now - btns.pressStart) >= LONG_MS) {
        btns.pressArmed = false;
        btns.pressStart = 0;
        markUserActivity();
        return PALA_LONG;
      }
    }
    if (btns.tripleClick) { btns.resetClicks(); markUserActivity(); return PALA_TRIPLE; }
    if (btns.doubleClick) { btns.resetClicks(); markUserActivity(); return PALA_DOUBLE; }
    if (btns.shortClick)  { btns.resetClicks(); markUserActivity(); return PALA_CLICK; }
    delay(1);
  }
}

static uint8_t api_pollEvent() {
  btns.poll();
  if (btns.longClick)   { btns.resetClicks(); markUserActivity(); return PALA_LONG; }
  if (btns.tripleClick) { btns.resetClicks(); markUserActivity(); return PALA_TRIPLE; }
  if (btns.doubleClick) { btns.resetClicks(); markUserActivity(); return PALA_DOUBLE; }
  if (btns.shortClick)  { btns.resetClicks(); markUserActivity(); return PALA_CLICK; }
  return 0;
}

static uint32_t api_millisNow() {
  return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static int api_buttonPressed() {
  return btns.stablePressed ? 1 : 0;
}

static void api_delayMs(uint32_t ms) {
  delay(ms);
}

static uint32_t api_pendingPresses() {
  btns.poll();
  uint32_t n = btns.rawPressCount;
  btns.rawPressCount = 0;
  return n;
}

static uint32_t api_rtcSeconds() {
  return (uint32_t)(esp_timer_get_time() / 1000000ULL);
}

static int api_storageRead(const char* key, void* buf, int maxlen) {
  char path[64];
  snprintf(path, sizeof(path), "/apps/%s.dat", key);
  File f = LittleFS.open(path, "r");
  if (!f) return -1;
  int n = f.read((uint8_t*)buf, maxlen);
  f.close();
  return n;
}

static int api_storageWrite(const char* key, const void* buf, int len) {
  char path[64];
  snprintf(path, sizeof(path), "/apps/%s.dat", key);
  File f = LittleFS.open(path, "w");
  if (!f) return -1;
  int n = f.write((const uint8_t*)buf, len);
  f.close();
  return n;
}

static int api_snprintf_wrap(char* buf, int len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r = vsnprintf(buf, (size_t)len, fmt, args);
  va_end(args);
  return r;
}

void initPalaAPI() {
  g_palaAPI.clearScreen       = api_clearScreen;
  g_palaAPI.drawHeader        = api_drawHeader;
  g_palaAPI.drawTextAt        = api_drawTextAt;
  g_palaAPI.drawCenteredLarge = api_drawCenteredLarge;
  g_palaAPI.refreshDisplay    = api_refreshDisplay;
  g_palaAPI.waitForEvent      = api_waitForEvent;
  g_palaAPI.snprintf_wrap     = api_snprintf_wrap;
  g_palaAPI.pollEvent         = api_pollEvent;
  g_palaAPI.millisNow         = api_millisNow;
  g_palaAPI.buttonPressed     = api_buttonPressed;
  g_palaAPI.delayMs           = api_delayMs;
  g_palaAPI.pendingPresses    = api_pendingPresses;
  g_palaAPI.storageRead       = api_storageRead;
  g_palaAPI.storageWrite      = api_storageWrite;
  g_palaAPI.rtcSeconds        = api_rtcSeconds;
}

// ============================================================================
//  App loader
// ============================================================================
void freeAppExecBuf() {
  if (g_appExecBuf) {
    heap_caps_free(g_appExecBuf);
    g_appExecBuf  = nullptr;
    g_appExecSize = 0;
  }
}

bool loadAndRunApp(const char* path) {
  freeAppExecBuf();

  File f = FS.open(path, "r");
  if (!f) { drawCenter("App not found", path); delay(1500); return false; }

  size_t fileSize = f.size();
  if (fileSize < sizeof(PalaAppHeader) + 4) {
    f.close(); drawCenter("App too small", "Invalid file"); delay(1500); return false;
  }

  const size_t MAX_APP_BINARY = 48 * 1024;
  if (fileSize > MAX_APP_BINARY) {
    f.close(); drawCenter("App too large", "> 48 KB"); delay(1500); return false;
  }

  g_appExecBuf = heap_caps_malloc(fileSize, MALLOC_CAP_EXEC | MALLOC_CAP_32BIT);
  if (!g_appExecBuf) {
    f.close();
    char msg[32];
    snprintf(msg, sizeof(msg), "Need %u bytes", (unsigned)fileSize);
    drawCenter("No exec memory", msg);
    delay(1500);
    return false;
  }
  g_appExecSize = fileSize;

  // heap_caps_malloc(MALLOC_CAP_EXEC) returns an IRAM instruction-bus address (0x403xxxxx).
  // On ESP32-S3 that address is not writable from the data bus; use the DIRAM DRAM view
  // (same physical SRAM, shifted by SOC_I_D_OFFSET) for all reads and writes.
  uint8_t* dataBuf = (uint8_t*)MAP_IRAM_TO_DRAM((uint32_t)g_appExecBuf);

  size_t bytesRead = f.read(dataBuf, fileSize);
  f.close();
  if (bytesRead != fileSize) {
    freeAppExecBuf(); drawCenter("Read error", "Partial read"); delay(1500); return false;
  }

  PalaAppHeader* hdr = (PalaAppHeader*)dataBuf;

  if (hdr->magic != PALA_APP_MAGIC) {
    freeAppExecBuf(); drawCenter("Bad app file", "Wrong magic"); delay(1500); return false;
  }
  if (hdr->api_version != PALA_API_VERSION) {
    char msg[32];
    snprintf(msg, sizeof(msg), "API v%u, need v%u",
             (unsigned)hdr->api_version, (unsigned)PALA_API_VERSION);
    freeAppExecBuf(); drawCenter("API mismatch", msg); delay(1500); return false;
  }

  if (hdr->entry_offset < sizeof(PalaAppHeader) || hdr->entry_offset >= fileSize) {
    freeAppExecBuf(); drawCenter("Bad entry offset", nullptr); delay(1500); return false;
  }

  // l32r uses the instruction bus (can reach IRAM), but firmware data loads use the data
  // bus (cannot reach IRAM). Relocations must therefore resolve to DRAM addresses so that
  // string/data pointers passed to firmware API functions are data-bus accessible.
  uint32_t base = (uint32_t)dataBuf;
  if (hdr->reloc_count > 0) {
    if (hdr->reloc_offset < sizeof(PalaAppHeader) ||
        hdr->reloc_offset + hdr->reloc_count * 4u > fileSize) {
      freeAppExecBuf(); drawCenter("Bad reloc table", nullptr); delay(1500); return false;
    }
    uint32_t* relocs = (uint32_t*)(dataBuf + hdr->reloc_offset);
    for (uint32_t i = 0; i < hdr->reloc_count; i++) {
      uint32_t off = relocs[i];
      if (off + 4u > hdr->reloc_offset) {
        freeAppExecBuf(); drawCenter("Reloc out of range", nullptr); delay(1500); return false;
      }
      *(uint32_t*)(dataBuf + off) += base;
    }
  }

  pala_app_entry_t entry = (pala_app_entry_t)((uint8_t*)g_appExecBuf + hdr->entry_offset);

  resetInputFrontend();
  entry(&g_palaAPI);

  freeAppExecBuf();
  resetInputFrontend();
  return true;
}
