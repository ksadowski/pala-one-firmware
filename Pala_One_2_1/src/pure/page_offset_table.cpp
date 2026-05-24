#include "src/pure/page_offset_table.h"
#include "src/state.h"
#include "src/pure/paths.h"

void resetOffsetCache() {
  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    g_offsetCache[i].pathHash = 0;
    g_offsetCache[i].page = -1;
    g_offsetCache[i].offset = 0;
    g_offsetCache[i].stamp = 0;
  }
  g_offsetCacheStamp = 1;
}

bool lookupOffsetCache(const String& path, int targetPage, int& cachedPage, uint32_t& cachedOffset) {
  uint32_t h = hashPath32(path);
  bool found = false;
  int bestPage = -1;
  uint32_t bestOffset = 0;
  uint32_t bestStamp = 0;

  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    if (g_offsetCache[i].pathHash != h) continue;
    if (g_offsetCache[i].page > targetPage) continue;
    if (g_offsetCache[i].page > bestPage || (g_offsetCache[i].page == bestPage && g_offsetCache[i].stamp > bestStamp)) {
      bestPage = g_offsetCache[i].page;
      bestOffset = g_offsetCache[i].offset;
      bestStamp = g_offsetCache[i].stamp;
      found = true;
    }
  }

  if (found) {
    cachedPage = bestPage;
    cachedOffset = bestOffset;
  }
  return found;
}

void storeOffsetCache(const String& path, int page, uint32_t offset) {
  if (page < 0) return;

  uint32_t h = hashPath32(path);
  int slot = -1;
  uint32_t oldestStamp = 0xFFFFFFFFu;

  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    if (g_offsetCache[i].pathHash == h && g_offsetCache[i].page == page) {
      slot = i;
      break;
    }
    if (g_offsetCache[i].page < 0) {
      slot = i;
      break;
    }
    if (g_offsetCache[i].stamp < oldestStamp) {
      oldestStamp = g_offsetCache[i].stamp;
      slot = i;
    }
  }

  if (slot < 0) return;
  g_offsetCache[slot].pathHash = h;
  g_offsetCache[slot].page = page;
  g_offsetCache[slot].offset = offset;
  g_offsetCache[slot].stamp = g_offsetCacheStamp++;
  if (g_offsetCacheStamp == 0) g_offsetCacheStamp = 1;
}
