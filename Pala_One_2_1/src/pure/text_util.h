#ifndef PALA_PURE_TEXT_UTIL_H
#define PALA_PURE_TEXT_UTIL_H

#include <Arduino.h>
#include <FS.h>
#include <stdint.h>

static inline bool isUtf8ContinuationByte(uint8_t b) {
  return (b & 0xC0) == 0x80;
}
static inline bool isBookmarkLabelWordChar(char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         ((uint8_t)c >= 128);
}

int    utf8CharLenFromLead(uint8_t b);
int    utf8SafeCharLenAt(const String& s, int index);
String utf8CharAt(const String& s, int index);
bool   isBreakableWhitespaceChar(const String& ch);
bool   isBreakablePunctuationChar(const String& ch);
String normalizeTypography(const String& in);
String compactText(const String& in);
String readBookmarkLabelAtOffset(File& f, uint32_t off, int page);
void   trimTrailingSpaces(String& s);
void   trimLeadingSpaces(String& s);
bool   lineEndsWithSpace(const String& s);
void   sanitizeListText(String& s);
String bmKeyFor(const String& bookKey);

#endif // PALA_PURE_TEXT_UTIL_H
