#include "src/ui/screens/reader_screen.h"
#include "src/state.h"
#include "src/ui/ui.h"
#include "src/reader/reader.h"
#include "src/storage/page_cache.h"
#include "src/storage/progress.h"
#include "src/hal/input.h"

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
