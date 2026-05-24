#ifndef PALA_STORAGE_PROGRESS_H
#define PALA_STORAGE_PROGRESS_H

#include <Arduino.h>
#include <stdint.h>

void          resetSaveThrottle();
void          saveProgress(bool force);
uint8_t       loadBookmarksForKey(const String& bookKey, uint16_t outPages[], uint32_t outOffsets[]);
void          saveBookmarksForKey(const String& bookKey, const uint16_t pages[], const uint32_t offsets[], uint8_t count);
const char*   addBookmarkForCurrentBook();

#endif // PALA_STORAGE_PROGRESS_H
