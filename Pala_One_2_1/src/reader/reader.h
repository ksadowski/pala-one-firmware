#ifndef PALA_READER_READER_H
#define PALA_READER_READER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <stdint.h>

// ---- Book / FS helpers ----
void     safeCloseCurrentBook();
void     clearCurrentBookState();
void     resetPreviewState();
void     resetUiEphemeralState();
void     resetNavigationState();
bool     reopenCurrentBookIfNeeded();
void     syncWakeState(bool reading);
void     enterLibraryRoot(bool redraw = true);

// ---- Pagination / text layout ----
uint32_t readPageFromFile(File& f, uint32_t startPos, bool draw, String* outText);
uint32_t buildNextOffsetFor(File& f, uint32_t startPos);
uint32_t buildNextOffset(uint32_t startPos);
uint32_t pageOffsetForPage(File& f, const String& path, int page);
void     ensureOffsetsUpTo(int targetPage);

// ---- Search / navigation ----
uint32_t searchInBook(File& f, const String& phrase);
int      findPageForOffset(const String& path, uint32_t offset);

// ---- Reader open / render ----
bool     openBookByIndex(int idx);
void     relocateOpenBookToOffset(uint32_t targetOffset);
void     drawStatusBar(uint32_t startOffset);
void     renderCurrentPage();

#endif // PALA_READER_READER_H
