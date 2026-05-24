#include "src/storage/page_cache.h"
#include "src/state.h"
#include "src/pure/paths.h"
#include "src/pure/page_offset_table.h"

int savedPageForBookPath(const String& path) {
  String key = prefKeyForBook(path);
  int p = prefs.getInt((key + "_p").c_str(), 0);
  return (p < 0) ? 0 : p;
}

bool loadPageOffsetCacheForBook(const String& path, size_t expectedSize) {
  String cachePath = pageCachePathForBook(path);
  File f = FS.open(cachePath, "r");
  if (!f) return false;

  uint32_t magic    = 0;
  uint32_t fileSize = 0;
  uint16_t count    = 0;

  if (f.read((uint8_t*)&magic,    sizeof(magic))    != sizeof(magic))    { f.close(); return false; }
  if (f.read((uint8_t*)&fileSize, sizeof(fileSize)) != sizeof(fileSize)) { f.close(); return false; }
  if (f.read((uint8_t*)&count,    sizeof(count))    != sizeof(count))    { f.close(); return false; }

  if (magic != 0x50434F46UL || fileSize != (uint32_t)expectedSize || count == 0 || count > MAX_PAGES) {
    f.close();
    return false;
  }

  g_reader.knownPages = 0;
  for (uint16_t i = 0; i < count; i++) {
    uint32_t off = 0;
    if (f.read((uint8_t*)&off, sizeof(off)) != sizeof(off)) break;
    g_reader.pageOffsets[i] = off;
    g_reader.knownPages++;
  }
  f.close();

  if (g_reader.knownPages == 0) {
    g_reader.knownPages = 1;
    g_reader.pageOffsets[0] = 0;
    return false;
  }
  return true;
}

void savePageOffsetCacheForBook(const String& path, size_t fileSize) {
  if (g_reader.knownPages <= 1) return;

  String cachePath = pageCachePathForBook(path);
  File f = FS.open(cachePath, "w");
  if (!f) return;

  uint32_t magic   = 0x50434F46UL;
  uint32_t size32  = (uint32_t)fileSize;
  uint16_t count16 = (uint16_t)min(g_reader.knownPages, MAX_PAGES);

  f.write((const uint8_t*)&magic,   sizeof(magic));
  f.write((const uint8_t*)&size32,  sizeof(size32));
  f.write((const uint8_t*)&count16, sizeof(count16));
  f.write((const uint8_t*)g_reader.pageOffsets, count16 * sizeof(uint32_t));
  f.close();
}

void invalidateAllPageCaches() {
  resetOffsetCache();

  File root = FS.open("/");
  if (root && root.isDirectory()) {
    File f = root.openNextFile();
    while (f) {
      String name = String(f.name());
      bool removeIt = name.startsWith("/pc_") && name.endsWith(".bin");
      f.close();
      if (removeIt) FS.remove(name);
      f = root.openNextFile();
    }
    root.close();
  } else if (root) {
    root.close();
  }

  File progressDir = FS.open("/books/.progress");
  if (progressDir && progressDir.isDirectory()) {
    File f = progressDir.openNextFile();
    while (f) {
      String name = String(f.name());
      if (name.endsWith(".txt")) {
        f.close();
        String progressPath = "/books/.progress/" + name;
        File progressFile = FS.open(progressPath.c_str(), "r");
        if (progressFile) {
          String pageStr   = progressFile.readStringUntil('\n');
          String offsetStr = progressFile.readStringUntil('\n');
          progressFile.readStringUntil('\n');
          progressFile.close();

          uint32_t currentOffset = offsetStr.toInt();

          FS.remove(progressPath.c_str());
          File newFile = FS.open(progressPath.c_str(), "w");
          if (newFile) {
            newFile.println(pageStr);
            newFile.println(currentOffset);
            newFile.println("1");
            newFile.close();
            Serial.print("[Invalidate] Marked for relocation: ");
            Serial.println(name);
          }
        }
      } else {
        f.close();
      }
      f = progressDir.openNextFile();
    }
    progressDir.close();
  }

  if (g_reader.currentBookPath.length() > 0) {
    uint32_t currentOffset = g_reader.pageOffsets[g_reader.pageIndex];
    String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
    File progressFile = FS.open(progressPath.c_str(), "r");
    if (progressFile) {
      String pageStr   = progressFile.readStringUntil('\n');
      progressFile.readStringUntil('\n');
      progressFile.readStringUntil('\n');
      progressFile.close();

      FS.remove(progressPath.c_str());
      File newFile = FS.open(progressPath.c_str(), "w");
      if (newFile) {
        newFile.println(pageStr);
        newFile.println(currentOffset);
        newFile.println("1");
        newFile.close();
        Serial.print("[Invalidate] Marked current book for relocation with offset ");
        Serial.println(currentOffset);
      }
    }
  }
}
