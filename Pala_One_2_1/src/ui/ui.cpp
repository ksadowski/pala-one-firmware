#include "src/ui/ui.h"
#include "src/config.h"
#include "src/state.h"
#include "src/hal/battery.h"
#include "src/storage/library.h"
#include "src/storage/progress.h"
#include "src/pure/paths.h"
#include "src/pure/text_util.h"
#include "pala_one_sleep_black_icon_v4.h"

#include "src/webui/web_helpers.h"

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

static void splitListLabelForDisplay(const String& in, int maxWidth, String& line1, String& line2) {
  line1 = in;
  line2 = "";
  if (u8g2.getUTF8Width(in.c_str()) <= maxWidth) return;

  int bestBreak = -1;
  for (int i = 0; i < (int)in.length(); i++) {
    if (in[i] != ' ') continue;
    String left = in.substring(0, i);
    left.trim();
    if (left.length() == 0) continue;
    if (u8g2.getUTF8Width(left.c_str()) <= maxWidth) bestBreak = i;
    else break;
  }

  if (bestBreak < 0) {
    for (int i = 1; i < (int)in.length(); i++) {
      String left = in.substring(0, i);
      if (u8g2.getUTF8Width(left.c_str()) > maxWidth) {
        bestBreak = max(1, i - 1);
        break;
      }
    }
  }

  if (bestBreak < 0) return;

  line1 = in.substring(0, bestBreak);
  line1.trim();
  line2 = in.substring(bestBreak);
  line2.trim();

  while (line2.length() > 0 && u8g2.getUTF8Width(line2.c_str()) > maxWidth) {
    line2.remove(line2.length() - 1);
  }
}

void drawLibrary() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  buildLibraryEntries();

  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Library");

  int totalItems = g_library.entryCount;
  int visible    = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 3) visible = 3;
  if (visible > 6) visible = 6;

  int top = g_library.selectedItem - (visible / 2);
  if (top < 0) top = 0;
  if (top > totalItems - visible) top = max(0, totalItems - visible);

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= totalItems) break;

    String label  = libraryEntryLabel(idx);
    bool isSystem = (g_library.entryTypes[idx] == LIB_ENTRY_BOOKMARKS ||
                     g_library.entryTypes[idx] == LIB_ENTRY_LIST      ||
                     g_library.entryTypes[idx] == LIB_ENTRY_ABOUT     ||
                     g_library.entryTypes[idx] == LIB_ENTRY_APPS);
    bool boldText = (idx == g_library.selectedItem);
    drawMenuBulletRow(y, label, idx == g_library.selectedItem,
                      boldText, g_library.entryDepths[idx], isSystem);
    y += lineH;
  }
  display.update();
}

void drawListScreen() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("List");

  if (!listHasVisibleItems()) {
    drawMenuBulletRow(y, "No items", true, false, 0, false);
    display.update();
    return;
  }

  if (g_list.selectedIndex < 0) g_list.selectedIndex = 0;
  if (g_list.selectedIndex >= g_list.count) g_list.selectedIndex = g_list.count - 1;

  int visibleRows = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visibleRows < 3) visibleRows = 3;

  int top = g_list.selectedIndex - 2;
  if (top < 0) top = 0;
  if (top > g_list.count - 1) top = max(0, g_list.count - 1);

  int rowsUsed = 0;
  for (int idx = top; idx < g_list.count; idx++) {
    if (rowsUsed >= visibleRows) break;

    String label    = String(g_list.items[idx].text);
    bool   selected = (idx == g_list.selectedIndex);
    String line1, line2;
    int maxWidth = SCREEN_W - UI_LIST_LEFT - MARGIN_X;

    if (selected) splitListLabelForDisplay(label, maxWidth, line1, line2);
    else {
      line1 = label;
      line2 = "";
      while (line1.length() > 0 && u8g2.getUTF8Width(line1.c_str()) > maxWidth)
        line1.remove(line1.length() - 1);
    }

    drawMenuBulletRow(y, line1, selected, selected, 0, false);
    if (g_list.items[idx].done) {
      int w1      = u8g2.getUTF8Width(line1.c_str());
      int strikeY = y - ((ascent - descent) / 3);
      gfx.drawFastHLine(UI_LIST_LEFT, strikeY, w1, 1);
    }
    y += lineH;
    rowsUsed++;

    if (selected && line2.length() > 0 && rowsUsed < visibleRows) {
      u8g2.setFont(BOLD_FONT);
      u8g2.setCursor(UI_LIST_LEFT, y);
      u8g2.print(line2.c_str());
      if (g_list.items[idx].done) {
        int w2      = u8g2.getUTF8Width(line2.c_str());
        int strikeY = y - ((ascent - descent) / 3);
        gfx.drawFastHLine(UI_LIST_LEFT, strikeY, w2, 1);
      }
      y += lineH;
      rowsUsed++;
      u8g2.setFont(MAIN_FONT);
    }
  }
  display.update();
}

