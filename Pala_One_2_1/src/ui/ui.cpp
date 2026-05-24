#include "src/ui/ui.h"
#include "src/config.h"
#include "src/state.h"
#include "src/hal/battery.h"

// UI layout constants (mirrors the static consts removed from .ino)
static const int UI_HEADER_TOP   = 6;
static const int UI_HEADER_GAP   = 6;
static const int UI_LIST_LEFT    = MARGIN_X + 4;
static const int UI_DEPTH_INDENT = 10;

// ============================================================================
//  Battery drawing
// ============================================================================
#if HAS_BATTERY
void drawBatteryTopRight() {
  updateBatteryCached(false);

  int pct = g_battery.valid ? g_battery.pctShown : 0;
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;

  const int iconW = 18;
  const int iconH = 9;
  int xIcon = SCREEN_W - MARGIN_X - iconW - 2;
  int yIcon = 2;

  gfx.drawRect(xIcon, yIcon, iconW, iconH, 1);
  gfx.fillRect(xIcon + iconW, yIcon + 2, 2, iconH - 4, 1);

  int innerW = iconW - 2;
  int fillW  = (innerW * pct) / 100;
  if (fillW > 0) gfx.fillRect(xIcon + 1, yIcon + 1, fillW, iconH - 2, 1);
  if (g_battery.low && pct > 0)
    gfx.drawLine(xIcon + 3, yIcon + 2, xIcon + 3, yIcon + iconH - 3, 0);

  u8g2.setFont(u8g2_font_6x10_tf);
  char buf[8];
  if (g_battery.valid) snprintf(buf, sizeof(buf), "%d%%", pct);
  else                 snprintf(buf, sizeof(buf), "--");
  int wTxt = u8g2.getUTF8Width(buf);
  u8g2.setCursor(xIcon - 4 - wTxt, yIcon + 8);
  u8g2.print(buf);
  u8g2.setFont(MAIN_FONT);
}
#endif

// ============================================================================
//  Drawing primitives
// ============================================================================
void beginPageCanvas(bool clearMem) {
  if (clearMem) display.clearMemory();
  display.landscape();
  u8g2.setFontMode(1);
  u8g2.setForegroundColor(1);
  u8g2.setBackgroundColor(0);
}

void prepareMenuFrame() {
  bool doFull = (menuDrawsSinceFull >= MENU_FULL_REFRESH_EVERY);
  if (doFull) {
    display.fastmodeOff();
    display.clear();
    menuDrawsSinceFull = 0;
  } else {
    display.fastmodeOn();
  }
  beginPageCanvas();
  menuDrawsSinceFull++;
}

void drawCenter(const char* a, const char* b) {
  display.fastmodeOff();
  display.clear();
  beginPageCanvas();
  u8g2.setFont(MAIN_FONT);

  const int lineH = 16;
  int y = (SCREEN_H / 2) - lineH / 2;
  if (b) y -= lineH / 2;

  int wA = u8g2.getUTF8Width(a);
  u8g2.setCursor((SCREEN_W - wA) / 2, y);
  u8g2.print(a);

  if (b) {
    y += lineH;
    int wB = u8g2.getUTF8Width(b);
    u8g2.setCursor((SCREEN_W - wB) / 2, y);
    u8g2.print(b);
  }
  display.update();
}

void showToast(const String& msg) {
  g_toast.msg     = msg;
  g_toast.untilMs = millis() + TOAST_MS;
}

void drawToastIfActive() {
  if (g_toast.untilMs == 0) return;
  if ((int32_t)(millis() - g_toast.untilMs) > 0) {
    g_toast.untilMs = 0;
    g_toast.msg     = "";
    return;
  }

  const int yTop = SCREEN_H - STATUS_H;
  gfx.fillRect(0, yTop, SCREEN_W, STATUS_H, 0);

  u8g2.setFont(u8g2_font_6x10_tf);
  int textY = SCREEN_H - 1;
  u8g2.setCursor(MARGIN_X, textY);
  u8g2.print(g_toast.msg.c_str());
  u8g2.setFont(MAIN_FONT);
}

// ============================================================================
//  Menu drawing
// ============================================================================
int drawSectionHeader(const char* title) {
  u8g2.setFont(BOLD_FONT);
  int ascent = u8g2.getFontAscent();
  int yTitle = UI_HEADER_TOP + ascent - 2;

  const char* headerText = "Pala One";
  if (title && strcmp(title, "Library") != 0) headerText = title;

  u8g2.setCursor(MARGIN_X, yTitle);
  u8g2.print(headerText);

#if HAS_BATTERY
  drawBatteryTopRight();
#endif

  int lineY = yTitle + 4;
  gfx.drawFastHLine(MARGIN_X, lineY, SCREEN_W - (MARGIN_X * 2), 1);

  int contentTop = lineY + UI_HEADER_GAP + 11;
  u8g2.setFont(MAIN_FONT);
  return contentTop;
}

void drawMenuBulletRow(int yBaseline, const String& label, bool selected,
                       bool boldText, int depth, bool systemItem) {
  int textX = UI_LIST_LEFT + (depth * UI_DEPTH_INDENT);
  if (systemItem) textX += 2;

  u8g2.setForegroundColor(1);
  u8g2.setFont(boldText ? BOLD_FONT : MAIN_FONT);
  u8g2.setCursor(textX, yBaseline);
  u8g2.print(label.c_str());
  u8g2.setFont(MAIN_FONT);
}

