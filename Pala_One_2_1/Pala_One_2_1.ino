#include "src/hal/display.h"

DisplayType display;

#include "pala_one_sleep_black_icon_v4.h"

#include "src/state.h"
#include "src/hal/input.h"
#include "src/hal/battery.h"
#include "src/pure/text_util.h"
#include "src/pure/paths.h"
#include "src/pure/page_offset_table.h"
#include "src/storage/fs_util.h"
#include "src/storage/page_cache.h"
#include "src/storage/library.h"
#include "src/storage/progress.h"
#include "src/storage/app_catalog.h"
#include "src/reader/reader.h"
#include "src/ui/ui.h"
#include "src/ui/mode_handlers.h"
#include "src/settings/settings.h"
#include "src/apps/app_runner.h"
#include "src/webui/web_helpers.h"
#include "src/ble/ble_server.h"

const uint8_t* PAGE_FONT = u8g2_font_5x8_tf;
const uint8_t* MAIN_FONT = u8g2_font_helvR08_te;
const uint8_t* BOLD_FONT = u8g2_font_helvB08_te;

HeltecGFXAdapter gfx(display);
static void goToSleep() {
  if (!ENABLE_DEEP_SLEEP) return;

  if (g_bookmarkUi.previewActive) {
    int tmpPage = g_reader.pageIndex;
    g_reader.pageIndex = g_bookmarkUi.previewSavedPage;
    saveProgress(true);
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
    g_reader.pageIndex = tmpPage;
  } else if (mode == MODE_READER) {
    saveProgress(true);
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
  }

  delay(50);

  bool wasReading = (mode == MODE_READER || mode == MODE_BM_PREVIEW) && g_reader.currentBookPath.length() > 0;
  syncWakeState(wasReading);
  safeCloseCurrentBook();

  drawSleepScreen();
  delay(600);

  // Stop BLE advertising before sleep (btStop causes heap corruption)
  // Deep sleep automatically deinitializes all peripherals including BLE
  if (g_bleServer) {
    g_bleServer->getAdvertising()->stop();
  }

  Platform::prepareToSleep();
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN, 0);
  delay(50);
  esp_deep_sleep_start();
}

// ============================================================================
//  Setup
// ============================================================================
void setup() {
  Serial.begin(115200);
  delay(200);
  setCpuFrequencyMhz(240); // full speed for init; lowered to 80 MHz at end of setup

  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), btnISR, CHANGE);

  // Configure e-paper control pins as outputs to avoid GPIO warnings
  // (heltec-eink-modules library should do this, but we do it defensively)
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  u8g2.begin(gfx);
  initPalaAPI();
  invalidateMetrics();
  (void)getMetrics();
  resetOffsetCache();

#if HAS_BATTERY
  adcSetupOnce();
  pinMode(BAT_ADC_CTRL, INPUT);
  updateBatteryCached(true);
#endif

  display.fastmodeOff();
  display.clear();

  // Initialize BLE server
  setupBLE();

  if (!fsBegin()) {
    drawCenter("Storage error", "Try factory reset");
    return;
  }
  ensureBooksDir();
  if (!FS.exists("/apps")) FS.mkdir("/apps");

  prefs.begin("ereader", false);
  
  // Check if this is first launch with new byte offset system
  // Only delete .progress directory if firmware version changed
  String lastVersion = prefs.getString("fw_version", "");
  String currentVersion = FW_VERSION;
  
  if (lastVersion != currentVersion) {
    Serial.print("[Setup] Firmware version changed from ");
    Serial.print(lastVersion);
    Serial.print(" to ");
    Serial.println(currentVersion);
    
    // Delete old .progress directory to migrate to new byte offset approach
    if (FS.exists("/books/.progress")) {
      Serial.println("[Setup] Deleting .progress directory for migration");
      // Recursively delete .progress directory
      File progressDir = FS.open("/books/.progress");
      if (progressDir && progressDir.isDirectory()) {
        File f = progressDir.openNextFile();
        while (f) {
          String name = String(f.name());
          f.close();
          String path = "/books/.progress/" + name;
          FS.remove(path.c_str());
          f = progressDir.openNextFile();
        }
        progressDir.close();
      }
      FS.rmdir("/books/.progress");
    }
    
    // Update stored version
    prefs.putString("fw_version", currentVersion);
    Serial.println("[Setup] Firmware version updated");
  }
  
  // Create .progress directory if it doesn't exist
  if (!FS.exists("/books/.progress")) {
    FS.mkdir("/books/.progress");
  }

  loadSettings();
  loadBooks();
  markUserActivity();

  bool restored = false;
  if (prefs.getInt("wake_mode", 0) == 1) {
    String wp = prefs.getString("wake_path", "");
    if (wp.length() > 0) {
      for (int i = 0; i < g_library.bookCount; i++) {
        if (strcmp(g_library.books[i].path, wp.c_str()) == 0) {
          if (openBookByIndex(i)) {
            resetPreviewState();
            mode = MODE_READER;
            g_reader.pageTurnsSinceFull = FULL_REFRESH_EVERY_N_PAGES;
            renderCurrentPage();   // draw first — takes ~300ms, user releases button during this
            resetInputFrontend();  // then discard the wake-press only
            restored = true;
          }
          break;
        }
      }
    }
  }

  if (!restored) {
    drawLibrary();
    resetInputFrontend();
  }

  // Drop to 80 MHz for normal operation — saves significant power.
  // Upload mode will raise it back to 240 MHz temporarily.
  setCpuFrequencyMhz(80);
}

