#include "src/ble/ble_server.h"
#include "src/state.h"
#include "src/config.h"
#include "src/settings/settings.h"
#include "src/storage/page_cache.h"
#include "src/storage/progress.h"
#include "src/storage/app_catalog.h"
#include "src/pure/paths.h"
#include "src/pure/text_util.h"
#include "src/reader/reader.h"
#include "src/ui/ui.h"
#include "src/webui/web_helpers.h"

// ============================================================================
//  BLE callback classes
// ============================================================================
class PalaBLEServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    g_bleConnected = true;
    lastUserActionMs = millis(); // Reset sleep timer on connect
    sendBLEStatus("connected");
    Serial.println("[BLE] Connection established");
    // Note: PHY mode is negotiated automatically during connection
    // The ESP32 BLE library being used doesn't expose PHY mode directly
  }
  void onDisconnect(BLEServer* pServer) {
    g_bleConnected = false;
    sendBLEStatus("disconnected");
    Serial.println("[BLE] Disconnected");
    // Restart advertising after disconnect
    BLEDevice::startAdvertising();
  }
};

class BLECommandCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    // Reset sleep timer on any BLE activity
    lastUserActionMs = millis();

    std::string cmd = std::string(pCharacteristic->getValue().c_str());
    if (cmd.length() > 0) {
      handleBLECommand(cmd);
    }
  }
};

class BLEDataCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    if (g_bleTransferState != BLE_TRANSFER_UPLOADING) return;

    lastUserActionMs = millis();

    String arduinoData = pCharacteristic->getValue();
    uint32_t len = (uint32_t)arduinoData.length();
    if (len == 0) return;

    const uint8_t* src = (const uint8_t*)arduinoData.c_str();

    // Check ring buffer capacity (head - tail = bytes currently buffered)
    uint32_t used = g_bleRamBufHead - g_bleRamBufTail;
    if (BLE_RAM_BUF_SIZE - used < len) {
      Serial.println("[BLE Upload] RAM buffer full, dropping chunk");
      return;
    }

    // Write into ring buffer, handling wrap-around
    uint32_t idx = g_bleRamBufHead & (BLE_RAM_BUF_SIZE - 1);
    if (idx + len <= BLE_RAM_BUF_SIZE) {
      memcpy(g_bleRamBuf + idx, src, len);
    } else {
      uint32_t part1 = BLE_RAM_BUF_SIZE - idx;
      memcpy(g_bleRamBuf + idx, src, part1);
      memcpy(g_bleRamBuf,       src + part1, len - part1);
    }
    g_bleRamBufHead += len; // publish to main loop

    g_bleTransferOffset += len;

    // ACK immediately — flash write happens in main loop
    String ack = "ACK:" + String(g_bleTransferOffset);
    sendBLEStatus(ack.c_str());

    if (g_bleTransferOffset >= g_bleTransferTotalSize) {
      Serial.println("[BLE Upload] All bytes received into RAM buffer");
      g_bleAllReceived = true;
    }
  }
};

// ============================================================================
//  Status sender
// ============================================================================
void sendBLEStatus(const char* msg) {
  if (g_bleCharStatus && g_bleConnected) {
    g_bleCharStatus->setValue(msg);
    g_bleCharStatus->notify();
  }
}

