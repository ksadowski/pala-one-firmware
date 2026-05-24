#ifndef PALA_STORAGE_LIBRARY_H
#define PALA_STORAGE_LIBRARY_H

#include <Arduino.h>

bool   isFolderExpanded(int idx);
void   setFolderExpanded(int idx, bool expanded);
String bookLeafLabel(int idx);
void   loadListItems();
void   saveListItems();
bool   listHasVisibleItems();
void   deleteBookMetadata(const String& path);
void   migrateBookMetadata(const String& oldPath, const String& newPath);
void   loadBooks();
bool   libraryFolderExists(const String& folderRel);
String libraryEntryLabel(int idx);
void   buildLibraryEntries();

#endif // PALA_STORAGE_LIBRARY_H
