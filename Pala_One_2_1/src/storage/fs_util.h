#ifndef PALA_STORAGE_FS_UTIL_H
#define PALA_STORAGE_FS_UTIL_H

#include <Arduino.h>
#include <stddef.h>

bool   fsBegin();
size_t fsTotalBytesSafe();
size_t fsUsedBytesSafe();
size_t fsFreeBytesSafe();
void   ensureBooksDir();
bool   ensureDirRecursive(const String& path);
bool   isDirEmpty(const String& path);

#endif // PALA_STORAGE_FS_UTIL_H