// ============================================================================
//  Command handler
// ============================================================================
void handleBLECommand(std::string cmd) {
  Serial.print("[BLE] Command: ");
  Serial.println(cmd.c_str());

  String command(cmd.c_str());
  command.trim();

  if (command == "LIST_FILES") {
    String json = "[";
    bool first = true;
    for (int i = 0; i < g_library.bookCount; i++) {
      if (!first) json += ",";
      first = false;
      json += "{\"name\":\"" + String(g_library.books[i].name) + "\",";
      json += "\"path\":\"" + String(g_library.books[i].path) + "\",";
      json += "\"size\":" + String(g_library.books[i].size) + "}";
    }
    json += "]";
    if (g_bleCharData && g_bleConnected) {
      g_bleCharData->setValue(json.c_str());
      g_bleCharData->notify();
    }
    sendBLEStatus("files_list_sent");
  }
  else if (command == "LIST_APPS") {
    String json = "[";
    bool first = true;
    if (FS.exists("/apps")) {
      File appsDir = FS.open("/apps");
      if (appsDir) {
        File f = appsDir.openNextFile();
        while (f) {
          String name = String(f.name());
          if (name.endsWith(".bin")) {
            if (!first) json += ",";
            first = false;
            json += "{\"name\":\"" + htmlEscape(name) + "\",";
            json += "\"path\":\"/apps/" + htmlEscape(name) + "\",";
            json += "\"size\":" + String((int)f.size()) + "}";
          }
          f.close();
          f = appsDir.openNextFile();
        }
        appsDir.close();
      }
    }
    json += "]";
    if (g_bleCharData && g_bleConnected) {
      g_bleCharData->setValue(json.c_str());
      g_bleCharData->notify();
    }
    sendBLEStatus("apps_list_sent");
  }
  else if (command.startsWith("UPLOAD:")) {
    String params = command.substring(7);
    Serial.print("[BLE] UPLOAD command: ");
    Serial.println(params);
    int colonIdx = params.indexOf(':', 1);
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      uint32_t size = params.substring(colonIdx + 1).toInt();
      Serial.print("[BLE] Path: "); Serial.println(path);
      Serial.print("[BLE] Size: "); Serial.println(size);
      if (size > 0 && size < 10 * 1024 * 1024) {
        g_uploadPath   = path;
        g_uploadSize   = size;
        g_uploadIsApp  = false;
        g_initUpload   = true;
        Serial.println("[BLE] Deferred upload initialization to main loop");
      } else {
        Serial.println("[BLE] Invalid size");
        sendBLEStatus("upload_error:invalid_size");
      }
    } else {
      Serial.println("[BLE] Invalid params");
      sendBLEStatus("upload_error:invalid_params");
    }
  }
  else if (command.startsWith("UPLOAD_APP:")) {
    String params = command.substring(11);
    Serial.print("[BLE] UPLOAD_APP command: ");
    Serial.println(params);
    int colonIdx = params.indexOf(':', 1);
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      uint32_t size = params.substring(colonIdx + 1).toInt();
      Serial.print("[BLE] Path: "); Serial.println(path);
      Serial.print("[BLE] Size: "); Serial.println(size);
      const size_t MAX_APP_BINARY = 48 * 1024;
      if (size > 0 && size <= MAX_APP_BINARY) {
        g_uploadPath   = path;
        g_uploadSize   = size;
        g_uploadIsApp  = true;
        g_initUpload   = true;
        Serial.println("[BLE] Deferred app upload initialization to main loop");
      } else {
        Serial.println("[BLE] Invalid app size");
        sendBLEStatus("upload_error:invalid_size");
      }
    } else {
      Serial.println("[BLE] Invalid params");
      sendBLEStatus("upload_error:invalid_params");
    }
  }
  else if (command.startsWith("DOWNLOAD:")) {
    String path = command.substring(9);
    if (FS.exists(path)) {
      g_bleTransferFile = FS.open(path, "r");
      if (g_bleTransferFile) {
        g_bleTransferPath       = path;
        g_bleTransferOffset     = 0;
        g_bleTransferTotalSize  = g_bleTransferFile.size();
        g_bleTransferBytesSent  = 0;
        g_bleTransferState      = BLE_TRANSFER_DOWNLOADING;
        String sizeMsg = "DOWNLOAD_READY:" + String(g_bleTransferTotalSize);
        sendBLEStatus(sizeMsg.c_str());
      } else {
        sendBLEStatus("download_error:cannot_open");
      }
    } else {
      sendBLEStatus("download_error:not_found");
    }
  }
  else if (command == "DOWNLOAD_NEXT_CHUNK") {
    if (g_bleTransferState == BLE_TRANSFER_DOWNLOADING && g_bleTransferFile) {
      uint8_t chunk[BLE_CHUNK_SIZE];
      g_bleTransferFile.seek(g_bleTransferOffset);
      int bytesRead = g_bleTransferFile.read(chunk, BLE_CHUNK_SIZE);
      if (bytesRead > 0) {
        g_bleCharData->setValue(chunk, bytesRead);
        g_bleCharData->notify();
        g_bleTransferOffset += bytesRead;
        if (g_bleTransferOffset >= g_bleTransferTotalSize) {
          g_bleTransferFile.close();
          g_bleTransferState = BLE_TRANSFER_IDLE;
          sendBLEStatus("download_complete");
        } else {
          String offsetMsg = "CHUNK_SENT:" + String(g_bleTransferOffset);
          sendBLEStatus(offsetMsg.c_str());
        }
      } else {
        g_bleTransferFile.close();
        g_bleTransferState = BLE_TRANSFER_IDLE;
        sendBLEStatus("download_error:read_failed");
      }
    } else {
      sendBLEStatus("download_error:not_active");
    }
  }
  else if (command == "GET_SETTINGS") {
    String json = "{";
    json += "\"fontSize\":"              + String(g_settings.fontSize)              + ",";
    json += "\"fontFamily\":"            + String(g_settings.fontFamily)            + ",";
    json += "\"fontWeight\":"            + String(g_settings.fontWeight)            + ",";
    json += "\"sleepSecs\":"             + String(g_settings.sleepSecs)             + ",";
    json += "\"lineGap\":"               + String(g_settings.lineGap)               + ",";
    json += "\"readerLongPressAction\":" + String(g_settings.readerLongPressAction);
    json += "}";
    if (g_bleCharData && g_bleConnected) {
      g_bleCharData->setValue(json.c_str());
      g_bleCharData->notify();
    }
    sendBLEStatus("settings_sent");
  }
  else if (command.startsWith("SET_SETTINGS:")) {
    String settingsJson = command.substring(13);
    bool layoutChanged = false;
    bool wasReading = (mode == MODE_READER) && g_reader.file;

    Serial.print("[BLE] Settings before: fontSize=");
    Serial.print(g_settings.fontSize);
    Serial.print(", fontFamily=");
    Serial.print(g_settings.fontFamily);
    Serial.print(", sleepSecs=");
    Serial.print(g_settings.sleepSecs);
    Serial.print(", lineGap=");
    Serial.println(g_settings.lineGap);

    // Parse fontFamily
    int ffIdx = settingsJson.indexOf("\"fontFamily\":");
    if (ffIdx >= 0) {
      int valStart = ffIdx + 13;
      int valEnd = settingsJson.indexOf(',', valStart);
      if (valEnd < 0) valEnd = settingsJson.indexOf('}', valStart);
      if (valEnd > valStart) {
        int ff = settingsJson.substring(valStart, valEnd).toInt();
        if (ff >= 0 && ff <= 3) {
          if (ff != g_settings.fontFamily) {
            g_settings.fontFamily = ff;
            prefs.putInt("cfg_font_family", ff);
            applyFontSize(g_settings.fontSize);
            layoutChanged = true;
            Serial.print("[BLE] Changed fontFamily to "); Serial.println(ff);
          }
        }
      }
    }

    // Parse fontWeight
    int fwIdx = settingsJson.indexOf("\"fontWeight\":");
    if (fwIdx >= 0) {
      int valStart = fwIdx + 13;
      int valEnd = settingsJson.indexOf(',', valStart);
      if (valEnd < 0) valEnd = settingsJson.indexOf('}', valStart);
      if (valEnd > valStart) {
        int fw = settingsJson.substring(valStart, valEnd).toInt();
        if (fw >= 0 && fw <= 4) {
          if (fw != g_settings.fontWeight) {
            g_settings.fontWeight = fw;
            prefs.putInt("cfg_font_weight", fw);
            applyFontSize(g_settings.fontSize);
            layoutChanged = true;
            Serial.print("[BLE] Changed fontWeight to "); Serial.println(fw);
          }
        }
      }
    }

    // Parse fontSize
    int fsIdx = settingsJson.indexOf("\"fontSize\":");
    if (fsIdx >= 0) {
      int valStart = fsIdx + 11;
      int valEnd = settingsJson.indexOf(',', valStart);
      if (valEnd < 0) valEnd = settingsJson.indexOf('}', valStart);
      if (valEnd > valStart) {
        int fs = settingsJson.substring(valStart, valEnd).toInt();
        if (fs == 12 || fs == 14 || fs == 16 || fs == 18 || fs == 20) {
          if (fs != g_settings.fontSize) {
            applyFontSize(fs);
            prefs.putInt("cfg_font", fs);
            layoutChanged = true;
            Serial.print("[BLE] Attempt started: changing fontSize to "); Serial.println(fs);
          }
        }
      }
    }

    // Parse sleepSecs
    int ssIdx = settingsJson.indexOf("\"sleepSecs\":");
    if (ssIdx >= 0) {
      int valStart = ssIdx + 12;
      int valEnd = settingsJson.indexOf(',', valStart);
      if (valEnd < 0) valEnd = settingsJson.indexOf('}', valStart);
      if (valEnd > valStart) {
        int ss = settingsJson.substring(valStart, valEnd).toInt();
        if (ss >= 10 && ss <= 3600) {
          if ((uint32_t)ss != g_settings.sleepSecs) {
            g_settings.sleepSecs = (uint32_t)ss;
            prefs.putInt("cfg_sleep", ss);
          }
        }
      }
    }

    // Parse lineGap
    int lgIdx = settingsJson.indexOf("\"lineGap\":");
    if (lgIdx >= 0) {
      int valStart = lgIdx + 10;
      int valEnd = settingsJson.indexOf(',', valStart);
      if (valEnd < 0) valEnd = settingsJson.indexOf('}', valStart);
      if (valEnd > valStart) {
        int lg = settingsJson.substring(valStart, valEnd).toInt();
        if (lg >= 0 && lg <= 4) {
          if (lg != g_settings.lineGap) {
            g_settings.lineGap = lg;
            prefs.putInt("cfg_lgap", lg);
            invalidateMetrics();
            layoutChanged = true;
          }
        }
      }
    }

    if (layoutChanged && wasReading) {
      Serial.println("[BLE] Layout changed, invalidating page caches");
      invalidateAllPageCaches();
    }
    if (layoutChanged) {
      invalidateAllPageCaches();
      Serial.print("[BLE] Attempt succeeded: fontSize updated to "); Serial.println(g_settings.fontSize);
    }
    sendBLEStatus("settings_updated");
    g_redrawCurrentMode = true;
  }
  else if (command.startsWith("GET_ALL_BOOKMARKS")) {
    g_fetchAllBookmarks = true;
  }
  else if (command.startsWith("GET_BOOKMARKS:")) {
    String path = command.substring(14);
    String key = prefKeyForBook(path);
    uint16_t pages[MAX_BOOKMARKS];
    uint32_t offsets[MAX_BOOKMARKS];
    uint8_t count = loadBookmarksForKey(key, pages, offsets);

    File f;
    bool fileOpen = false;
    if (FS.exists(path)) {
      f = FS.open(path, "r");
      fileOpen = f;
    }

    String json = "{\"book\":\"" + path + "\",\"bookmarks\":[";
    for (uint8_t i = 0; i < count; i++) {
      if (i > 0) json += ",";
      json += "{\"page\":" + String(pages[i]) + ",\"offset\":" + String(offsets[i]);
      if (fileOpen) {
        uint32_t resolvedOffset = resolveBookmarkOffset(path, pages[i], offsets[i]);
        String label = readBookmarkLabelAtOffset(f, resolvedOffset, pages[i]);
        label.replace("\\", "\\\\");
        label.replace("\"", "\\\"");
        Serial.print("[BLE] Bookmark label: "); Serial.println(label);
        json += ",\"label\":\"" + label + "\"";
      } else {
        json += ",\"label\":\"Page " + String(pages[i] + 1) + "\"";
      }
      json += "}";
    }
    json += "]}";
    Serial.print("[BLE] Bookmark JSON: "); Serial.println(json);
    if (fileOpen) f.close();
    if (g_bleCharData && g_bleConnected) {
      g_bleCharData->setValue(json.c_str());
      g_bleCharData->notify();
    }
    sendBLEStatus("bookmarks_sent");
  }
  else if (command.startsWith("VIEW_BOOKMARK:")) {
    String params = command.substring(14);
    int colonIdx = params.indexOf(':', 1);
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      int page = params.substring(colonIdx + 1).toInt();

      String key = prefKeyForBook(path);
      uint16_t pages[MAX_BOOKMARKS];
      uint32_t offsets[MAX_BOOKMARKS];
      uint8_t count = loadBookmarksForKey(key, pages, offsets);

      uint32_t offset = 0xFFFFFFFF;
      for (uint8_t i = 0; i < count; i++) {
        if ((int)pages[i] == page) { offset = offsets[i]; break; }
      }

      if (offset == 0xFFFFFFFF) {
        sendBLEStatus("bookmark_not_found");
      } else {
        File f = FS.open(path, "r");
        if (f) {
          uint32_t resolvedOffset = resolveBookmarkOffset(path, (uint16_t)page, offset);
          String txt;
          txt.reserve(900);
          readPageFromFile(f, resolvedOffset, false, &txt);
          f.close();
          txt.trim();
          if (txt.length() == 0) txt = "(empty)";

          String json = "{\"path\":\"" + path + "\",\"page\":" + String(page) + ",\"text\":\"";
          txt.replace("\\", "\\\\");
          txt.replace("\"", "\\\"");
          txt.replace("\n", "\\n");
          txt.replace("\r", "\\r");
          txt.replace("\t", "\\t");
          json += txt + "\"}";
          Serial.print("[BLE] Bookmark view JSON: "); Serial.println(json);

          int jsonLen = json.length();
          int chunkSize = 512;
          for (int offset = 0; offset < jsonLen; offset += chunkSize) {
            int endIdx = min(offset + chunkSize, jsonLen);
            String chunk = json.substring(offset, endIdx);
            if (g_bleCharData && g_bleConnected) {
              g_bleCharData->setValue(chunk.c_str());
              g_bleCharData->notify();
            }
            delay(10);
          }
          sendBLEStatus("bookmark_view_sent");
        } else {
          sendBLEStatus("file_open_failed");
        }
      }
    } else {
      sendBLEStatus("invalid_params");
    }
  }
  else if (command.startsWith("ADD_BOOKMARK:")) {
    String bookmarkJson = command.substring(13);
    int pathIdx = bookmarkJson.indexOf("\"path\":\"");
    int pageIdx = bookmarkJson.indexOf("\"page\":");
    if (pathIdx >= 0 && pageIdx >= 0) {
      int pathStart = pathIdx + 8;
      int pathEnd   = bookmarkJson.indexOf('"', pathStart);
      int pageStart = pageIdx + 7;
      int pageEnd   = bookmarkJson.indexOf(',', pageStart);
      if (pageEnd < 0) pageEnd = bookmarkJson.indexOf('}', pageStart);

      if (pathEnd > pathStart && pageEnd > pageStart) {
        String path = bookmarkJson.substring(pathStart, pathEnd);
        int page    = bookmarkJson.substring(pageStart, pageEnd).toInt();
        String key  = prefKeyForBook(path);

        uint16_t pages[MAX_BOOKMARKS];
        uint32_t offsets[MAX_BOOKMARKS];
        uint8_t count = loadBookmarksForKey(key, pages, offsets);

        bool exists = false;
        for (uint8_t i = 0; i < count; i++) {
          if ((int)pages[i] == page) { exists = true; break; }
        }

        if (!exists && count < MAX_BOOKMARKS) {
          pages[count]   = (uint16_t)page;
          offsets[count] = 0xFFFFFFFFUL;
          count++;
          saveBookmarksForKey(key, pages, offsets, count);
          sendBLEStatus("bookmark_added");
        } else if (exists) {
          sendBLEStatus("bookmark_exists");
        } else {
          sendBLEStatus("bookmarks_full");
        }
      } else { sendBLEStatus("invalid_json"); }
    } else { sendBLEStatus("invalid_json"); }
  }
  else if (command.startsWith("DELETE_BOOKMARK:")) {
    String bookmarkJson = command.substring(16);
    int pathIdx = bookmarkJson.indexOf("\"path\":\"");
    int pageIdx = bookmarkJson.indexOf("\"page\":");
    if (pathIdx >= 0 && pageIdx >= 0) {
      int pathStart = pathIdx + 8;
      int pathEnd   = bookmarkJson.indexOf('"', pathStart);
      int pageStart = pageIdx + 7;
      int pageEnd   = bookmarkJson.indexOf(',', pageStart);
      if (pageEnd < 0) pageEnd = bookmarkJson.indexOf('}', pageStart);

      if (pathEnd > pathStart && pageEnd > pageStart) {
        String path = bookmarkJson.substring(pathStart, pathEnd);
        int page    = bookmarkJson.substring(pageStart, pageEnd).toInt();
        String key  = prefKeyForBook(path);

        uint16_t pages[MAX_BOOKMARKS];
        uint32_t offsets[MAX_BOOKMARKS];
        uint8_t count = loadBookmarksForKey(key, pages, offsets);

        bool found = false;
        for (uint8_t i = 0; i < count; i++) {
          if ((int)pages[i] == page) {
            found = true;
            for (uint8_t j = i; j < count - 1; j++) {
              pages[j]   = pages[j + 1];
              offsets[j] = offsets[j + 1];
            }
            count--;
            saveBookmarksForKey(key, pages, offsets, count);
            break;
          }
        }
        if (found) {
          sendBLEStatus("bookmark_deleted");
        } else {
          sendBLEStatus("bookmark_not_found");
        }
      } else { sendBLEStatus("invalid_json"); }
    } else { sendBLEStatus("invalid_json"); }
  }
  else if (command == "CANCEL_TRANSFER") {
    if (g_bleTransferState != BLE_TRANSFER_IDLE) {
      if (g_bleTransferFile) g_bleTransferFile.close();
      if (g_bleTransferState == BLE_TRANSFER_UPLOADING) {
        String tmpPath = g_bleTransferPath + ".tmp";
        if (FS.exists(tmpPath)) FS.remove(tmpPath);
      }
      g_bleTransferState     = BLE_TRANSFER_IDLE;
      g_bleTransferPath      = "";
      g_bleTransferOffset    = 0;
      g_bleTransferTotalSize = 0;
      g_bleTransferBytesSent = 0;
      sendBLEStatus("transfer_cancelled");
    } else {
      sendBLEStatus("no_active_transfer");
    }
  }
  else if (command.startsWith("DELETE:")) {
    String path = command.substring(7);
    Serial.print("[BLE] DELETE command for path: "); Serial.println(path);
    if (FS.exists(path)) {
      Serial.println("[BLE] File exists, attempting to remove");
      if (g_reader.currentBookPath == path && g_reader.file) {
        g_reader.file.close();
        g_reader.currentBookPath = "";
        g_reader.file            = File();
        g_reader.pageIndex       = 0;
        g_reader.knownPages      = 0;
        Serial.println("[BLE] Closed file that was open for reading");
        g_switchToLibrary = true;
        Serial.println("[BLE] Deferred switch to library mode");
      }
      if (FS.remove(path)) {
        if (path.startsWith("/books/")) {
          g_reloadLibrary = true;
          sendBLEStatus("file_delete_success");
          Serial.println("[BLE] File removed successfully, deferred library reload");
        } else if (path.startsWith("/apps/")) {
          sendBLEStatus("app_delete_success");
          scanApps();
          if (mode == MODE_APPS) drawAppsMenu();
          Serial.println("[BLE] File removed successfully, apps reloaded");
          sendBLEStatus("apps_reloaded");
        } else {
          Serial.println("[BLE] File removed successfully");
        }
      } else {
        Serial.println("[BLE] Failed to remove file");
        sendBLEStatus("delete_error:cannot_remove");
      }
    } else {
      Serial.println("[BLE] File not found");
      sendBLEStatus("delete_error:not_found");
    }
  }
  else {
    sendBLEStatus("unknown_command");
  }
}

