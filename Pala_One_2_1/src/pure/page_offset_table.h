#ifndef PALA_PURE_PAGE_OFFSET_TABLE_H
#define PALA_PURE_PAGE_OFFSET_TABLE_H

#include <Arduino.h>
#include <stdint.h>

void resetOffsetCache();
bool lookupOffsetCache(const String& path, int targetPage, int& cachedPage, uint32_t& cachedOffset);
void storeOffsetCache(const String& path, int page, uint32_t offset);

#endif // PALA_PURE_PAGE_OFFSET_TABLE_H
