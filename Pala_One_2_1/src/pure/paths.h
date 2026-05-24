#ifndef PALA_PURE_PATHS_H
#define PALA_PURE_PATHS_H

#include <Arduino.h>
#include <stdint.h>

uint32_t fnv1a32(const char* s);
uint32_t hashPath32(const String& path);
String   prefKeyForBook(const String& path);
String   pageCachePathForBook(const String& path);
String   stripTxtExt(const String& s);
String   lastPathComponent(const String& path);
String   folderParent(const String& relPath);
String   prettyRelativeLabel(const String& relPath);
String   folderLeafLabel(const String& relPath);
bool     isAllowedFolderByte(uint8_t c);
String   sanitizeFolderSegment(const String& segment);
String   sanitizeFolderInput(const String& raw);
String   sanitizeUploadedFilename(String fname);

#endif // PALA_PURE_PATHS_H