// ============================================================================
//  BLE setup
// ============================================================================
void setupBLE() {
  Serial.println("[BLE] Initializing BLE...");
  BLEDevice::init("Pala One");
  BLEDevice::setMTU(517);
  Serial.println("[BLE] Device name set to 'Pala One'");

  g_bleServer = BLEDevice::createServer();
  g_bleServer->setCallbacks(new PalaBLEServerCallbacks());
  Serial.println("[BLE] Server created");

  BLEService* pService = g_bleServer->createService(PALA_BLE_SERVICE_UUID);

  g_bleCharCmd = pService->createCharacteristic(
    PALA_BLE_CHAR_CMD_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  g_bleCharCmd->setCallbacks(new BLECommandCallbacks());

  g_bleCharData = pService->createCharacteristic(
    PALA_BLE_CHAR_DATA_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_WRITE_NR |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  g_bleCharData->setCallbacks(new BLEDataCallbacks());
  g_bleCharData->addDescriptor(new BLE2902());

  g_bleCharStatus = pService->createCharacteristic(
    PALA_BLE_CHAR_STATUS_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  g_bleCharStatus->addDescriptor(new BLE2902());

  pService->start();
  Serial.println("[BLE] Service started");

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(PALA_BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMaxPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("[BLE] Advertising started");
}