void drawAbout() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent = u8g2.getFontAscent();
  int lineH  = (ascent - u8g2.getFontDescent()) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Device");

  String rows[5] = {
    "Firmware " FW_VERSION,
    "1x next / down",
    "2x open / select",
    "3x home",
    "Hold bookmark"
  };

  for (int i = 0; i < 5; i++) {
    u8g2.setFont(i == 0 ? BOLD_FONT : MAIN_FONT);
    u8g2.setCursor(MARGIN_X, y);
    u8g2.print(rows[i].c_str());
    y += lineH;
  }
  display.update();
}

void drawAppsMenu() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Apps");

  if (g_apps.count == 0) {
    drawMenuBulletRow(y, "No apps installed", true, false, 0, false);
    display.update();
    return;
  }

  int visible = max(2, (SCREEN_H - y - BOT_PAD) / lineH);
  int top     = g_apps.selectedIndex - (visible / 2);
  top = max(0, min(top, g_apps.count - visible));

  for (int i = 0; i < visible; i++) {
    int  idx = top + i;
    if (idx >= g_apps.count) break;
    bool sel = (idx == g_apps.selectedIndex);
    drawMenuBulletRow(y, String(g_apps.apps[idx].name), sel, sel, 0, false);
    y += lineH;
  }
  display.update();
}

void drawBookmarksBookSelect() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Bookmarks");

  if (g_library.bookCount == 0) {
    drawMenuBulletRow(y, "No books", true, false, 0, false);
    display.update();
    return;
  }

  if (g_bookmarkUi.bookIndex < 0) g_bookmarkUi.bookIndex = 0;
  if (g_bookmarkUi.bookIndex >= g_library.bookCount)
    g_bookmarkUi.bookIndex = g_library.bookCount - 1;

  int visible = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 2) visible = 2;
  if (visible > 6) visible = 6;

  int top = g_bookmarkUi.bookIndex - (visible / 2);
  if (top < 0) top = 0;
  if (top > g_library.bookCount - visible) top = max(0, g_library.bookCount - visible);

  for (int i = 0; i < visible; i++) {
    int  idx = top + i;
    if (idx >= g_library.bookCount) break;
    bool sel = (idx == g_bookmarkUi.bookIndex);
    drawMenuBulletRow(y, String(g_library.books[idx].name), sel, sel, 0, false);
    y += lineH;
  }
  display.update();
}

void drawBookmarksList() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap;
  int y = drawSectionHeader("Bookmarks");

  String bookPath = String(g_library.books[g_bookmarkUi.bookIndex].path);
  String key      = prefKeyForBook(bookPath);
  g_bookmarkUi.count = loadBookmarksForKey(key, g_bookmarkUi.pages, g_bookmarkUi.offsets);
  if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count)
    g_bookmarkUi.selectedIndex = max(0, (int)g_bookmarkUi.count - 1);

  if (g_bookmarkUi.count == 0) {
    drawMenuBulletRow(y, "No bookmarks", true, false, 0, false);
    display.update();
    return;
  }

  File f = FS.open(bookPath, "r");
  if (!f) {
    drawMenuBulletRow(y, "Open failed", true, false, 0, false);
    display.update();
    return;
  }

  int visible = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 1) visible = 1;
  if (visible > 5) visible = 5;

  int top = g_bookmarkUi.selectedIndex - (visible / 2);
  if (top < 0) top = 0;
  if (top > (int)g_bookmarkUi.count - visible) top = max(0, (int)g_bookmarkUi.count - visible);

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= (int)g_bookmarkUi.count) break;

    int      targetPage = (int)g_bookmarkUi.pages[idx];
    if (targetPage < 0) targetPage = 0;
    uint32_t pageOff = resolveBookmarkOffset(bookPath, (uint16_t)targetPage, g_bookmarkUi.offsets[idx]);
    String   sn      = readBookmarkLabelAtOffset(f, pageOff, targetPage);
    bool     sel     = (idx == g_bookmarkUi.selectedIndex);
    drawMenuBulletRow(y, sn, sel, sel, 0, false);
    y += lineH;
  }

  f.close();
  display.update();
}

// ============================================================================
//  Sleep screen
// ============================================================================
void drawSleepScreen() {
  display.fastmodeOff();
  display.clear();
  beginPageCanvas();

  File sf = FS.open("/sleep.bin", "r");
  if (sf && sf.size() >= 3904) {
    static uint8_t sleepBuf[3904];
    sf.read(sleepBuf, 3904);
    sf.close();
    gfx.fillScreen(1);
    gfx.drawXBitmap(0, 0, sleepBuf, SCREEN_W, SCREEN_H, 0);
  } else {
    if (sf) sf.close();
    gfx.fillScreen(1);
    gfx.drawXBitmap(0, 0, pala_one_sleep_black_icon_v4_bits, SCREEN_W, SCREEN_H, 0);
  }
  display.update();
}
