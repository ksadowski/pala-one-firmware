#include "src/ui/screens/about_screen.h"
#include "src/ui/ui.h"
#include "src/state.h"
#include "src/config.h"

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

void handleModeAbout() {
  if (btns.shortClick || btns.doubleClick || btns.longClick || btns.quadClick) {
    mode = MODE_LIBRARY;
    drawLibrary();
  }
}
