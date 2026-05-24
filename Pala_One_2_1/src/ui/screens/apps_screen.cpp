#include "src/ui/screens/apps_screen.h"
#include "src/ui/ui.h"
#include "src/state.h"
#include "src/storage/app_catalog.h"
#include "src/apps/app_runner.h"

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

void handleModeApps() {
  if (btns.shortClick) {
    if (g_apps.count > 0)
      g_apps.selectedIndex = (g_apps.selectedIndex + 1) % g_apps.count;
    drawAppsMenu();
    return;
  }
  if (btns.doubleClick) {
    if (g_apps.count > 0 && g_apps.selectedIndex < g_apps.count) {
      loadAndRunApp(g_apps.apps[g_apps.selectedIndex].path);
      drawAppsMenu();
    }
    return;
  }
  // Triple-click handled globally → library root
}
