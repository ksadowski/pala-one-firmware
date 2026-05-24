#include "src/ui/screens/library_screen.h"
#include "src/ui/ui.h"
#include "src/state.h"
#include "src/storage/library.h"
#include "src/storage/progress.h"
#include "src/storage/app_catalog.h"
#include "src/reader/reader.h"
#include "src/ui/screens/about_screen.h"
#include "src/ui/screens/apps_screen.h"
#include "src/ui/screens/list_screen.h"
#include "src/ui/screens/bookmarks_screen.h"

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

void handleModeLibrary() {
  if (!btns.anyClick()) return;

  int totalItems = g_library.entryCount;

  if (btns.shortClick) {
    g_library.selectedItem++;
    if (g_library.selectedItem >= totalItems) g_library.selectedItem = 0;
    drawLibrary();
    return;
  }

  if (!btns.doubleClick) return;
  if (g_library.selectedItem < 0 || g_library.selectedItem >= g_library.entryCount) {
    drawLibrary();
    return;
  }

  LibraryEntryType entryType = g_library.entryTypes[g_library.selectedItem];
  int entryRef = g_library.entryRefs[g_library.selectedItem];

  if (entryType == LIB_ENTRY_FOLDER) {
    bool expanded = isFolderExpanded(entryRef);
    setFolderExpanded(entryRef, !expanded);
    drawLibrary();
    return;
  }

  if (entryType == LIB_ENTRY_BOOK) {
    if (openBookByIndex(entryRef)) {
      resetPreviewState();
      mode = MODE_READER;
      renderCurrentPage();
    } else {
      drawCenter("Open failed", "Try upload again");
      drawLibrary();
    }
    return;
  }

  if (entryType == LIB_ENTRY_BOOKMARKS) {
    g_bookmarkUi.bookIndex = 0;
    mode = MODE_BM_BOOK_SELECT;
    drawBookmarksBookSelect();
    return;
  }

  if (entryType == LIB_ENTRY_LIST) {
    g_list.selectedIndex = 0;
    mode = MODE_LIST;
    drawListScreen();
    return;
  }

  if (entryType == LIB_ENTRY_ABOUT) {
    mode = MODE_ABOUT;
    drawAbout();
    return;
  }

  if (entryType == LIB_ENTRY_APPS) {
    g_apps.selectedIndex = 0;
    scanApps();
    mode = MODE_APPS;
    drawAppsMenu();
    return;
  }
}
