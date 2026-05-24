#include "src/ui/screens/bookmarks_screen.h"
#include "src/ui/ui.h"
#include "src/state.h"
#include "src/storage/library.h"
#include "src/storage/progress.h"
#include "src/storage/page_cache.h"
#include "src/pure/paths.h"
#include "src/pure/text_util.h"
#include "src/reader/reader.h"
#include "src/hal/input.h"
#include "src/webui/web_helpers.h"

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

void handleModeBookmarkBookSelect() {
  if (btns.tripleClick) {
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  if (g_library.bookCount == 0) {
    if (btns.anyClick()) {
      mode = MODE_LIBRARY;
      drawLibrary();
    }
    return;
  }

  if (btns.shortClick) {
    g_bookmarkUi.bookIndex++;
    if (g_bookmarkUi.bookIndex >= g_library.bookCount) g_bookmarkUi.bookIndex = 0;
    drawBookmarksBookSelect();
    return;
  }

  if (btns.longClick) {
    g_bookmarkUi.bookIndex--;
    if (g_bookmarkUi.bookIndex < 0) g_bookmarkUi.bookIndex = g_library.bookCount - 1;
    drawBookmarksBookSelect();
    return;
  }

  if (btns.doubleClick) {
    g_bookmarkUi.selectedIndex = 0;
    mode = MODE_BM_LIST;
    drawBookmarksList();
  }
}

void handleModeBookmarkList() {
  if (!btns.anyClick()) return;

  if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count)
    g_bookmarkUi.selectedIndex = max(0, (int)g_bookmarkUi.count - 1);

  if (btns.shortClick) {
    if (g_bookmarkUi.count > 0) {
      g_bookmarkUi.selectedIndex++;
      if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count) g_bookmarkUi.selectedIndex = 0;
    }
    drawBookmarksList();
    return;
  }

  if (btns.doubleClick) {
    if (g_bookmarkUi.count == 0) return;

    String previewPath = String(g_library.books[g_bookmarkUi.bookIndex].path);
    if (g_reader.currentBookPath == previewPath && g_reader.currentBookKey.length() > 0) {
      g_bookmarkUi.previewSavedPage = g_reader.pageIndex;
    } else {
      String previewKey = prefKeyForBook(previewPath);
      g_bookmarkUi.previewSavedPage = prefs.getInt((previewKey + "_p").c_str(), 0);
    }

    if (openBookByIndex(g_bookmarkUi.bookIndex)) {
      g_bookmarkUi.previewActive = true;
      g_reader.pageIndex = (int)g_bookmarkUi.pages[g_bookmarkUi.selectedIndex];
      if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
      mode = MODE_BM_PREVIEW;
      renderCurrentPage();
    } else {
      mode = MODE_LIBRARY;
      drawLibrary();
    }
    return;
  }

  if (btns.tripleClick) {
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  if (btns.longClick) {
    mode = MODE_BM_BOOK_SELECT;
    drawBookmarksBookSelect();
    return;
  }
}

void handleModeBookmarkPreview() {
  if (btns.tripleClick) {
    g_bookmarkUi.previewActive = false;
    safeCloseCurrentBook();
    mode = MODE_BM_LIST;
    drawBookmarksList();
    return;
  }

  if (btns.longClick) {
    g_bookmarkUi.previewActive = false;
    saveProgress(true);
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
    mode = MODE_READER;
    renderCurrentPage();
    return;
  }

  if (btns.doubleClick) {
    if (g_reader.pageIndex > 0) {
      g_reader.pageIndex--;
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
    }
    return;
  }

  if (btns.shortClick) {
    int oldPage = g_reader.pageIndex;
    g_reader.pageIndex++;
    ensureOffsetsUpTo(g_reader.pageIndex);
    if (g_reader.eofReached && g_reader.pageIndex >= g_reader.knownPages)
      g_reader.pageIndex = g_reader.knownPages - 1;
    if (g_reader.pageIndex != oldPage) {
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
    }
    return;
  }
}
