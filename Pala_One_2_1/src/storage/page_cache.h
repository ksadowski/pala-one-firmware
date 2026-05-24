#ifndef PALA_STORAGE_PAGE_CACHE_H
#define PALA_STORAGE_PAGE_CACHE_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

int    savedPageForBookPath(const String& path);
bool   loadPageOffsetCacheForBook(const String& path, size_t expectedSize);
void   savePageOffsetCacheForBook(const String& path, size_t fileSize);
void   invalidateAllPageCaches();

#endif // PALA_STORAGE_PAGE_CACHE_H
