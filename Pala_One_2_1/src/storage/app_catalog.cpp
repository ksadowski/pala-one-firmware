#include "src/storage/app_catalog.h"
#include "pala_app.h"
#include "src/state.h"
#include "src/pure/paths.h"

void scanApps() {
  g_apps.count = 0;

  if (!FS.exists("/apps")) {
    FS.mkdir("/apps");
    return;
  }

  File dir = FS.open("/apps");
  if (!dir || !dir.isDirectory()) {
    if (dir) dir.close();
    return;
  }

  File f = dir.openNextFile();
  while (f && g_apps.count < MAX_APPS) {
    String entryName = String(f.name());
    f.close();

    if (!entryName.endsWith(".bin")) {
      f = dir.openNextFile();
      continue;
    }

    String absPath = entryName.startsWith("/") ? entryName : (String("/apps/") + entryName);
    AppDiscovery& entry = g_apps.apps[g_apps.count];

    File hf = FS.open(absPath, "r");
    bool usedHeader = false;
    if (hf && hf.size() >= sizeof(PalaAppHeader)) {
      PalaAppHeader hdr;
      if (hf.read((uint8_t*)&hdr, sizeof(hdr)) == sizeof(hdr) &&
          hdr.magic == PALA_APP_MAGIC) {
        hdr.name[MAX_APP_NAME] = '\0';
        strncpy(entry.name, hdr.name, MAX_APP_NAME);
        entry.name[MAX_APP_NAME] = '\0';
        usedHeader = true;
      }
    }
    if (hf) hf.close();

    if (!usedHeader) {
      String stem = lastPathComponent(absPath);
      if (stem.endsWith(".bin")) stem = stem.substring(0, stem.length() - 4);
      stem.replace('_', ' ');
      strncpy(entry.name, stem.c_str(), MAX_APP_NAME);
      entry.name[MAX_APP_NAME] = '\0';
    }

    strncpy(entry.path, absPath.c_str(), MAX_APP_PATH);
    entry.path[MAX_APP_PATH] = '\0';
    g_apps.count++;

    f = dir.openNextFile();
  }
  if (f) f.close();
  dir.close();

  if (g_apps.selectedIndex >= g_apps.count) g_apps.selectedIndex = 0;
}
