#include "src/storage/progress.h"
#include "src/state.h"
#include "src/pure/text_util.h"
#include "src/storage/page_cache.h"

void resetSaveThrottle() {
  g_reader.lastSaveMs    = 0;
  g_reader.lastSavedPage = -1;
}

void saveProgress(bool force) {
  if (!g_reader.file) return;

  if (!force) {
    if (g_reader.pageIndex == g_reader.lastSavedPage) return;
  }

  Serial.print("[Progress] Saving progress for book: ");
  Serial.print(g_reader.currentBookKey);
  Serial.print(", page: ");
  Serial.print(g_reader.pageIndex);
  Serial.print(", offset: ");
  Serial.println(g_reader.pageOffsets[g_reader.pageIndex]);

  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  FS.remove(progressPath.c_str());
  File f = FS.open(progressPath.c_str(), "w");
  if (f) {
    f.println(g_reader.pageIndex);
    f.println(g_reader.pageOffsets[g_reader.pageIndex]);
    f.println("0");
    f.close();
  }

  String segmentPath = "/books/.progress/" + g_reader.currentBookKey + "_seg.txt";
  if (FS.exists(segmentPath.c_str())) {
    FS.remove(segmentPath.c_str());
  }

  g_reader.lastSaveMs    = millis();
  g_reader.lastSavedPage = g_reader.pageIndex;
}

uint8_t loadBookmarksForKey(const String& bookKey, uint16_t outPages[], uint32_t outOffsets[]) {
  const size_t NEW_SIZE = 1 + MAX_BOOKMARKS * 6;
  uint8_t buf[NEW_SIZE] = {0};
  size_t got = prefs.getBytes(bmKeyFor(bookKey).c_str(), buf, sizeof(buf));
  if (got < 1) return 0;

  uint8_t count = buf[0];
  if (count > MAX_BOOKMARKS) count = MAX_BOOKMARKS;

  bool hasOffsets = (got >= (size_t)(1 + count * 6));
  if (hasOffsets) {
    for (uint8_t i = 0; i < count; i++) {
      size_t base = 1 + (size_t)i * 6;
      outPages[i]   = (uint16_t)(buf[base + 0] | (buf[base + 1] << 8));
      outOffsets[i] = (uint32_t)buf[base + 2]         |
                      ((uint32_t)buf[base + 3] << 8)  |
                      ((uint32_t)buf[base + 4] << 16) |
                      ((uint32_t)buf[base + 5] << 24);
    }
  } else {
    for (uint8_t i = 0; i < count; i++) {
      size_t base = 1 + (size_t)i * 2;
      outPages[i]   = (uint16_t)(buf[base + 0] | (buf[base + 1] << 8));
      outOffsets[i] = 0xFFFFFFFFUL;
    }
  }
  return count;
}

void saveBookmarksForKey(const String& bookKey, const uint16_t pages[], const uint32_t offsets[], uint8_t count) {
  if (count > MAX_BOOKMARKS) count = MAX_BOOKMARKS;
  const size_t BUF_SIZE = 1 + MAX_BOOKMARKS * 6;
  uint8_t buf[BUF_SIZE] = {0};
  buf[0] = count;
  for (uint8_t i = 0; i < count; i++) {
    size_t   base = 1 + (size_t)i * 6;
    uint16_t page = pages[i];
    uint32_t off  = offsets[i];
    buf[base + 0] = (uint8_t)(page & 0xFF);
    buf[base + 1] = (uint8_t)((page >> 8) & 0xFF);
    buf[base + 2] = (uint8_t)(off & 0xFF);
    buf[base + 3] = (uint8_t)((off >> 8)  & 0xFF);
    buf[base + 4] = (uint8_t)((off >> 16) & 0xFF);
    buf[base + 5] = (uint8_t)((off >> 24) & 0xFF);
  }
  prefs.putBytes(bmKeyFor(bookKey).c_str(), buf, 1 + count * 6);
}

const char* addBookmarkForCurrentBook() {
  if (g_reader.currentBookKey.length() == 0) return nullptr;

  uint16_t pages[MAX_BOOKMARKS];
  uint32_t offsets[MAX_BOOKMARKS];
  uint8_t count = loadBookmarksForKey(g_reader.currentBookKey, pages, offsets);

  for (uint8_t i = 0; i < count; i++) {
    if ((int)pages[i] == g_reader.pageIndex) return "Bookmark exists";
  }

  uint32_t currentOffset = g_reader.lastPageStartOffset;

  if (count < MAX_BOOKMARKS) {
    pages[count]   = (uint16_t)g_reader.pageIndex;
    offsets[count] = currentOffset;
    count++;
  } else {
    for (uint8_t i = 1; i < MAX_BOOKMARKS; i++) {
      pages[i - 1]   = pages[i];
      offsets[i - 1] = offsets[i];
    }
    pages[MAX_BOOKMARKS - 1]   = (uint16_t)g_reader.pageIndex;
    offsets[MAX_BOOKMARKS - 1] = currentOffset;
    count = MAX_BOOKMARKS;
  }

  for (uint8_t i = 0; i < count; i++) {
    for (uint8_t j = i + 1; j < count; j++) {
      if (pages[j] < pages[i]) {
        uint16_t tp = pages[i]; pages[i] = pages[j]; pages[j] = tp;
        uint32_t to = offsets[i]; offsets[i] = offsets[j]; offsets[j] = to;
      }
    }
  }

  saveBookmarksForKey(g_reader.currentBookKey, pages, offsets, count);
  if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
  return "Bookmark saved";
}
