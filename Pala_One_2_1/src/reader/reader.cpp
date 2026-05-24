#include "src/reader/reader.h"
#include "src/config.h"
#include "src/state.h"
#include "src/ui/ui.h"
#include "src/settings/settings.h"
#include "src/pure/text_util.h"
#include "src/pure/paths.h"
#include "src/pure/page_offset_table.h"
#include "src/pure/paginator.h"
#include "src/storage/page_cache.h"
#include "src/storage/progress.h"


// ============================================================================
//  Book / FS helpers
// ============================================================================
void safeCloseCurrentBook() {
  if (g_reader.file) g_reader.file.close();
}

void clearCurrentBookState() {
  safeCloseCurrentBook();
  g_reader.currentBookKey        = "";
  g_reader.currentBookPath       = "";
  g_reader.pageIndex             = 0;
  g_reader.knownPages            = 0;
  g_reader.eofReached            = false;
  g_reader.lastPageStartOffset   = 0;
  g_reader.pageTurnsSinceFull    = 0;
  g_reader.lastSaveMs            = 0;
  g_reader.lastSavedPage         = -1;
}

void resetPreviewState() {
  g_bookmarkUi.previewActive    = false;
  g_bookmarkUi.previewSavedPage = 0;
}

void resetUiEphemeralState() {
  g_toast.msg     = "";
  g_toast.untilMs = 0;
  resetPreviewState();
}

void resetNavigationState() {
  g_library.currentFolder      = "";
  g_library.selectedItem       = 0;
  g_bookmarkUi.bookIndex       = 0;
  g_bookmarkUi.selectedIndex   = 0;
}

bool reopenCurrentBookIfNeeded() {
  if (g_reader.currentBookPath.length() == 0) return false;
  safeCloseCurrentBook();
  g_reader.file = FS.open(g_reader.currentBookPath, "r");
  return (bool)g_reader.file;
}

void syncWakeState(bool reading) {
  prefs.putInt("wake_mode", reading ? 1 : 0);
  if (reading && g_reader.currentBookPath.length() > 0)
    prefs.putString("wake_path", g_reader.currentBookPath);
  else
    prefs.remove("wake_path");
}

void enterLibraryRoot(bool redraw) {
  safeCloseCurrentBook();
  resetPreviewState();
  resetNavigationState();
  syncWakeState(false);
  mode = MODE_LIBRARY;
  if (redraw) drawLibrary();
}

// ============================================================================
//  Pagination / text layout
// ============================================================================
static int measureWrapper(const char* str) {
  return u8g2.getUTF8Width(str);
}

struct LineCallbackData {
  bool draw;
  String* outText;
  int cursorY;
};

static void lineCallbackWrapper(const char* buf, size_t len, void* userData) {
  LineCallbackData* data = (LineCallbackData*)userData;
  if (data->draw) {
    u8g2.setCursor(MARGIN_X, data->cursorY);
    u8g2.print(buf);
    data->cursorY += getMetrics().lineH;
  }
  if (data->outText) {
    (*data->outText) += String(buf);
    (*data->outText) += "\n";
  }
}

uint32_t readPageFromFile(File& f, uint32_t startPos, bool draw, String* outText) {
  u8g2.setFont(MAIN_FONT);
  const LayoutMetrics& m = getMetrics();

  LineCallbackData data;
  data.draw = draw;
  data.outText = outText;
  data.cursorY = TOP_PAD + m.ascent;

  LineCallback callback = (draw || outText) ? lineCallbackWrapper : nullptr;

  return paginatePage(f, startPos, m, measureWrapper, callback, &data);
}

uint32_t buildNextOffsetFor(File& f, uint32_t startPos) {
  return readPageFromFile(f, startPos, false, nullptr);
}

uint32_t buildNextOffset(uint32_t startPos) {
  uint32_t next = readPageFromFile(g_reader.file, startPos, false, nullptr);
  if (next >= (uint32_t)g_reader.file.size()) g_reader.eofReached = true;
  return next;
}

