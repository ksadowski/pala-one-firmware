#include "src/ui/screens/list_screen.h"
#include "src/ui/ui.h"
#include "src/state.h"
#include "src/storage/library.h"

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

static const int UI_LIST_LEFT = MARGIN_X + 4;

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

void handleModeList() {
  if (!listHasVisibleItems()) {
    mode = MODE_LIBRARY;
    drawLibrary();
    return;
  }

  if (btns.shortClick) {
    g_list.selectedIndex++;
    if (g_list.selectedIndex >= g_list.count) g_list.selectedIndex = 0;
    drawListScreen();
    return;
  }

  if (btns.longClick) {
    if (g_list.selectedIndex >= 0 && g_list.selectedIndex < g_list.count) {
      g_list.items[g_list.selectedIndex].done =
          g_list.items[g_list.selectedIndex].done ? 0 : 1;
      saveListItems();
      drawListScreen();
    }
    return;
  }

  if (btns.doubleClick || btns.tripleClick) {
    mode = MODE_LIBRARY;
    drawLibrary();
    return;
  }
}
