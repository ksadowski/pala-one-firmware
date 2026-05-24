#include "src/ui/mode_handlers.h"
#include "src/state.h"
#include "src/ui/ui.h"
#include "src/reader/reader.h"
#include "src/storage/library.h"
#include "src/storage/progress.h"
#include "src/storage/page_cache.h"
#include "src/storage/app_catalog.h"
#include "src/pure/paths.h"
#include "src/hal/input.h"
#include "src/apps/app_runner.h"

static void idlePrefetchReader() {
  static uint32_t lastIdlePrefetchMs = 0;
  if (mode != MODE_READER) return;
  if (g_bookmarkUi.previewActive) return;
  if (!g_reader.file) return;
  if (g_reader.eofReached) return;
  uint32_t now = millis();
  if ((uint32_t)(now - lastIdlePrefetchMs) < 60) return;
  lastIdlePrefetchMs = now;
  ensureOffsetsUpTo(g_reader.pageIndex + READER_IDLE_PREFETCH_PAGES);
}

void handleModeAbout() {
  if (btns.shortClick || btns.doubleClick || btns.longClick || btns.quadClick) {
    mode = MODE_LIBRARY;
    drawLibrary();
  }
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
  // NOTE: g_bookmarkUi.count/.pages/.offsets are loaded once by drawBookmarksList()
  // when entering this mode. No need to reload from Preferences every loop.
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
    // Triple-click = all the way home to library root
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  if (btns.longClick) {
    // Long-click = back to book select
    mode = MODE_BM_BOOK_SELECT;
    drawBookmarksBookSelect();
    return;
  }
}

void handleModeBookmarkPreview() {
  if (btns.tripleClick) {
    // Triple-click = back to bookmark list (where the user came from)
    g_bookmarkUi.previewActive = false;
    safeCloseCurrentBook();
    mode = MODE_BM_LIST;
    drawBookmarksList();
    return;
  }

  if (btns.longClick) {
    // Long-press = accept this bookmark position, continue reading from here
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

void handleModeReader() {
  if (btns.longClick) {
    const char* msg = addBookmarkForCurrentBook();
    if (msg) showToast(msg);
    g_reader.pageTurnsSinceFull++;
    renderCurrentPage();
    return;
  }

  if (btns.doubleClick) {
    if (g_reader.pageIndex > 0) {
      g_reader.pageIndex--;
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
      saveProgress(false);
    }
    return;
  }

  if (btns.shortClick) {
    int oldPage = g_reader.pageIndex;
    g_reader.pageIndex++;
    ensureOffsetsUpTo(g_reader.pageIndex);
    if (g_reader.eofReached && g_reader.pageIndex >= g_reader.knownPages)
      g_reader.pageIndex = g_reader.knownPages - 1;
    if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
    if (g_reader.pageIndex != oldPage) {
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
      saveProgress(false);
    }
    return;
  }

  idlePrefetchReader();
}