uint32_t pageOffsetForPage(File& f, const String& path, int page) {
  if (page < 0) page = 0;

  int      cachedPage   = 0;
  uint32_t cachedOffset = 0;
  if (!lookupOffsetCache(path, page, cachedPage, cachedOffset)) {
    cachedPage   = 0;
    cachedOffset = 0;
  }

  uint32_t off = cachedOffset;
  for (int p = cachedPage; p < page; p++) {
    uint32_t next = buildNextOffsetFor(f, off);
    if (next == off) break;
    off = next;
    storeOffsetCache(path, p + 1, off);
  }

  storeOffsetCache(path, page, off);
  return off;
}

void ensureOffsetsUpTo(int targetPage) {
  if (g_reader.knownPages < 1) {
    g_reader.knownPages     = 1;
    g_reader.pageOffsets[0] = 0;
  }

  bool addedOffsets = false;
  while (!g_reader.eofReached && g_reader.knownPages <= targetPage && g_reader.knownPages < MAX_PAGES) {
    uint32_t start = g_reader.pageOffsets[g_reader.knownPages - 1];
    uint32_t next  = buildNextOffset(start);
    if (next <= start) {
      g_reader.eofReached = true;
      break;
    }
    g_reader.pageOffsets[g_reader.knownPages] = next;
    storeOffsetCache(g_reader.currentBookPath, g_reader.knownPages, next);
    g_reader.knownPages++;
    addedOffsets = true;
  }

  if (g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;
  if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;

  if (addedOffsets && (g_reader.knownPages % 50 == 0 || g_reader.eofReached)) {
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
  }
}

// ============================================================================
//  Reader open / render
// ============================================================================
bool openBookByIndex(int idx) {
  safeCloseCurrentBook();
  if (idx < 0 || idx >= g_library.bookCount) return false;

  String path = String(g_library.books[idx].path);
  File f = FS.open(path, "r");
  if (!f || f.isDirectory()) {
    if (f) f.close();
    return false;
  }

  g_reader.file             = f;
  g_reader.currentBookKey  = prefKeyForBook(path);
  g_reader.currentBookPath = path;
  g_reader.knownPages      = 1;
  g_reader.pageOffsets[0]  = 0;
  g_reader.eofReached      = false;
  loadPageOffsetCacheForBook(path, g_reader.file.size());

  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  File progressFile   = FS.open(progressPath.c_str(), "r");
  uint32_t savedOffset  = 0;
  int needsRelocation   = 0;

  if (progressFile) {
    String pageStr = progressFile.readStringUntil('\n');
    g_reader.pageIndex = pageStr.toInt();

    String offsetStr = progressFile.readStringUntil('\n');
    savedOffset = offsetStr.toInt();

    String relocateStr = progressFile.readStringUntil('\n');
    needsRelocation = relocateStr.toInt();

    progressFile.close();
    if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
    Serial.print("[Progress] Restored page index: ");
    Serial.print(g_reader.pageIndex);
    Serial.print(", offset: ");
    Serial.print(savedOffset);
    Serial.print(", needs relocation: ");
    Serial.println(needsRelocation);
  } else {
    g_reader.pageIndex = 0;
    Serial.println("[Progress] No saved progress, starting at page 0");
  }

  if (needsRelocation == 1 && savedOffset > 0) {
    Serial.print("[Progress] Relocating to offset ");
    Serial.println(savedOffset);
    relocateOpenBookToOffset(savedOffset);
  }

  g_reader.pageTurnsSinceFull = 0;
  resetSaveThrottle();
  syncWakeState(true);

  storeOffsetCache(path, 0, 0);

  int warmTarget = g_reader.pageIndex + PREFETCH_AHEAD_PAGES;
  if (warmTarget < 1) warmTarget = 1;
  ensureOffsetsUpTo(warmTarget);
  return true;
}

void relocateOpenBookToOffset(uint32_t targetOffset) {
  Serial.print("[Relocate] Starting relocation to offset ");
  Serial.println(targetOffset);

  int  foundPage = 0;
  bool found     = false;

  for (int i = 0; i < MAX_PAGES && !found; i++) {
    ensureOffsetsUpTo(i + 1);

    uint32_t pageStart = g_reader.pageOffsets[i];
    uint32_t pageEnd   = (i < MAX_PAGES - 1) ? g_reader.pageOffsets[i + 1] : g_reader.file.size();

    if (targetOffset >= pageStart && targetOffset < pageEnd) {
      foundPage = i;
      found     = true;
      Serial.print("[Relocate] Found offset on page ");
      Serial.println(foundPage);
      break;
    }

    if (i % 10 == 0) yield();
  }

  if (found) {
    g_reader.pageIndex = foundPage;
    Serial.print("[Relocate] Relocated to page ");
    Serial.println(foundPage);
  } else {
    Serial.println("[Relocate] Could not find offset, staying at page 0");
    g_reader.pageIndex = 0;
  }

  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  File progressFile   = FS.open(progressPath.c_str(), "r");
  if (progressFile) {
    String pageStr   = progressFile.readStringUntil('\n');
    String offsetStr = progressFile.readStringUntil('\n');
    progressFile.readStringUntil('\n');
    progressFile.close();

    FS.remove(progressPath.c_str());
    File f = FS.open(progressPath.c_str(), "w");
    if (f) {
      f.println(pageStr);
      f.println(offsetStr);
      f.println("0");
      f.close();
    }
  }
}

void drawStatusBar(uint32_t startOffset) {
  size_t total = g_reader.file.size();
  if (total == 0) total = 1;

  int pageTextW = 0;
  if (SHOW_PAGE_NUMBER) {
    u8g2.setFont(PAGE_FONT);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", g_reader.pageIndex + 1);
    pageTextW = u8g2.getUTF8Width(buf);
    u8g2.setCursor(SCREEN_W - MARGIN_X - pageTextW, SCREEN_H - 1);
    u8g2.print(buf);
    u8g2.setFont(MAIN_FONT);
  }

  if (SHOW_PROGRESS_BAR) {
    const int padR = SHOW_PAGE_NUMBER ? (pageTextW + 8) : 0;
    int w = (SCREEN_W - 2 * MARGIN_X) - padR;
    if (w < 40) w = 40;

    int x0   = MARGIN_X;
    int yTop  = SCREEN_H - 7;
    int barH  = 4;
    int filled = (int)((startOffset * (uint32_t)w) / (uint32_t)total);
    if (filled < 0) filled = 0;
    if (filled > w) filled = w;

    gfx.drawRect(x0, yTop, w, barH, 1);
    if (filled > 0) gfx.fillRect(x0 + 1, yTop + 1, max(0, filled - 2), barH - 2, 1);
  }
}

void renderCurrentPage() {
  if (!g_reader.file && !reopenCurrentBookIfNeeded()) {
    drawCenter("Open failed", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  if (!g_reader.file || g_reader.file.isDirectory()) {
    drawCenter("Open failed", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  size_t bookSize = g_reader.file.size();
  if (bookSize == 0) {
    drawCenter("Book empty", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  ensureOffsetsUpTo(g_reader.pageIndex);
  if (g_reader.knownPages <= 0) {
    drawCenter("Book empty", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
  if (g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;

  if (g_reader.pageOffsets[g_reader.pageIndex] >= bookSize) {
    g_reader.pageIndex      = 0;
    g_reader.knownPages     = 1;
    g_reader.pageOffsets[0] = 0;
    g_reader.eofReached     = false;
  }

  uint32_t start = g_reader.pageOffsets[g_reader.pageIndex];
  g_reader.lastPageStartOffset = start;
  g_reader.file.seek(start);

  bool doFull = (g_reader.pageTurnsSinceFull >= FULL_REFRESH_EVERY_N_PAGES);
  if (doFull) {
    display.fastmodeOff();
    display.clear();
    g_reader.pageTurnsSinceFull = 0;
  } else {
    display.fastmodeOn();
  }

  beginPageCanvas();
  u8g2.setFont(MAIN_FONT);

  uint32_t nextOff = readPageFromFile(g_reader.file, start, true, nullptr);
  (void)nextOff;

  bool toastActive = (g_toast.untilMs != 0) && ((int32_t)(millis() - g_toast.untilMs) <= 0);
  if (toastActive) drawToastIfActive();
  else drawStatusBar(start);
  display.update();
}