// ============================================================================
//  Main loop
// ============================================================================
void loop() {
  btns.poll();

  if (g_isrDropCount > BTN_QUEUE_RECOVER_THRESHOLD) {
    noInterrupts();
    g_isrDropCount = 0;
    interrupts();
    clearButtonQueue();
    btns.resetState();
  }

  if (btns.anyClick()) markUserActivity();

  // Handle deferred library reload (outside BLE callback context)
  if (g_reloadLibrary) {
    Serial.println("[BLE] Performing deferred library reload");
    loadBooks();
    // Redraw library screen if in library mode
    if (mode == MODE_LIBRARY) {
      drawLibrary();
    }
    g_reloadLibrary = false;
    // Notify iOS app that library is ready
    sendBLEStatus("library_ready");
    Serial.println("[BLE] Library ready");
  }

  // Handle deferred mode switch (outside BLE callback context)
  if (g_switchToLibrary) {
    Serial.println("[BLE] Performing deferred switch to library mode");
    mode = MODE_LIBRARY;
    g_switchToLibrary = false;
  }

  // Handle deferred redraw (outside BLE callback context)
  if (g_redrawCurrentMode) {
    Serial.println("[BLE] Redrawing current mode");
    invalidateMetrics();
    switch (mode) {
      case MODE_LIBRARY: drawLibrary(); break;
      case MODE_READER: renderCurrentPage(); break;
      case MODE_LIST: drawListScreen(); break;
      case MODE_ABOUT: drawAbout(); break;
      case MODE_APPS: drawAppsMenu(); break;
      default: break;
    }
    g_redrawCurrentMode = false;
  }

  // Handle deferred GET_ALL_BOOKMARKS (outside BLE callback context)
  if (g_fetchAllBookmarks) {
    Serial.println("[BLE] Fetching all bookmarks");
    loadBooks();
    Serial.print("[BLE] Library loaded: ");
    Serial.print(g_library.bookCount);
    Serial.println(" books");

    String json = "[";
    bool first = true;
    int totalBookmarks = 0;

    for (int b = 0; b < g_library.bookCount; b++) {
      String path = String(g_library.books[b].path);
      String key = prefKeyForBook(path);
      uint16_t pages[MAX_BOOKMARKS];
      uint32_t offsets[MAX_BOOKMARKS];
      uint8_t count = loadBookmarksForKey(key, pages, offsets);

      Serial.print("[BLE] Book: ");
      Serial.print(g_library.books[b].name);
      Serial.print(" - ");
      Serial.print(count);
      Serial.println(" bookmarks");

      if (count == 0) continue;

      // Open file to read bookmark labels
      File f;
      bool fileOpen = false;
      if (FS.exists(path)) {
        f = FS.open(path, "r");
        fileOpen = f;
      }

      for (uint8_t i = 0; i < count; i++) {
        if (!first) json += ",";
        first = false;
        totalBookmarks++;

        json += "{\"book\":\"" + String(g_library.books[b].name) + "\",\"path\":\"" + path + "\",\"page\":" + String(pages[i]) + ",\"offset\":" + String(offsets[i]);

        // Add label if file is open
        if (fileOpen) {
          uint32_t resolvedOffset = resolveBookmarkOffset(path, pages[i], offsets[i]);
          String label = readBookmarkLabelAtOffset(f, resolvedOffset, pages[i]);
          // Escape quotes and backslashes for JSON
          label.replace("\\", "\\\\");
          label.replace("\"", "\\\"");
          json += ",\"label\":\"" + label + "\"";
        } else {
          json += ",\"label\":\"Page " + String(pages[i] + 1) + "\"";
        }
        json += "}";
      }

      if (fileOpen) f.close();
    }

    json += "]";

    Serial.print("[BLE] Total bookmarks sent: ");
    Serial.println(totalBookmarks);
    Serial.print("[BLE] JSON length: ");
    Serial.println(json.length());

    // Send JSON in chunks sized to negotiated MTU
    const int maxChunkSize = 512;
    int jsonLen = json.length();
    int offset = 0;
    int chunkNum = 0;

    while (offset < jsonLen) {
      int chunkSize = min(maxChunkSize, jsonLen - offset);
      String chunk = json.substring(offset, offset + chunkSize);

      if (g_bleCharData && g_bleConnected) {
        g_bleCharData->setValue(chunk.c_str());
        g_bleCharData->notify();
      }

      offset += chunkSize;
      chunkNum++;
      Serial.print("[BLE] Sent chunk ");
      Serial.print(chunkNum);
      Serial.print(" (");
      Serial.print(chunkSize);
      Serial.println(" bytes)");
      delay(10);
    }

    delay(20);

    sendBLEStatus("all_bookmarks_sent");
    g_fetchAllBookmarks = false;
  }

  // Handle deferred upload initialization (outside BLE callback context)
  if (g_initUpload) {
    Serial.println("[BLE] Initializing upload in main loop");
    size_t freeBytes = fsFreeBytesSafe();
    Serial.print("[BLE] Free space: ");
    Serial.println(freeBytes);
    if (freeBytes < g_uploadSize + 8192) {
      Serial.println("[BLE] Not enough space");
      sendBLEStatus("upload_error:no_space");
      g_initUpload = false;
      g_uploadIsApp = false;
    } else {
      // Ensure apps directory exists for app uploads
      if (g_uploadIsApp && !FS.exists("/apps")) {
        FS.mkdir("/apps");
        Serial.println("[BLE] Created /apps directory");
      }
      
      String tmpPath = g_uploadPath + ".tmp";
      Serial.print("[BLE] Opening temp file: ");
      Serial.println(tmpPath);
      if (FS.exists(tmpPath)) FS.remove(tmpPath);
      g_bleTransferFile = FS.open(tmpPath, "w");

      if (g_bleTransferFile) {
        Serial.println("[BLE] File opened successfully");
        g_bleTransferPath = g_uploadPath;
        g_bleTransferOffset = 0;
        g_bleTransferTotalSize = g_uploadSize;
        g_bleTransferBytesSent = 0;
        g_bleRamBufHead = 0;
        g_bleRamBufTail = 0;
        g_bleAllReceived = false;
        g_finalizeUpload = false;
        g_bleTransferState = BLE_TRANSFER_UPLOADING;
        Serial.println("[BLE] Sending upload_ready status");
        sendBLEStatus("upload_ready");
      } else {
        Serial.println("[BLE] Failed to open file");
        sendBLEStatus("upload_error:cannot_open");
      }
      g_uploadPath = "";
      g_uploadSize = 0;
      g_initUpload = false;
    }
  }

  // Flush BLE RAM ring buffer to flash in 4 KB batches
  if (g_bleTransferState == BLE_TRANSFER_UPLOADING) {
    static const uint32_t FLUSH_THRESHOLD = 4096;
    uint32_t buffered = g_bleRamBufHead - g_bleRamBufTail;
    bool shouldFlush = (buffered >= FLUSH_THRESHOLD) ||
                       (g_bleAllReceived && buffered > 0);

    if (shouldFlush) {
      uint32_t toFlush = (buffered > FLUSH_THRESHOLD) ? FLUSH_THRESHOLD : buffered;
      uint32_t idx = g_bleRamBufTail & (BLE_RAM_BUF_SIZE - 1);

      if (idx + toFlush <= BLE_RAM_BUF_SIZE) {
        g_bleTransferFile.write(g_bleRamBuf + idx, toFlush);
      } else {
        uint32_t part1 = BLE_RAM_BUF_SIZE - idx;
        g_bleTransferFile.write(g_bleRamBuf + idx, part1);
        g_bleTransferFile.write(g_bleRamBuf,        toFlush - part1);
      }
      g_bleRamBufTail += toFlush;

      if (g_bleAllReceived && g_bleRamBufHead == g_bleRamBufTail) {
        Serial.println("[BLE Upload] All data flushed to flash, finalizing");
        g_bleTransferFile.close();
        g_bleTransferState = BLE_TRANSFER_IDLE;
        g_finalizeUpload = true;
      }
    }
  }

  // Post-transfer finalization (validation / normalization)
  if (g_finalizeUpload) {
    g_finalizeUpload = false;

    if (g_uploadIsApp) {
      String tmpPath = g_bleTransferPath + ".tmp";
      String finalPath = g_bleTransferPath;
      Serial.println("[BLE Upload] Validating app binary...");

      bool valid = false;
      File vf = FS.open(tmpPath, "r");
      if (vf) {
        if (vf.size() >= sizeof(PalaAppHeader)) {
          PalaAppHeader hdr;
          if (vf.read((uint8_t*)&hdr, sizeof(hdr)) == sizeof(hdr)) {
            valid = (hdr.magic == PALA_APP_MAGIC);
          }
        }
        vf.close();
      }

      if (valid) {
        if (FS.exists(finalPath)) FS.remove(finalPath);
        if (FS.rename(tmpPath, finalPath)) {
          Serial.println("[BLE Upload] App upload complete");
          sendBLEStatus("app_upload_complete");
          scanApps();
          if (mode == MODE_APPS) drawAppsMenu();
          sendBLEStatus("apps_reloaded");
        } else {
          if (FS.exists(tmpPath)) FS.remove(tmpPath);
          sendBLEStatus("upload_error:rename_failed");
        }
      } else {
        if (FS.exists(tmpPath)) FS.remove(tmpPath);
        sendBLEStatus("upload_error:invalid_app");
      }
      g_uploadIsApp = false;

    } else if (g_uploadIsBinary) {
      // Binary file upload - just rename temp file, no normalization
      String tmpPath = g_bleTransferPath + ".tmp";
      String finalPath = g_bleTransferPath;
      Serial.println("[BLE Upload] Finalizing binary file...");

      if (FS.rename(tmpPath, finalPath)) {
        Serial.println("[BLE Upload] Binary upload complete");
        sendBLEStatus("file_upload_complete");
      } else {
        FS.remove(tmpPath);
        sendBLEStatus("upload_error:rename_failed");
      }
      g_uploadIsBinary = false;

    } else {
      String tmpPath = g_bleTransferPath + ".tmp";
      String finalPath = g_bleTransferPath;
      Serial.println("[BLE Upload] Normalizing book file...");

      File tmpFile = FS.open(tmpPath, "r");
      File finalFile = FS.open(finalPath, "w");
      if (tmpFile && finalFile) {
        String pendingUtf8Tail = "";
        int chunkCount = 0;
        while (tmpFile.available()) {
          uint8_t buf[512];
          int bytesRead = tmpFile.read(buf, 512);
          if (bytesRead > 0) {
            String chunk = pendingUtf8Tail + String((const char*)buf, bytesRead);
            int len = (int)chunk.length();
            if (len > 4) {
              pendingUtf8Tail = chunk.substring(len - 4);
              chunk = chunk.substring(0, len - 4);
            } else {
              pendingUtf8Tail = chunk;
              chunk = "";
            }
            if (chunk.length() > 0) {
              String cleaned = normalizeTypography(chunk);
              cleaned = compactText(cleaned);
              finalFile.print(cleaned);
            }
          }
          if (++chunkCount % 10 == 0) yield();
        }
        if (pendingUtf8Tail.length() > 0) {
          String cleaned = normalizeTypography(pendingUtf8Tail);
          cleaned = compactText(cleaned);
          finalFile.print(cleaned);
        }
        tmpFile.close();
        finalFile.close();
        FS.remove(tmpPath);
        Serial.println("[BLE Upload] Book upload complete");
        sendBLEStatus("file_upload_complete");
        g_reloadLibrary = true;
      } else {
        if (tmpFile)  tmpFile.close();
        if (finalFile) finalFile.close();
        FS.remove(tmpPath);
        sendBLEStatus("upload_error:file_open");
      }
    }
  }

  // Keep device awake while BLE connected to companion
  if (g_bleConnected) {
    lastUserActionMs = millis();
  }

  if (ENABLE_DEEP_SLEEP) {
    static uint32_t lastSleepLogMs = 0;
    uint32_t elapsed = (uint32_t)(millis() - lastUserActionMs);
    uint32_t sleepAfter = sleepAfterMs();
    int32_t remaining = (int32_t)sleepAfter - (int32_t)elapsed;

    // Log remaining sleep time every 10 seconds
    if ((uint32_t)(millis() - lastSleepLogMs) > 10000) {
      Serial.print("[Sleep] Time to sleep: ");
      Serial.print(remaining / 1000);
      Serial.println(" seconds");
      lastSleepLogMs = millis();
    }

    if (elapsed > sleepAfter) {
      goToSleep();
      return;
    }
  }

  // Global triple-click = go home to library root.
  // Bookmark screens handle triple-click themselves for correct back-navigation,
  // so exclude them here.
  if (btns.tripleClick
      && mode != MODE_BM_PREVIEW
      && mode != MODE_BM_LIST
      && mode != MODE_BM_BOOK_SELECT) {
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  switch (mode) {
    case MODE_ABOUT:          handleModeAbout(); break;
    case MODE_LIST:           handleModeList(); break;
    case MODE_APPS:           handleModeApps(); break;
    case MODE_BM_BOOK_SELECT: handleModeBookmarkBookSelect(); break;
    case MODE_BM_LIST:        handleModeBookmarkList(); break;
    case MODE_BM_PREVIEW:     handleModeBookmarkPreview(); break;
    case MODE_LIBRARY:        handleModeLibrary(); break;
    case MODE_READER:         handleModeReader(); break;
  }
}
