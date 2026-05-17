#include <heltec-eink-modules.h>

// ── Board selection: uncomment the line that matches your hardware ────────────
//#define BOARD_V1_1
#define BOARD_V1_2
// ─────────────────────────────────────────────────────────────────────────────

#include "pala_app.h"
#include "pala_api.h"
#include <stdarg.h>
#ifdef BOARD_V1_1
  using DisplayType = EInkDisplay_WirelessPaperV1_1;
#else
  using DisplayType = EInkDisplay_WirelessPaperV1_2;
#endif

DisplayType display;

#include "pala_one_sleep_black_icon_v4.h"

// Define U8G2_FONT_SECTION as empty for Arduino compatibility
#ifndef U8G2_FONT_SECTION
#define U8G2_FONT_SECTION(name)
#endif

// Custom fonts
#include "fonts/atkinson_regular_12.c"
#include "fonts/atkinson_regular_14.c"
#include "fonts/atkinson_regular_16.c"
#include "fonts/atkinson_regular_18.c"
#include "fonts/atkinson_regular_20.c"
#include "fonts/atkinson_medium_12.c"
#include "fonts/atkinson_medium_14.c"
#include "fonts/atkinson_medium_16.c"
#include "fonts/atkinson_medium_18.c"
#include "fonts/atkinson_medium_20.c"
#include "fonts/atkinson_semibold_12.c"
#include "fonts/atkinson_semibold_14.c"
#include "fonts/atkinson_semibold_16.c"
#include "fonts/atkinson_semibold_18.c"
#include "fonts/atkinson_semibold_20.c"
#include "fonts/atkinson_bold_12.c"
#include "fonts/atkinson_bold_14.c"
#include "fonts/atkinson_bold_16.c"
#include "fonts/atkinson_bold_18.c"
#include "fonts/atkinson_bold_20.c"
#include "fonts/atkinson_extrabold_12.c"
#include "fonts/atkinson_extrabold_14.c"
#include "fonts/atkinson_extrabold_16.c"
#include "fonts/atkinson_extrabold_18.c"
#include "fonts/atkinson_extrabold_20.c"
#include "fonts/noto_sans_regular_12.c"
#include "fonts/noto_sans_regular_14.c"
#include "fonts/noto_sans_regular_16.c"
#include "fonts/noto_sans_regular_18.c"
#include "fonts/noto_sans_regular_20.c"
#include "fonts/noto_sans_medium_12.c"
#include "fonts/noto_sans_medium_14.c"
#include "fonts/noto_sans_medium_16.c"
#include "fonts/noto_sans_medium_18.c"
#include "fonts/noto_sans_medium_20.c"
#include "fonts/noto_sans_semibold_12.c"
#include "fonts/noto_sans_semibold_14.c"
#include "fonts/noto_sans_semibold_16.c"
#include "fonts/noto_sans_semibold_18.c"
#include "fonts/noto_sans_semibold_20.c"
#include "fonts/noto_sans_bold_12.c"
#include "fonts/noto_sans_bold_14.c"
#include "fonts/noto_sans_bold_16.c"
#include "fonts/noto_sans_bold_18.c"
#include "fonts/noto_sans_bold_20.c"
#include "fonts/noto_sans_extrabold_12.c"
#include "fonts/noto_sans_extrabold_14.c"
#include "fonts/noto_sans_extrabold_16.c"
#include "fonts/noto_sans_extrabold_18.c"
#include "fonts/noto_sans_extrabold_20.c"
#include "fonts/noto_serif_regular_12.c"
#include "fonts/noto_serif_regular_14.c"
#include "fonts/noto_serif_regular_16.c"
#include "fonts/noto_serif_regular_18.c"
#include "fonts/noto_serif_regular_20.c"
#include "fonts/noto_serif_medium_12.c"
#include "fonts/noto_serif_medium_14.c"
#include "fonts/noto_serif_medium_16.c"
#include "fonts/noto_serif_medium_18.c"
#include "fonts/noto_serif_medium_20.c"
#include "fonts/noto_serif_semibold_12.c"
#include "fonts/noto_serif_semibold_14.c"
#include "fonts/noto_serif_semibold_16.c"
#include "fonts/noto_serif_semibold_18.c"
#include "fonts/noto_serif_semibold_20.c"
#include "fonts/noto_serif_bold_12.c"
#include "fonts/noto_serif_bold_14.c"
#include "fonts/noto_serif_bold_16.c"
#include "fonts/noto_serif_bold_18.c"
#include "fonts/noto_serif_bold_20.c"
#include "fonts/noto_serif_extrabold_12.c"
#include "fonts/noto_serif_extrabold_14.c"
#include "fonts/noto_serif_extrabold_16.c"
#include "fonts/noto_serif_extrabold_18.c"
#include "fonts/noto_serif_extrabold_20.c"

// Literata
#include "fonts/literata_regular_12.c"
#include "fonts/literata_regular_14.c"
#include "fonts/literata_regular_16.c"
#include "fonts/literata_regular_18.c"
#include "fonts/literata_regular_20.c"
#include "fonts/literata_medium_12.c"
#include "fonts/literata_medium_14.c"
#include "fonts/literata_medium_16.c"
#include "fonts/literata_medium_18.c"
#include "fonts/literata_medium_20.c"
#include "fonts/literata_semibold_12.c"
#include "fonts/literata_semibold_14.c"
#include "fonts/literata_semibold_16.c"
#include "fonts/literata_semibold_18.c"
#include "fonts/literata_semibold_20.c"
#include "fonts/literata_bold_12.c"
#include "fonts/literata_bold_14.c"
#include "fonts/literata_bold_16.c"
#include "fonts/literata_bold_18.c"
#include "fonts/literata_bold_20.c"
#include "fonts/literata_extrabold_12.c"
#include "fonts/literata_extrabold_14.c"
#include "fonts/literata_extrabold_16.c"
#include "fonts/literata_extrabold_18.c"
#include "fonts/literata_extrabold_20.c"

#include <Preferences.h>

#include <LittleFS.h>
#define FS LittleFS

#include <Adafruit_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>
U8G2_FOR_ADAFRUIT_GFX u8g2;

#include <esp_timer.h>
#include <esp_rtc_time.h>
#include <esp_bt.h>
#include <esp_sleep.h>
#include <esp_heap_caps.h>
#include <soc/soc.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ============================================================================
//  Firmware / Product constants
// ============================================================================
#define FW_VERSION "2.1"

static const int SCREEN_W = 250;
static const int SCREEN_H = 122;

static const uint8_t MAX_BOOKMARKS = 12;
static const int MAX_BOOKS = 80;
static const int MAX_FOLDERS = 32;
static const int MAX_PAGES = 10000;
static const int MAX_LIBRARY_ENTRIES = (MAX_BOOKS * 2) + (MAX_FOLDERS * 2) + 8;
static const int OFFSET_CACHE_SIZE = 96;
static const int MAX_LIST_ITEMS = 16;
static const int MAX_LIST_TEXT = 64;

static const int PREFETCH_AHEAD_PAGES = 1;
static const int READER_IDLE_PREFETCH_PAGES = 1;

static const uint32_t DOUBLE_MS = 300;
static const uint32_t TRIPLE_MS = 550;
static const uint32_t LONG_MS = 850;
static const uint32_t DEBOUNCE_MS = 14;

static const uint32_t SAVE_EVERY_MS = 7000;
static const uint32_t TOAST_MS = 650;
static const uint32_t UPLOAD_AUTO_EXIT_MS = 15UL * 60UL * 1000UL;
static const uint32_t BAT_CACHE_MS = 180000; // 3 min — battery changes slowly

static const int FULL_REFRESH_EVERY_N_PAGES = 100;

// ============================================================================
//  BLE Service / Characteristic UUIDs
// ============================================================================
#define PALA_BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define PALA_BLE_CHAR_CMD_UUID       "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define PALA_BLE_CHAR_DATA_UUID      "beb5483f-36e1-4688-b7f5-ea07361b26a8"
#define PALA_BLE_CHAR_STATUS_UUID    "beb54840-36e1-4688-b7f5-ea07361b26a8"
static const int MENU_FULL_REFRESH_EVERY = 60;

static const int MARGIN_X = 6;
static const int TOP_PAD = 0;
static const int BOT_PAD = 0;
static const int STATUS_H = 8;

static const bool SHOW_PROGRESS_BAR = true;
static const bool SHOW_PAGE_NUMBER = true;
static const bool ENABLE_DEEP_SLEEP = true;

static const uint8_t* PAGE_FONT = u8g2_font_5x8_tf;
const uint8_t* MAIN_FONT = u8g2_font_helvR08_te;
const uint8_t* BOLD_FONT = u8g2_font_helvB08_te;

#define BTN 0
#define HAS_BATTERY 1
#if HAS_BATTERY
  #define BAT_ADC_CTRL 19
  #define BAT_ADC_IN   20
#endif

// ============================================================================
//  Types
// ============================================================================
enum Mode {
  MODE_LIBRARY,
  MODE_READER,
  MODE_ABOUT,
  MODE_LIST,
  MODE_BM_BOOK_SELECT,
  MODE_BM_LIST,
  MODE_BM_PREVIEW,
  MODE_APPS,
};

enum ReaderLongPressAction {
  LONGPRESS_BOOKMARK = 0
};

enum LibraryEntryType {
  LIB_ENTRY_BACK,
  LIB_ENTRY_FOLDER,
  LIB_ENTRY_BOOK,
  LIB_ENTRY_BOOKMARKS,
  LIB_ENTRY_LIST,
  LIB_ENTRY_ABOUT,
  LIB_ENTRY_APPS,
};

struct BookInfo {
  char name[80];
  char path[96];
  size_t size;
  char folder[64];
};

struct LayoutMetrics {
  int ascent;
  int descent;
  int lineH;
  int maxWidth;
  int maxLines;
};

struct RuntimeSettings {
  int fontSize = 12;
  uint32_t sleepSecs = 120;
  int lineGap = 0;
  int readerLongPressAction = LONGPRESS_BOOKMARK;
  int fontFamily = 0;  // 0=atkinson, 1=literata, 2=noto_sans, 3=noto_serif
  int fontWeight = 0;  // 0=regular, 1=medium, 2=semibold, 3=bold, 4=extrabold
};

struct LibraryState {
  BookInfo books[MAX_BOOKS];
  int bookCount = 0;

  char folders[MAX_FOLDERS][64];
  int folderCount = 0;

  int selectedItem = 0;
  String currentFolder;

  LibraryEntryType entryTypes[MAX_LIBRARY_ENTRIES];
  int entryRefs[MAX_LIBRARY_ENTRIES];
  int entryDepths[MAX_LIBRARY_ENTRIES];
  int entryCount = 0;

  bool folderExpanded[MAX_FOLDERS] = {false};
};

struct ReaderState {
  File file;
  String currentBookKey;
  String currentBookPath;
  int pageIndex = 0;

  uint32_t pageOffsets[MAX_PAGES];
  int knownPages = 0;
  bool eofReached = false;

  uint32_t lastPageStartOffset = 0;
  int pageTurnsSinceFull = 0;

  uint32_t lastSaveMs = 0;
  int lastSavedPage = -1;
};

struct BookmarkUiState {
  int bookIndex = 0;
  int selectedIndex = 0;
  uint16_t pages[MAX_BOOKMARKS];
  uint32_t offsets[MAX_BOOKMARKS];
  uint8_t count = 0;

  bool previewActive = false;
  int previewSavedPage = 0;
};

struct ToastState {
  String msg;
  uint32_t untilMs = 0;
};

struct ListItem {
  char text[MAX_LIST_TEXT + 1];
  uint8_t done = 0;
};

struct ListState {
  ListItem items[MAX_LIST_ITEMS];
  int count = 0;
  int selectedIndex = 0;
};

#define MAX_APPS     16
#define MAX_APP_NAME 32
#define MAX_APP_PATH 80

struct AppDiscovery {
  char name[MAX_APP_NAME + 1];
  char path[MAX_APP_PATH + 1];
};

struct AppsState {
  AppDiscovery apps[MAX_APPS];
  int count         = 0;
  int selectedIndex = 0;
};

struct UploadState {
  File bookTmpFile;
  File sleepTmpFile;
  File appTmpFile;

  String bookTmpPath;
  String bookPendingUtf8Tail;
  String bookFinalName;
  bool bookOk = false;
  String bookError;

  String sleepTmpPath;
  bool sleepOk = false;
  String sleepError;

  String appTmpPath;
  String appFinalName;
  bool appOk = false;
  String appError;

  uint32_t startedMs = 0;
};

struct BatteryState {
  float rawV = 0.0f;
  float filteredV = 0.0f;
  int pctRaw = 0;
  int pctShown = 0;
  bool valid = false;
  bool low = false;
  uint32_t lastMs = 0;
  float calibrationFactor = 1.00f;
};

struct ButtonState {
  bool stablePressed = false;
  uint32_t lastStableChange = 0;
  uint32_t pressStart = 0;
  bool pressArmed = false;
  uint32_t lastRelease = 0;
  uint32_t firstClickRelease = 0;
  uint8_t clickCount = 0;

  bool shortClick = false;
  bool doubleClick = false;
  bool tripleClick = false;
  bool quadClick = false;
  bool longClick = false;

  uint32_t rawPressCount = 0; // every short press-release, unfiltered by multi-click windows

  void resetClicks() {
    shortClick = false;
    doubleClick = false;
    tripleClick = false;
    quadClick = false;
    longClick = false;
  }

  void resetState() {
    stablePressed = false;
    lastStableChange = 0;
    pressStart = 0;
    pressArmed = false;
    lastRelease = 0;
    firstClickRelease = 0;
    clickCount = 0;
    rawPressCount = 0;
    resetClicks();
  }

  bool anyClick() const {
    return shortClick || doubleClick || tripleClick || quadClick || longClick;
  }

  void poll();
};

struct OffsetCacheEntry {
  uint32_t pathHash = 0;
  int page = -1;
  uint32_t offset = 0;
  uint32_t stamp = 0;
};

// ============================================================================
//  Globals
// ============================================================================
Mode mode = MODE_LIBRARY;
Preferences prefs;

RuntimeSettings g_settings;
LibraryState g_library;
ReaderState g_reader;
BookmarkUiState g_bookmarkUi;
ToastState g_toast;
ListState g_list;
UploadState g_upload;
BatteryState g_battery;
ButtonState btns;
OffsetCacheEntry g_offsetCache[OFFSET_CACHE_SIZE];
uint32_t g_offsetCacheStamp = 1;

uint32_t lastUserActionMs = 0;
int menuDrawsSinceFull = 0;
static AppsState g_apps;
static PalaAPI   g_palaAPI;
static void*     g_appExecBuf  = nullptr;
static size_t    g_appExecSize = 0;
LayoutMetrics g_metrics;
bool g_metricsValid = false;

// BLE globals
static BLEServer* g_bleServer = nullptr;
static BLECharacteristic* g_bleCharCmd = nullptr;
static BLECharacteristic* g_bleCharData = nullptr;
static BLECharacteristic* g_bleCharStatus = nullptr;
static bool g_bleConnected = false;

// Deferred library reload flag
static bool g_reloadLibrary = false;

// Deferred mode switch flag
static bool g_switchToLibrary = false;

// Deferred redraw flag for settings refresh
static bool g_redrawCurrentMode = false;
static bool g_fetchAllBookmarks = false;

// Deferred upload initialization flags
static bool g_initUpload = false;
static bool g_uploadIsApp = false;
static String g_uploadPath = "";
static uint32_t g_uploadSize = 0;

// BLE file transfer state
enum BLETransferState {
  BLE_TRANSFER_IDLE,
  BLE_TRANSFER_UPLOADING,
  BLE_TRANSFER_DOWNLOADING
};
static BLETransferState g_bleTransferState = BLE_TRANSFER_IDLE;
static File g_bleTransferFile;
static String g_bleTransferPath;
static uint32_t g_bleTransferOffset = 0;
static uint32_t g_bleTransferTotalSize = 0;
static uint32_t g_bleTransferBytesSent = 0;
static const uint32_t BLE_CHUNK_SIZE = 512; // Safe chunk size for BLE

static const uint8_t BTN_Q = 64;
static const uint32_t BTN_QUEUE_RECOVER_THRESHOLD = 10;
volatile uint8_t btnQHead = 0;
volatile uint8_t btnQTail = 0;
volatile bool btnQState[BTN_Q];
volatile uint32_t btnQTimeMs[BTN_Q];
volatile uint32_t g_isrDropCount = 0;

// ============================================================================
//  Display adapter
// ============================================================================
class HeltecGFXAdapter : public Adafruit_GFX {
public:
  explicit HeltecGFXAdapter(DisplayType& d)
    : Adafruit_GFX(SCREEN_W, SCREEN_H), disp(d) {}

  void drawPixel(int16_t x, int16_t y, uint16_t color) override {
    if (x < 0 || y < 0 || x >= SCREEN_W || y >= SCREEN_H) return;
    uint16_t c = color ? BLACK : WHITE;
    int16_t xx = (SCREEN_W - 1) - x;
    int16_t yy = (SCREEN_H - 1) - y;
    disp.drawPixel(xx, yy, c);
  }

private:
  DisplayType& disp;
};
HeltecGFXAdapter gfx(display);

// ============================================================================
//  Forward declarations
// ============================================================================
static void invalidateMetrics();
static const LayoutMetrics& getMetrics();
static void applyFontSize(int sz);
static void loadSettings();
static void markUserActivity();
static void clearButtonQueue();
static void resetInputFrontend();

static void setupBLE();
static void handleBLECommand(std::string cmd);
static void sendBLEStatus(const char* msg);

static bool fsBegin();
static void ensureBooksDir();
static void safeCloseCurrentBook();
static void clearCurrentBookState();
static bool reopenCurrentBookIfNeeded();
static void resetPreviewState();
static void resetUiEphemeralState();
static void resetNavigationState();
static void syncWakeState(bool reading);
static void enterLibraryRoot(bool redraw = true);

static uint32_t hashPath32(const String& path);
static void resetOffsetCache();
static bool lookupOffsetCache(const String& path, int targetPage, int& cachedPage, uint32_t& cachedOffset);
static void storeOffsetCache(const String& path, int page, uint32_t offset);
static void idlePrefetchReader();
static String pageCachePathForBook(const String& path);
static bool loadPageOffsetCacheForBook(const String& path, size_t expectedSize);
static void savePageOffsetCacheForBook(const String& path, size_t fileSize);
static void invalidateAllPageCaches();
static uint32_t resolveBookmarkOffset(const String& path, uint16_t page, uint32_t storedOffset);
static String readPageTextForWeb(const String& path, int page);



static bool fsBegin() {
  // First try to mount without formatting — protects existing data.
  if (FS.begin(false)) return true;

  // Mount failed. This happens on a brand-new device where the LittleFS
  // partition has never been formatted. Format once, then mount.
  // If the partition already had data but is now corrupt, this wipes it —
  // which is the correct recovery action (same as factory reset).
  Serial.println("[FS] Mount failed — formatting LittleFS...");
  if (!FS.format()) {
    Serial.println("[FS] Format failed.");
    return false;
  }
  Serial.println("[FS] Format OK, mounting...");
  return FS.begin(false);
}

static size_t fsTotalBytesSafe() {
  return FS.totalBytes();
}

static size_t fsUsedBytesSafe() {
  return FS.usedBytes();
}

static size_t fsFreeBytesSafe() {
  size_t total = fsTotalBytesSafe();
  size_t used = fsUsedBytesSafe();
  return (total >= used) ? (total - used) : 0;
}

static void invalidateMetrics() {
  g_metricsValid = false;
}

// ============================================================================
//  BLE Server Setup
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
    Serial.print("[BLE Data] Write callback, transfer state: ");
    Serial.println(g_bleTransferState);

    if (g_bleTransferState == BLE_TRANSFER_UPLOADING && g_bleTransferFile) {
      // Reset sleep timer to prevent sleep during upload
      lastUserActionMs = millis();

      String arduinoData = pCharacteristic->getValue();
      std::string data = std::string(arduinoData.c_str(), arduinoData.length());
      Serial.print("[BLE Data] Received chunk size: ");
      Serial.println(data.length());

      if (data.length() > 0) {
        g_bleTransferFile.write((const uint8_t*)data.data(), data.length());
        g_bleTransferOffset += data.length();

        Serial.print("[BLE Upload] Received chunk: ");
        Serial.print(g_bleTransferOffset);
        Serial.print("/");
        Serial.print(g_bleTransferTotalSize);
        Serial.println(" bytes");

        // Send ACK with offset
        String ack = "ACK:" + String(g_bleTransferOffset);
        Serial.print("[BLE Upload] Sending ACK: ");
        Serial.println(ack);
        sendBLEStatus(ack.c_str());

        // Yield to watchdog
        yield();

        // Check if transfer complete
        if (g_bleTransferOffset >= g_bleTransferTotalSize) {
          Serial.println("[BLE Upload] Transfer complete");
          g_bleTransferFile.close();
          g_bleTransferState = BLE_TRANSFER_IDLE;

          if (g_uploadIsApp) {
            // App upload - validate and finalize binary file
            Serial.println("[BLE Upload] Validating app binary...");
            String tmpPath = g_bleTransferPath + ".tmp";
            String finalPath = g_bleTransferPath;

            // Validate magic number and size
            bool valid = false;
            File vf = FS.open(tmpPath, "r");
            if (vf) {
              if (vf.size() >= sizeof(PalaAppHeader)) {
                PalaAppHeader hdr;
                if (vf.read((uint8_t*)&hdr, sizeof(hdr)) == sizeof(hdr)) {
                  if (hdr.magic == PALA_APP_MAGIC) {
                    valid = true;
                    Serial.println("[BLE Upload] App magic number valid");
                  } else {
                    Serial.println("[BLE Upload] Invalid app magic number");
                  }
                }
              }
              vf.close();
            }

            if (valid) {
              if (FS.exists(finalPath)) FS.remove(finalPath);
              if (FS.rename(tmpPath, finalPath)) {
                Serial.println("[BLE Upload] App upload complete");
                sendBLEStatus("app_upload_complete");
                scanApps(); // Reload device's internal app list
                if (mode == MODE_APPS) drawAppsMenu(); // Refresh display if in apps mode
                sendBLEStatus("apps_reloaded");
                g_uploadIsApp = false;
              } else {
                Serial.println("[BLE Upload] Failed to finalize app upload");
                if (FS.exists(tmpPath)) FS.remove(tmpPath);
                sendBLEStatus("upload_error:rename_failed");
                g_uploadIsApp = false;
              }
            } else {
              Serial.println("[BLE Upload] App validation failed");
              if (FS.exists(tmpPath)) FS.remove(tmpPath);
              sendBLEStatus("upload_error:invalid_app");
              g_uploadIsApp = false;
            }
          } else {
            // Book upload - apply normalization like HTTP upload
            Serial.println("[BLE Upload] Normalizing file...");
            String tmpPath = g_bleTransferPath + ".tmp";
            String finalPath = g_bleTransferPath;

            // Read, normalize, and rewrite file
            Serial.print("[BLE Upload] Normalizing file: ");
            Serial.println(tmpPath.c_str());
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
                // Yield to watchdog every few chunks
                if (++chunkCount % 10 == 0) {
                  yield();
                }
              }
              if (pendingUtf8Tail.length() > 0) {
                String cleaned = normalizeTypography(pendingUtf8Tail);
                cleaned = compactText(cleaned);
                finalFile.print(cleaned);
              }
              tmpFile.close();
              finalFile.close();
              Serial.print("[BLE Upload] Removing temp file: ");
              Serial.println(tmpPath.c_str());
              if (FS.remove(tmpPath)) {
                Serial.println("[BLE Upload] Temp file removed successfully");
              } else {
                Serial.println("[BLE Upload] Failed to remove temp file");
              }
              Serial.println("[BLE Upload] Upload complete");
              sendBLEStatus("file_upload_complete");
              // Defer library reload to avoid crash in BLE callback
              g_reloadLibrary = true;
            } else {
              Serial.println("[BLE Upload] Failed to open files for normalization");
              if (tmpFile) tmpFile.close();
              if (finalFile) finalFile.close();
              FS.remove(tmpPath);
              sendBLEStatus("upload_error:file_open");
            }
          }
        }
      }
    }
  }
};

static void sendBLEStatus(const char* msg) {
  if (g_bleCharStatus && g_bleConnected) {
    g_bleCharStatus->setValue(msg);
    g_bleCharStatus->notify();
  }
}

static void handleBLECommand(std::string cmd) {
  // Parse command and route to appropriate handler
  // Commands: "LIST_FILES", "UPLOAD:<path>", "DOWNLOAD:<path>", "GET_SETTINGS", "SET_SETTINGS:...", etc.
  Serial.print("[BLE] Command: ");
  Serial.println(cmd.c_str());

  String command(cmd.c_str());
  command.trim();

  if (command == "LIST_FILES") {
    // Return JSON list of books
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
    // Return JSON list of apps
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
    // Parse: UPLOAD:/books/filename.txt:12345 (path:size)
    int colonIdx = params.indexOf(':', 1); // Skip the first colon after UPLOAD:
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      uint32_t size = params.substring(colonIdx + 1).toInt();
      Serial.print("[BLE] Path: ");
      Serial.println(path);
      Serial.print("[BLE] Size: ");
      Serial.println(size);

      if (size > 0 && size < 10 * 1024 * 1024) { // Max 10MB
        // Defer file open to main loop to avoid BLE stack overflow
        g_uploadPath = path;
        g_uploadSize = size;
        g_uploadIsApp = false; // Ensure this is a book upload
        g_initUpload = true;
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
    // Parse: UPLOAD_APP:/apps/appname.bin:12345 (path:size)
    int colonIdx = params.indexOf(':', 1); // Skip the first colon after UPLOAD_APP:
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      uint32_t size = params.substring(colonIdx + 1).toInt();
      Serial.print("[BLE] Path: ");
      Serial.println(path);
      Serial.print("[BLE] Size: ");
      Serial.println(size);

      const size_t MAX_APP_BINARY = 48 * 1024; // 48KB max
      if (size > 0 && size <= MAX_APP_BINARY) {
        // Defer file open to main loop to avoid BLE stack overflow
        g_uploadPath = path;
        g_uploadSize = size;
        g_uploadIsApp = true;
        g_initUpload = true;
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
        g_bleTransferPath = path;
        g_bleTransferOffset = 0;
        g_bleTransferTotalSize = g_bleTransferFile.size();
        g_bleTransferBytesSent = 0;
        g_bleTransferState = BLE_TRANSFER_DOWNLOADING;

        // Send file size to client
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
    // Send next chunk of file
    if (g_bleTransferState == BLE_TRANSFER_DOWNLOADING && g_bleTransferFile) {
      uint8_t chunk[BLE_CHUNK_SIZE];
      g_bleTransferFile.seek(g_bleTransferOffset);
      int bytesRead = g_bleTransferFile.read(chunk, BLE_CHUNK_SIZE);

      if (bytesRead > 0) {
        g_bleCharData->setValue(chunk, bytesRead);
        g_bleCharData->notify();
        g_bleTransferOffset += bytesRead;

        // Check if complete
        if (g_bleTransferOffset >= g_bleTransferTotalSize) {
          g_bleTransferFile.close();
          g_bleTransferState = BLE_TRANSFER_IDLE;
          sendBLEStatus("download_complete");
        } else {
          // Send offset as status
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
    // Return current settings as JSON
    String json = "{";
    json += "\"fontSize\":" + String(g_settings.fontSize) + ",";
    json += "\"fontFamily\":" + String(g_settings.fontFamily) + ",";
    json += "\"fontWeight\":" + String(g_settings.fontWeight) + ",";
    json += "\"sleepSecs\":" + String(g_settings.sleepSecs) + ",";
    json += "\"lineGap\":" + String(g_settings.lineGap) + ",";
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
    // Simple JSON parsing: {"fontSize":10,"fontFamily":0,"sleepSecs":120,"lineGap":0}
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
            applyFontSize(g_settings.fontSize); // Re-apply font size with new family
            layoutChanged = true;
            Serial.print("[BLE] Changed fontFamily to ");
            Serial.println(ff);
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
            applyFontSize(g_settings.fontSize); // Re-apply font size with new weight
            layoutChanged = true;
            Serial.print("[BLE] Changed fontWeight to ");
            Serial.println(fw);
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
            Serial.print("[BLE] Attempt started: changing fontSize to ");
            Serial.println(fs);
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

    // Layout changed - page offsets will be invalidated
    if (layoutChanged && wasReading) {
      Serial.println("[BLE] Layout changed, invalidating page caches");
      invalidateAllPageCaches();
    }

    if (layoutChanged) {
      invalidateAllPageCaches();
      Serial.print("[BLE] Attempt succeeded: fontSize updated to ");
      Serial.println(g_settings.fontSize);
    }
    sendBLEStatus("settings_updated");

    // Defer redraw to main loop to handle all modes
    g_redrawCurrentMode = true;
  }
  else if (command.startsWith("GET_ALL_BOOKMARKS")) {
    // Defer heavy processing to main loop to avoid BLE stack overflow
    g_fetchAllBookmarks = true;
  }
  else if (command.startsWith("GET_BOOKMARKS:")) {
    String path = command.substring(14);
    String key = prefKeyForBook(path);
    uint16_t pages[MAX_BOOKMARKS];
    uint32_t offsets[MAX_BOOKMARKS];
    uint8_t count = loadBookmarksForKey(key, pages, offsets);

    // Open file to read bookmark labels
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

      // Add label if file is open
      if (fileOpen) {
        uint32_t resolvedOffset = resolveBookmarkOffset(path, pages[i], offsets[i]);
        String label = readBookmarkLabelAtOffset(f, resolvedOffset, pages[i]);
        // Escape quotes and backslashes for JSON
        label.replace("\\", "\\\\");
        label.replace("\"", "\\\"");
        Serial.print("[BLE] Bookmark label: ");
        Serial.println(label);
        json += ",\"label\":\"" + label + "\"";
      } else {
        json += ",\"label\":\"Page " + String(pages[i] + 1) + "\"";
      }
      json += "}";
    }
    json += "]}";
    Serial.print("[BLE] Bookmark JSON: ");
    Serial.println(json);

    if (fileOpen) f.close();

    if (g_bleCharData && g_bleConnected) {
      g_bleCharData->setValue(json.c_str());
      g_bleCharData->notify();
    }
    sendBLEStatus("bookmarks_sent");
  }
  else if (command.startsWith("VIEW_BOOKMARK:")) {
    String params = command.substring(14);
    // Parse: VIEW_BOOKMARK:/books/file.txt:123 (path:page)
    int colonIdx = params.indexOf(':', 1);
    if (colonIdx > 0) {
      String path = params.substring(0, colonIdx);
      int page = params.substring(colonIdx + 1).toInt();

      // Load bookmarks to get offset
      String key = prefKeyForBook(path);
      uint16_t pages[MAX_BOOKMARKS];
      uint32_t offsets[MAX_BOOKMARKS];
      uint8_t count = loadBookmarksForKey(key, pages, offsets);

      // Find the bookmark with matching page
      uint32_t offset = 0xFFFFFFFF;
      for (uint8_t i = 0; i < count; i++) {
        if ((int)pages[i] == page) {
          offset = offsets[i];
          break;
        }
      }

      if (offset == 0xFFFFFFFF) {
        sendBLEStatus("bookmark_not_found");
      } else {
        // Open file and read page
        File f = FS.open(path, "r");
        if (f) {
          uint32_t resolvedOffset = resolveBookmarkOffset(path, (uint16_t)page, offset);
          String txt;
          txt.reserve(900);
          readPageFromFile(f, resolvedOffset, false, &txt);
          f.close();
          txt.trim();
          if (txt.length() == 0) txt = "(empty)";

          // Send page text as JSON
          String json = "{\"path\":\"" + path + "\",\"page\":" + String(page) + ",\"text\":\"";
          // Escape quotes, backslashes, and newlines for JSON
          txt.replace("\\", "\\\\");
          txt.replace("\"", "\\\"");
          txt.replace("\n", "\\n");
          txt.replace("\r", "\\r");
          txt.replace("\t", "\\t");
          json += txt + "\"}";
          Serial.print("[BLE] Bookmark view JSON: ");
          Serial.println(json);

          // Send JSON in chunks to handle BLE MTU limitations
          int jsonLen = json.length();
          int chunkSize = 20; // BLE MTU for notifications is typically 20-23 bytes
          for (int offset = 0; offset < jsonLen; offset += chunkSize) {
            int endIdx = min(offset + chunkSize, jsonLen);
            String chunk = json.substring(offset, endIdx);
            if (g_bleCharData && g_bleConnected) {
              g_bleCharData->setValue(chunk.c_str());
              g_bleCharData->notify();
            }
            delay(20); // Small delay between chunks
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
    // Parse JSON: {"path":"/books/...","page":42}
    int pathIdx = bookmarkJson.indexOf("\"path\":\"");
    int pageIdx = bookmarkJson.indexOf("\"page\":");
    if (pathIdx >= 0 && pageIdx >= 0) {
      int pathStart = pathIdx + 8;
      int pathEnd = bookmarkJson.indexOf('"', pathStart);
      int pageStart = pageIdx + 7;
      int pageEnd = bookmarkJson.indexOf(',', pageStart);
      if (pageEnd < 0) pageEnd = bookmarkJson.indexOf('}', pageStart);

      if (pathEnd > pathStart && pageEnd > pageStart) {
        String path = bookmarkJson.substring(pathStart, pathEnd);
        int page = bookmarkJson.substring(pageStart, pageEnd).toInt();
        String key = prefKeyForBook(path);

        uint16_t pages[MAX_BOOKMARKS];
        uint32_t offsets[MAX_BOOKMARKS];
        uint8_t count = loadBookmarksForKey(key, pages, offsets);

        // Check if bookmark already exists
        bool exists = false;
        for (uint8_t i = 0; i < count; i++) {
          if ((int)pages[i] == page) {
            exists = true;
            break;
          }
        }

        if (!exists && count < MAX_BOOKMARKS) {
          pages[count] = (uint16_t)page;
          offsets[count] = 0xFFFFFFFFUL; // Will be resolved on access
          count++;
          saveBookmarksForKey(key, pages, offsets, count);
          sendBLEStatus("bookmark_added");
        } else if (exists) {
          sendBLEStatus("bookmark_exists");
        } else {
          sendBLEStatus("bookmarks_full");
        }
      } else {
        sendBLEStatus("invalid_json");
      }
    } else {
      sendBLEStatus("invalid_json");
    }
  }
  else if (command.startsWith("DELETE_BOOKMARK:")) {
    String bookmarkJson = command.substring(16);
    // Parse JSON: {"path":"/books/...","page":42}
    int pathIdx = bookmarkJson.indexOf("\"path\":\"");
    int pageIdx = bookmarkJson.indexOf("\"page\":");
    if (pathIdx >= 0 && pageIdx >= 0) {
      int pathStart = pathIdx + 8;
      int pathEnd = bookmarkJson.indexOf('"', pathStart);
      int pageStart = pageIdx + 7;
      int pageEnd = bookmarkJson.indexOf(',', pageStart);
      if (pageEnd < 0) pageEnd = bookmarkJson.indexOf('}', pageStart);

      if (pathEnd > pathStart && pageEnd > pageStart) {
        String path = bookmarkJson.substring(pathStart, pathEnd);
        int page = bookmarkJson.substring(pageStart, pageEnd).toInt();
        String key = prefKeyForBook(path);

        uint16_t pages[MAX_BOOKMARKS];
        uint32_t offsets[MAX_BOOKMARKS];
        uint8_t count = loadBookmarksForKey(key, pages, offsets);

        // Find and remove bookmark
        bool found = false;
        for (uint8_t i = 0; i < count; i++) {
          if ((int)pages[i] == page) {
            found = true;
            // Shift remaining bookmarks
            for (uint8_t j = i; j < count - 1; j++) {
              pages[j] = pages[j + 1];
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
      } else {
        sendBLEStatus("invalid_json");
      }
    } else {
      sendBLEStatus("invalid_json");
    }
  }
  else if (command == "CANCEL_TRANSFER") {
    if (g_bleTransferState != BLE_TRANSFER_IDLE) {
      if (g_bleTransferFile) g_bleTransferFile.close();

      // Clean up temp file if upload was in progress
      if (g_bleTransferState == BLE_TRANSFER_UPLOADING) {
        String tmpPath = g_bleTransferPath + ".tmp";
        if (FS.exists(tmpPath)) FS.remove(tmpPath);
      }

      g_bleTransferState = BLE_TRANSFER_IDLE;
      g_bleTransferPath = "";
      g_bleTransferOffset = 0;
      g_bleTransferTotalSize = 0;
      g_bleTransferBytesSent = 0;
      sendBLEStatus("transfer_cancelled");
    } else {
      sendBLEStatus("no_active_transfer");
    }
  }
  else if (command.startsWith("DELETE:")) {
    String path = command.substring(7);
    Serial.print("[BLE] DELETE command for path: ");
    Serial.println(path);
    if (FS.exists(path)) {
      Serial.println("[BLE] File exists, attempting to remove");

      // Close current file and switch to library mode if it's the one being deleted
      if (g_reader.currentBookPath == path && g_reader.file) {
        g_reader.file.close();
        g_reader.currentBookPath = "";
        g_reader.file = File();
        g_reader.pageIndex = 0;
        g_reader.knownPages = 0;
        Serial.println("[BLE] Closed file that was open for reading");
        // Defer mode switch to main loop to avoid FreeRTOS assert
        g_switchToLibrary = true;
        Serial.println("[BLE] Deferred switch to library mode");
      }

      if (FS.remove(path)) {
        // For books, defer library reload. For apps, reload app list
        if (path.startsWith("/books/")) {
          g_reloadLibrary = true;
          sendBLEStatus("file_delete_success");
          Serial.println("[BLE] File removed successfully, deferred library reload");
        } else if (path.startsWith("/apps/")) {
          sendBLEStatus("app_delete_success");
          scanApps();
          if (mode == MODE_APPS) drawAppsMenu(); // Refresh display if in apps mode
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

static void setupBLE() {
  // Initialize BLE
  Serial.println("[BLE] Initializing BLE...");
  BLEDevice::init("Pala One");
  Serial.println("[BLE] Device name set to 'Pala One'");

  // Create BLE server
  g_bleServer = BLEDevice::createServer();
  g_bleServer->setCallbacks(new PalaBLEServerCallbacks());
  Serial.println("[BLE] Server created");

  // Create BLE service
  BLEService* pService = g_bleServer->createService(PALA_BLE_SERVICE_UUID);

  // Create characteristics
  g_bleCharCmd = pService->createCharacteristic(
    PALA_BLE_CHAR_CMD_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  g_bleCharCmd->setCallbacks(new BLECommandCallbacks());

  g_bleCharData = pService->createCharacteristic(
    PALA_BLE_CHAR_DATA_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  g_bleCharData->setCallbacks(new BLEDataCallbacks());
  g_bleCharData->addDescriptor(new BLE2902());

  g_bleCharStatus = pService->createCharacteristic(
    PALA_BLE_CHAR_STATUS_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  g_bleCharStatus->addDescriptor(new BLE2902());

  // Start service
  pService->start();
  Serial.println("[BLE] Service started");

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(PALA_BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("[BLE] Advertising started");
}

static const LayoutMetrics& getMetrics() {
  if (!g_metricsValid) {
    u8g2.setFont(MAIN_FONT);
    g_metrics.ascent = u8g2.getFontAscent();
    g_metrics.descent = u8g2.getFontDescent();
    g_metrics.lineH = (g_metrics.ascent - g_metrics.descent) + g_settings.lineGap;

    int w = SCREEN_W - (MARGIN_X * 2);
    if (w < 50) w = 50;
    g_metrics.maxWidth = w;

    int maxHeight = SCREEN_H - TOP_PAD - BOT_PAD;
    if (SHOW_PROGRESS_BAR || SHOW_PAGE_NUMBER) maxHeight -= STATUS_H;

    g_metrics.maxLines = maxHeight / g_metrics.lineH;
    if (g_metrics.maxLines < 1) g_metrics.maxLines = 1;
    g_metricsValid = true;
  }
  return g_metrics;
}

// Helper to get font pointer based on family, size, and weight
static const uint8_t* getMainFont(int fam, int sz, int wt) {
  if (fam == 0) {  // Atkinson
    if (wt == 0) {  // regular
      if (sz == 12) return u8g2_font_atkinson_regular_12;
      if (sz == 14) return u8g2_font_atkinson_regular_14;
      if (sz == 16) return u8g2_font_atkinson_regular_16;
      if (sz == 18) return u8g2_font_atkinson_regular_18;
      if (sz == 20) return u8g2_font_atkinson_regular_20;
    } else if (wt == 1) {  // medium
      if (sz == 12) return u8g2_font_atkinson_medium_12;
      if (sz == 14) return u8g2_font_atkinson_medium_14;
      if (sz == 16) return u8g2_font_atkinson_medium_16;
      if (sz == 18) return u8g2_font_atkinson_medium_18;
      if (sz == 20) return u8g2_font_atkinson_medium_20;
    } else if (wt == 2) {  // semibold
      if (sz == 12) return u8g2_font_atkinson_semibold_12;
      if (sz == 14) return u8g2_font_atkinson_semibold_14;
      if (sz == 16) return u8g2_font_atkinson_semibold_16;
      if (sz == 18) return u8g2_font_atkinson_semibold_18;
      if (sz == 20) return u8g2_font_atkinson_semibold_20;
    } else if (wt == 3) {  // bold
      if (sz == 12) return u8g2_font_atkinson_bold_12;
      if (sz == 14) return u8g2_font_atkinson_bold_14;
      if (sz == 16) return u8g2_font_atkinson_bold_16;
      if (sz == 18) return u8g2_font_atkinson_bold_18;
      if (sz == 20) return u8g2_font_atkinson_bold_20;
    } else if (wt == 4) {  // extrabold
      if (sz == 12) return u8g2_font_atkinson_extrabold_12;
      if (sz == 14) return u8g2_font_atkinson_extrabold_14;
      if (sz == 16) return u8g2_font_atkinson_extrabold_16;
      if (sz == 18) return u8g2_font_atkinson_extrabold_18;
      if (sz == 20) return u8g2_font_atkinson_extrabold_20;
    }
  } else if (fam == 1) {  // Literata
    if (wt == 0) {  // regular
      if (sz == 12) return u8g2_font_literata_regular_12;
      if (sz == 14) return u8g2_font_literata_regular_14;
      if (sz == 16) return u8g2_font_literata_regular_16;
      if (sz == 18) return u8g2_font_literata_regular_18;
      if (sz == 20) return u8g2_font_literata_regular_20;
    } else if (wt == 1) {  // medium
      if (sz == 12) return u8g2_font_literata_medium_12;
      if (sz == 14) return u8g2_font_literata_medium_14;
      if (sz == 16) return u8g2_font_literata_medium_16;
      if (sz == 18) return u8g2_font_literata_medium_18;
      if (sz == 20) return u8g2_font_literata_medium_20;
    } else if (wt == 2) {  // semibold
      if (sz == 12) return u8g2_font_literata_semibold_12;
      if (sz == 14) return u8g2_font_literata_semibold_14;
      if (sz == 16) return u8g2_font_literata_semibold_16;
      if (sz == 18) return u8g2_font_literata_semibold_18;
      if (sz == 20) return u8g2_font_literata_semibold_20;
    } else if (wt == 3) {  // bold
      if (sz == 12) return u8g2_font_literata_bold_12;
      if (sz == 14) return u8g2_font_literata_bold_14;
      if (sz == 16) return u8g2_font_literata_bold_16;
      if (sz == 18) return u8g2_font_literata_bold_18;
      if (sz == 20) return u8g2_font_literata_bold_20;
    } else if (wt == 4) {  // extrabold
      if (sz == 12) return u8g2_font_literata_extrabold_12;
      if (sz == 14) return u8g2_font_literata_extrabold_14;
      if (sz == 16) return u8g2_font_literata_extrabold_16;
      if (sz == 18) return u8g2_font_literata_extrabold_18;
      if (sz == 20) return u8g2_font_literata_extrabold_20;
    }
  } else if (fam == 2) {  // Noto Sans
    if (wt == 0) {  // regular
      if (sz == 12) return u8g2_font_notosans_regular_12;
      if (sz == 14) return u8g2_font_notosans_regular_14;
      if (sz == 16) return u8g2_font_notosans_regular_16;
      if (sz == 18) return u8g2_font_notosans_regular_18;
      if (sz == 20) return u8g2_font_notosans_regular_20;
    } else if (wt == 1) {  // medium
      if (sz == 12) return u8g2_font_notosans_medium_12;
      if (sz == 14) return u8g2_font_notosans_medium_14;
      if (sz == 16) return u8g2_font_notosans_medium_16;
      if (sz == 18) return u8g2_font_notosans_medium_18;
      if (sz == 20) return u8g2_font_notosans_medium_20;
    } else if (wt == 2) {  // semibold
      if (sz == 12) return u8g2_font_notosans_semibold_12;
      if (sz == 14) return u8g2_font_notosans_semibold_14;
      if (sz == 16) return u8g2_font_notosans_semibold_16;
      if (sz == 18) return u8g2_font_notosans_semibold_18;
      if (sz == 20) return u8g2_font_notosans_semibold_20;
    } else if (wt == 3) {  // bold
      if (sz == 12) return u8g2_font_notosans_bold_12;
      if (sz == 14) return u8g2_font_notosans_bold_14;
      if (sz == 16) return u8g2_font_notosans_bold_16;
      if (sz == 18) return u8g2_font_notosans_bold_18;
      if (sz == 20) return u8g2_font_notosans_bold_20;
    } else if (wt == 4) {  // extrabold
      if (sz == 12) return u8g2_font_notosans_extrabold_12;
      if (sz == 14) return u8g2_font_notosans_extrabold_14;
      if (sz == 16) return u8g2_font_notosans_extrabold_16;
      if (sz == 18) return u8g2_font_notosans_extrabold_18;
      if (sz == 20) return u8g2_font_notosans_extrabold_20;
    }
  } else if (fam == 3) {  // Noto Serif
    if (wt == 0) {  // regular
      if (sz == 12) return u8g2_font_notoserif_regular_12;
      if (sz == 14) return u8g2_font_notoserif_regular_14;
      if (sz == 16) return u8g2_font_notoserif_regular_16;
      if (sz == 18) return u8g2_font_notoserif_regular_18;
      if (sz == 20) return u8g2_font_notoserif_regular_20;
    } else if (wt == 1) {  // medium
      if (sz == 12) return u8g2_font_notoserif_medium_12;
      if (sz == 14) return u8g2_font_notoserif_medium_14;
      if (sz == 16) return u8g2_font_notoserif_medium_16;
      if (sz == 18) return u8g2_font_notoserif_medium_18;
      if (sz == 20) return u8g2_font_notoserif_medium_20;
    } else if (wt == 2) {  // semibold
      if (sz == 12) return u8g2_font_notoserif_semibold_12;
      if (sz == 14) return u8g2_font_notoserif_semibold_14;
      if (sz == 16) return u8g2_font_notoserif_semibold_16;
      if (sz == 18) return u8g2_font_notoserif_semibold_18;
      if (sz == 20) return u8g2_font_notoserif_semibold_20;
    } else if (wt == 3) {  // bold
      if (sz == 12) return u8g2_font_notoserif_bold_12;
      if (sz == 14) return u8g2_font_notoserif_bold_14;
      if (sz == 16) return u8g2_font_notoserif_bold_16;
      if (sz == 18) return u8g2_font_notoserif_bold_18;
      if (sz == 20) return u8g2_font_notoserif_bold_20;
    } else if (wt == 4) {  // extrabold
      if (sz == 12) return u8g2_font_notoserif_extrabold_12;
      if (sz == 14) return u8g2_font_notoserif_extrabold_14;
      if (sz == 16) return u8g2_font_notoserif_extrabold_16;
      if (sz == 18) return u8g2_font_notoserif_extrabold_18;
      if (sz == 20) return u8g2_font_notoserif_extrabold_20;
    }
  }
  // Default fallback
  if (fam == 0) return u8g2_font_atkinson_regular_12;
  if (fam == 1) return u8g2_font_literata_regular_12;
  if (fam == 2) return u8g2_font_notosans_regular_12;
  return u8g2_font_notoserif_regular_12;
}

// Helper to get bold font pointer based on family and size
// All 4 fonts have extrabold
static const uint8_t* getBoldFont(int fam, int sz) {
  if (fam == 0) {  // Atkinson
    if (sz == 12) return u8g2_font_atkinson_extrabold_12;
    if (sz == 14) return u8g2_font_atkinson_extrabold_14;
    if (sz == 16) return u8g2_font_atkinson_extrabold_16;
    if (sz == 18) return u8g2_font_atkinson_extrabold_18;
    if (sz == 20) return u8g2_font_atkinson_extrabold_20;
  } else if (fam == 1) {  // Literata
    if (sz == 12) return u8g2_font_literata_extrabold_12;
    if (sz == 14) return u8g2_font_literata_extrabold_14;
    if (sz == 16) return u8g2_font_literata_extrabold_16;
    if (sz == 18) return u8g2_font_literata_extrabold_18;
    if (sz == 20) return u8g2_font_literata_extrabold_20;
  } else if (fam == 2) {  // Noto Sans
    if (sz == 12) return u8g2_font_notosans_extrabold_12;
    if (sz == 14) return u8g2_font_notosans_extrabold_14;
    if (sz == 16) return u8g2_font_notosans_extrabold_16;
    if (sz == 18) return u8g2_font_notosans_extrabold_18;
    if (sz == 20) return u8g2_font_notosans_extrabold_20;
  } else if (fam == 3) {  // Noto Serif
    if (sz == 12) return u8g2_font_notoserif_extrabold_12;
    if (sz == 14) return u8g2_font_notoserif_extrabold_14;
    if (sz == 16) return u8g2_font_notoserif_extrabold_16;
    if (sz == 18) return u8g2_font_notoserif_extrabold_18;
    if (sz == 20) return u8g2_font_notoserif_extrabold_20;
  }
  // Default fallback
  return u8g2_font_atkinson_extrabold_12;
}

static void applyFontSize(int sz) {
  int family = g_settings.fontFamily;
  int weight = g_settings.fontWeight;
  
  // Validate size
  if (sz != 12 && sz != 14 && sz != 16 && sz != 18 && sz != 20) {
    sz = 12;
  }
  
  // Validate family
  if (family < 0 || family > 3) {
    family = 0;
  }
  
  // Validate weight
  if (weight < 0 || weight > 4) {
    weight = 0;
  }
  
  MAIN_FONT = getMainFont(family, sz, weight);
  BOLD_FONT = getBoldFont(family, sz);
  
  g_settings.fontSize = sz;
  g_settings.fontFamily = family;
  g_settings.fontWeight = weight;
  invalidateMetrics();
}

static void loadSettings() {
  g_settings.fontFamily = prefs.getInt("cfg_font_family", 0);
  if (g_settings.fontFamily < 0 || g_settings.fontFamily > 3) g_settings.fontFamily = 0;
  
  g_settings.fontWeight = prefs.getInt("cfg_font_weight", 0);
  if (g_settings.fontWeight < 0 || g_settings.fontWeight > 4) g_settings.fontWeight = 0;
  
  applyFontSize(prefs.getInt("cfg_font", 12));

  g_settings.sleepSecs = (uint32_t)prefs.getInt("cfg_sleep", 120);
  if (g_settings.sleepSecs < 10) g_settings.sleepSecs = 10;
  if (g_settings.sleepSecs > 3600) g_settings.sleepSecs = 3600;

  g_settings.lineGap = prefs.getInt("cfg_lgap", 0);
  if (g_settings.lineGap < 0) g_settings.lineGap = 0;
  if (g_settings.lineGap > 4) g_settings.lineGap = 4;

  g_settings.readerLongPressAction = LONGPRESS_BOOKMARK;
  invalidateMetrics();
}

static inline uint32_t sleepAfterMs() {
  return g_settings.sleepSecs * 1000UL;
}

static void markUserActivity() {
  lastUserActionMs = millis();
}

static inline uint32_t isrNowMs() {
  return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static void clearButtonQueue() {
  noInterrupts();
  btnQHead = 0;
  btnQTail = 0;
  interrupts();
}

void IRAM_ATTR btnISR() {
  uint8_t next = (uint8_t)((btnQHead + 1) % BTN_Q);
  if (next == btnQTail) {
    btnQTail = (uint8_t)((btnQTail + 1) % BTN_Q);
    g_isrDropCount++;
  }
  btnQState[btnQHead] = (digitalRead(BTN) == LOW);
  btnQTimeMs[btnQHead] = isrNowMs();
  btnQHead = next;
}

void ButtonState::poll() {
  resetClicks();

  uint8_t headSnap;
  noInterrupts();
  headSnap = btnQHead;
  interrupts();

  while (btnQTail != headSnap) {
    noInterrupts();
    bool rawPressed = btnQState[btnQTail];
    uint32_t edgeT = btnQTimeMs[btnQTail];
    btnQTail = (uint8_t)((btnQTail + 1) % BTN_Q);
    interrupts();

    if ((uint32_t)(edgeT - lastStableChange) <= DEBOUNCE_MS) continue;
    if (rawPressed == stablePressed) continue;

    bool prevPressed = stablePressed;
    stablePressed = rawPressed;
    lastStableChange = edgeT;

    if (!prevPressed && stablePressed) {
      pressStart = edgeT;
      pressArmed = true;
    }

    if (prevPressed && !stablePressed) {
      if (pressArmed) {
        uint32_t dur = (uint32_t)(edgeT - pressStart);
        if (dur >= LONG_MS) {
          clickCount = 0;
          longClick = true;
        } else {
          rawPressCount++;  // count every short press unconditionally
          clickCount++;
          lastRelease = edgeT;
          if (clickCount == 1) firstClickRelease = edgeT;
          if (clickCount >= 4) {
            clickCount = 0;
            quadClick = true;
          }
        }
      }
      pressArmed = false;
      pressStart = 0;
    }
  }

  if (clickCount > 0) {
    uint32_t now = millis();
    bool emit = false;
    if (clickCount <= 2) emit = (uint32_t)(now - lastRelease) > DOUBLE_MS;
    else if (clickCount == 3) emit = (uint32_t)(now - firstClickRelease) > TRIPLE_MS;

    if (emit) {
      if (clickCount == 1) shortClick  = true;
      else if (clickCount == 2) doubleClick = true;
      else if (clickCount == 3) tripleClick = true;
      clickCount = 0;
    }
  }
}

static void resetInputFrontend() {
  // Wait for the button that triggered this transition (wake or triple-click)
  // to be physically released, then debounce. This prevents that single press
  // from leaking into the new mode as an accidental action.
  // We do NOT clear the whole ISR queue — any presses that arrive AFTER
  // release are intentional and should be processed normally.
  uint32_t deadline = millis() + 600; // safety timeout
  while (digitalRead(BTN) == LOW && (uint32_t)(millis()) < deadline) delay(1);
  delay(DEBOUNCE_MS + 2); // minimal debounce after release

  // Discard only events that happened BEFORE this moment (the transition press).
  // Events queued after the release are kept.
  noInterrupts();
  uint8_t headNow = btnQHead;
  interrupts();
  btnQTail = headNow; // advance tail to head = discard old events only
  btns.resetState();
  markUserActivity();
}

// ============================================================================
//  Book / FS helpers
// ============================================================================
static void safeCloseCurrentBook() {
  if (g_reader.file) g_reader.file.close();
}

static void clearCurrentBookState() {
  safeCloseCurrentBook();
  g_reader.currentBookKey = "";
  g_reader.currentBookPath = "";
  g_reader.pageIndex = 0;
  g_reader.knownPages = 0;
  g_reader.eofReached = false;
  g_reader.lastPageStartOffset = 0;
  g_reader.pageTurnsSinceFull = 0;
  g_reader.lastSaveMs = 0;
  g_reader.lastSavedPage = -1;
}

static void resetPreviewState() {
  g_bookmarkUi.previewActive = false;
  g_bookmarkUi.previewSavedPage = 0;
}

static void resetUiEphemeralState() {
  g_toast.msg = "";
  g_toast.untilMs = 0;
  resetPreviewState();
}

static void resetNavigationState() {
  g_library.currentFolder = "";
  g_library.selectedItem = 0;
  g_bookmarkUi.bookIndex = 0;
  g_bookmarkUi.selectedIndex = 0;
}

static bool reopenCurrentBookIfNeeded() {
  if (g_reader.currentBookPath.length() == 0) return false;
  safeCloseCurrentBook();
  g_reader.file = FS.open(g_reader.currentBookPath, "r");
  return (bool)g_reader.file;
}

static void syncWakeState(bool reading) {
  prefs.putInt("wake_mode", reading ? 1 : 0);
  if (reading && g_reader.currentBookPath.length() > 0) prefs.putString("wake_path", g_reader.currentBookPath);
  else prefs.remove("wake_path");
}

static void enterLibraryRoot(bool redraw) {
  safeCloseCurrentBook();
  resetPreviewState();
  resetNavigationState();
  syncWakeState(false);
  mode = MODE_LIBRARY;
  if (redraw) drawLibrary();
}

static void ensureBooksDir() {
  if (!FS.exists("/books")) FS.mkdir("/books");
}

static String stripTxtExt(const String& s) {
  return s.endsWith(".txt") ? s.substring(0, s.length() - 4) : s;
}

static String lastPathComponent(const String& path) {
  int slash = path.lastIndexOf('/');
  return (slash >= 0) ? path.substring(slash + 1) : path;
}

static String folderParent(const String& relPath) {
  int slash = relPath.lastIndexOf('/');
  return (slash < 0) ? String("") : relPath.substring(0, slash);
}

static String prettyRelativeLabel(const String& relPath) {
  String out;
  out.reserve(relPath.length() + 8);
  for (size_t i = 0; i < relPath.length(); i++) {
    char c = relPath[i];
    if (c == '_') out += ' ';
    else if (c == '/') out += " / ";
    else out += c;
  }
  return stripTxtExt(out);
}

static String folderLeafLabel(const String& relPath) {
  String leaf = lastPathComponent(relPath);
  leaf.replace('_', ' ');
  return leaf;
}

static bool isFolderExpanded(int idx) {
  if (idx < 0 || idx >= g_library.folderCount) return false;
  return g_library.folderExpanded[idx];
}

static void setFolderExpanded(int idx, bool expanded) {
  if (idx < 0 || idx >= g_library.folderCount) return;
  g_library.folderExpanded[idx] = expanded;
}


static String bookLeafLabel(int idx) {
  String leaf = stripTxtExt(lastPathComponent(String(g_library.books[idx].path)));
  leaf.replace('_', ' ');
  return leaf;
}

static bool isAllowedFolderByte(uint8_t c) {
  return ((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '_' || c == '-' || c == ' ' ||
          c >= 128);
}

static String sanitizeFolderSegment(const String& segment) {
  String clean;
  clean.reserve(segment.length());
  for (size_t i = 0; i < segment.length(); i++) {
    uint8_t c = (uint8_t)segment[i];
    clean += isAllowedFolderByte(c) ? (char)c : '_';
  }
  clean.trim();
  return clean;
}

static String sanitizeFolderInput(const String& raw) {
  String normalized = raw;
  normalized.replace('\\', '/');

  String out;
  int start = 0;
  while (start <= normalized.length()) {
    int slash = normalized.indexOf('/', start);
    String part = (slash >= 0) ? normalized.substring(start, slash) : normalized.substring(start);
    start = (slash >= 0) ? slash + 1 : normalized.length() + 1;

    part.trim();
    if (part.length() == 0 || part == "." || part == "..") continue;
    String clean = sanitizeFolderSegment(part);
    if (clean.length() == 0) continue;
    if (out.length() > 0) out += '/';
    out += clean;
  }
  return out;
}

static String sanitizeUploadedFilename(String fname) {
  int slash = fname.lastIndexOf('/');
  if (slash >= 0) fname = fname.substring(slash + 1);

  String clean;
  clean.reserve(fname.length());
  for (size_t i = 0; i < fname.length(); i++) {
    uint8_t c = (uint8_t)fname[i];
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        c == '_' || c == '-' || c == ' ' || c == '.' ||
        c >= 128) {
      clean += (char)c;
    } else {
      clean += '_';
    }
  }

  clean.replace("..", "");
  while (clean.startsWith(".")) clean.remove(0, 1);
  if (!clean.endsWith(".txt")) clean += ".txt";
  if (clean.length() == 0) clean = "book.txt";
  return clean;
}

static bool ensureDirRecursive(const String& path) {
  if (path.length() == 0 || path == "/") return true;
  if (FS.exists(path)) return true;

  int slash = path.lastIndexOf('/');
  if (slash > 0) {
    String parent = path.substring(0, slash);
    if (parent.length() > 0 && !FS.exists(parent)) {
      if (!ensureDirRecursive(parent)) return false;
    }
  }
  return FS.mkdir(path);
}

static bool isDirEmpty(const String& path) {
  File dir = FS.open(path);
  if (!dir || !dir.isDirectory()) {
    if (dir) dir.close();
    return false;
  }
  File f = dir.openNextFile();
  bool empty = !f;
  if (f) f.close();
  dir.close();
  return empty;
}

static uint32_t fnv1a32(const char* s) {
  uint32_t h = 2166136261u;
  while (*s) {
    h ^= (uint8_t)(*s++);
    h *= 16777619u;
  }
  return h;
}

static uint32_t hashPath32(const String& path) {
  return fnv1a32(path.c_str());
}

static void resetOffsetCache() {
  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    g_offsetCache[i].pathHash = 0;
    g_offsetCache[i].page = -1;
    g_offsetCache[i].offset = 0;
    g_offsetCache[i].stamp = 0;
  }
  g_offsetCacheStamp = 1;
}

static bool lookupOffsetCache(const String& path, int targetPage, int& cachedPage, uint32_t& cachedOffset) {
  uint32_t h = hashPath32(path);
  bool found = false;
  int bestPage = -1;
  uint32_t bestOffset = 0;
  uint32_t bestStamp = 0;

  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    if (g_offsetCache[i].pathHash != h) continue;
    if (g_offsetCache[i].page > targetPage) continue;
    if (g_offsetCache[i].page > bestPage || (g_offsetCache[i].page == bestPage && g_offsetCache[i].stamp > bestStamp)) {
      bestPage = g_offsetCache[i].page;
      bestOffset = g_offsetCache[i].offset;
      bestStamp = g_offsetCache[i].stamp;
      found = true;
    }
  }

  if (found) {
    cachedPage = bestPage;
    cachedOffset = bestOffset;
  }
  return found;
}

static void storeOffsetCache(const String& path, int page, uint32_t offset) {
  if (page < 0) return;

  uint32_t h = hashPath32(path);
  int slot = -1;
  uint32_t oldestStamp = 0xFFFFFFFFu;

  for (int i = 0; i < OFFSET_CACHE_SIZE; i++) {
    if (g_offsetCache[i].pathHash == h && g_offsetCache[i].page == page) {
      slot = i;
      break;
    }
    if (g_offsetCache[i].page < 0) {
      slot = i;
      break;
    }
    if (g_offsetCache[i].stamp < oldestStamp) {
      oldestStamp = g_offsetCache[i].stamp;
      slot = i;
    }
  }

  if (slot < 0) return;
  g_offsetCache[slot].pathHash = h;
  g_offsetCache[slot].page = page;
  g_offsetCache[slot].offset = offset;
  g_offsetCache[slot].stamp = g_offsetCacheStamp++;
  if (g_offsetCacheStamp == 0) g_offsetCacheStamp = 1;
}

static String prefKeyForBook(const String& path) {
  uint32_t h = fnv1a32(path.c_str());
  char buf[16];
  snprintf(buf, sizeof(buf), "b_%08lx", (unsigned long)h);
  return String(buf);
}

static int savedPageForBookPath(const String& path) {
  String key = prefKeyForBook(path);
  int p = prefs.getInt((key + "_p").c_str(), 0);
  return (p < 0) ? 0 : p;
}

static String pageCachePathForBook(const String& path) {
  return String("/pc_") + prefKeyForBook(path) + ".bin";
}

static bool loadPageOffsetCacheForBook(const String& path, size_t expectedSize) {
  String cachePath = pageCachePathForBook(path);
  File f = FS.open(cachePath, "r");
  if (!f) return false;

  uint32_t magic = 0;
  uint32_t fileSize = 0;
  uint16_t count = 0;

  if (f.read((uint8_t*)&magic, sizeof(magic)) != sizeof(magic)) { f.close(); return false; }
  if (f.read((uint8_t*)&fileSize, sizeof(fileSize)) != sizeof(fileSize)) { f.close(); return false; }
  if (f.read((uint8_t*)&count, sizeof(count)) != sizeof(count)) { f.close(); return false; }

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

static void savePageOffsetCacheForBook(const String& path, size_t fileSize) {
  if (g_reader.knownPages <= 1) return;

  String cachePath = pageCachePathForBook(path);
  File f = FS.open(cachePath, "w");
  if (!f) return;

  uint32_t magic = 0x50434F46UL;
  uint32_t size32 = (uint32_t)fileSize;
  uint16_t count16 = (uint16_t)min(g_reader.knownPages, MAX_PAGES);

  f.write((const uint8_t*)&magic, sizeof(magic));
  f.write((const uint8_t*)&size32, sizeof(size32));
  f.write((const uint8_t*)&count16, sizeof(count16));
  f.write((const uint8_t*)g_reader.pageOffsets, count16 * sizeof(uint32_t));
  f.close();
}

static void invalidateAllPageCaches() {
  // Page offsets are computed from the current font size and line spacing.
  // When either changes, ALL cached offsets are invalid -- both the in-memory
  // arrays and the on-disk .bin files. Mark each book as "needs relocation"
  // and save the current byte offset. On next open, relocateOpenBookToOffset
  // will find the correct page in the new layout.

  resetOffsetCache();

  // Remove all on-disk page-cache files (pc_*.bin)
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

  // Mark all progress files for relocation
  File progressDir = FS.open("/books/.progress");
  if (progressDir && progressDir.isDirectory()) {
    File f = progressDir.openNextFile();
    while (f) {
      String name = String(f.name());
      if (name.endsWith(".txt")) {
        f.close();
        // Read current progress
        String progressPath = "/books/.progress/" + name;
        File progressFile = FS.open(progressPath.c_str(), "r");
        if (progressFile) {
          String pageStr = progressFile.readStringUntil('\n');
          String offsetStr = progressFile.readStringUntil('\n');
          progressFile.readStringUntil('\n');  // skip old relocation flag
          progressFile.close();
          
          uint32_t currentOffset = offsetStr.toInt();
          
          // Rewrite with relocation flag set
          FS.remove(progressPath.c_str());
          File newFile = FS.open(progressPath.c_str(), "w");
          if (newFile) {
            newFile.println(pageStr);
            newFile.println(currentOffset);
            newFile.println("1");  // set relocation flag
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

  // For the currently open book, save current byte offset before resetting
  if (g_reader.currentBookPath.length() > 0) {
    uint32_t currentOffset = g_reader.pageOffsets[g_reader.pageIndex];
    String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
    File progressFile = FS.open(progressPath.c_str(), "r");
    if (progressFile) {
      String pageStr = progressFile.readStringUntil('\n');
      String offsetStr = progressFile.readStringUntil('\n');
      progressFile.readStringUntil('\n');  // skip old relocation flag
      progressFile.close();
      
      // Rewrite with current offset and relocation flag set
      FS.remove(progressPath.c_str());
      File newFile = FS.open(progressPath.c_str(), "w");
      if (newFile) {
        newFile.println(pageStr);
        newFile.println(currentOffset);
        newFile.println("1");  // set relocation flag
        newFile.close();
        Serial.print("[Invalidate] Marked current book for relocation with offset ");
        Serial.println(currentOffset);
      }
    }
  }
}

static void sanitizeListText(String& s) {
  s.replace("\r", "");
  s.replace("\n", " ");
  s.replace("\t", " ");
  while (s.indexOf("  ") != -1) s.replace("  ", " ");
  s.trim();
  if ((int)s.length() > MAX_LIST_TEXT) s = s.substring(0, MAX_LIST_TEXT);
}

static void loadListItems() {
  g_list.count = 0;
  g_list.selectedIndex = 0;
  uint8_t buf[1 + MAX_LIST_ITEMS * (1 + MAX_LIST_TEXT + 1)] = {0};
  size_t got = prefs.getBytes("list_v1", buf, sizeof(buf));
  if (got < 1) return;
  uint8_t count = buf[0];
  if (count > MAX_LIST_ITEMS) count = MAX_LIST_ITEMS;
  size_t pos = 1;
  for (uint8_t i = 0; i < count; i++) {
    if (pos + 1 + MAX_LIST_TEXT + 1 > got) break;
    g_list.items[i].done = buf[pos++];
    memcpy(g_list.items[i].text, &buf[pos], MAX_LIST_TEXT + 1);
    g_list.items[i].text[MAX_LIST_TEXT] = '\0';
    pos += (MAX_LIST_TEXT + 1);
    String t = String(g_list.items[i].text);
    sanitizeListText(t);
    if (t.length() == 0) continue;
    strncpy(g_list.items[g_list.count].text, t.c_str(), MAX_LIST_TEXT);
    g_list.items[g_list.count].text[MAX_LIST_TEXT] = '\0';
    g_list.items[g_list.count].done = g_list.items[i].done ? 1 : 0;
    g_list.count++;
  }
}

static void saveListItems() {
  uint8_t buf[1 + MAX_LIST_ITEMS * (1 + MAX_LIST_TEXT + 1)] = {0};
  buf[0] = (uint8_t)g_list.count;
  size_t pos = 1;
  for (int i = 0; i < g_list.count && i < MAX_LIST_ITEMS; i++) {
    buf[pos++] = g_list.items[i].done ? 1 : 0;
    memset(&buf[pos], 0, MAX_LIST_TEXT + 1);
    strncpy((char*)&buf[pos], g_list.items[i].text, MAX_LIST_TEXT);
    pos += (MAX_LIST_TEXT + 1);
  }
  prefs.putBytes("list_v1", buf, pos);
}

static bool listHasVisibleItems() {
  for (int i = 0; i < g_list.count; i++) {
    if (g_list.items[i].text[0]) return true;
  }
  return false;
}

static String bmKeyFor(const String& bookKey) {
  return bookKey + "_bm";
}

static void deleteBookMetadata(const String& path) {
  String key = prefKeyForBook(path);
  prefs.remove((key + "_p").c_str());
  prefs.remove(bmKeyFor(key).c_str());
  String cachePath = pageCachePathForBook(path);
  if (FS.exists(cachePath)) FS.remove(cachePath);
  if (prefs.getString("wake_path", "") == path) {
    prefs.remove("wake_path");
    prefs.putInt("wake_mode", 0);
  }
}

static void migrateBookMetadata(const String& oldPath, const String& newPath) {
  String oldKey = prefKeyForBook(oldPath);
  String newKey = prefKeyForBook(newPath);

  int progress = prefs.getInt((oldKey + "_p").c_str(), -1);
  if (progress >= 0) {
    prefs.putInt((newKey + "_p").c_str(), progress);
    prefs.remove((oldKey + "_p").c_str());
  }

  uint8_t buf[1 + MAX_BOOKMARKS * 6] = {0};
  size_t got = prefs.getBytes(bmKeyFor(oldKey).c_str(), buf, sizeof(buf));
  if (got > 0) {
    prefs.putBytes(bmKeyFor(newKey).c_str(), buf, got);
    prefs.remove(bmKeyFor(oldKey).c_str());
  }

  String oldCache = pageCachePathForBook(oldPath);
  String newCache = pageCachePathForBook(newPath);
  if (FS.exists(oldCache)) {
    if (FS.exists(newCache)) FS.remove(newCache);
    FS.rename(oldCache, newCache);
  }

  if (prefs.getString("wake_path", "") == oldPath) {
    prefs.putString("wake_path", newPath);
  }

  if (g_reader.currentBookPath == oldPath) {
    g_reader.currentBookPath = newPath;
    g_reader.currentBookKey = newKey;
  }
}

// ============================================================================
//  Book library scan / sort
// ============================================================================
static void addFolderIfMissing(const String& folderRel) {
  if (folderRel.length() == 0) return;
  for (int i = 0; i < g_library.folderCount; i++) {
    if (strcmp(g_library.folders[i], folderRel.c_str()) == 0) return;
  }
  if (g_library.folderCount < MAX_FOLDERS) {
    strncpy(g_library.folders[g_library.folderCount], folderRel.c_str(), 63);
    g_library.folders[g_library.folderCount][63] = '\0';
    g_library.folderCount++;
  }
}

static void sortFolders() {
  for (int i = 0; i < g_library.folderCount - 1; i++) {
    for (int j = i + 1; j < g_library.folderCount; j++) {
      if (strcmp(g_library.folders[j], g_library.folders[i]) < 0) {
        char tmp[64];
        memcpy(tmp, g_library.folders[i], 64);
        memcpy(g_library.folders[i], g_library.folders[j], 64);
        memcpy(g_library.folders[j], tmp, 64);
      }
    }
  }
}

static void sortBooks() {
  for (int i = 0; i < g_library.bookCount - 1; i++) {
    for (int j = i + 1; j < g_library.bookCount; j++) {
      if (strcmp(g_library.books[j].name, g_library.books[i].name) < 0) {
        BookInfo tmp = g_library.books[i];
        g_library.books[i] = g_library.books[j];
        g_library.books[j] = tmp;
      }
    }
  }
}

static void scanBooksRecursive(const String& absDir, const String& relDir) {
  File dir = FS.open(absDir);
  if (!dir || !dir.isDirectory()) return;

  File f = dir.openNextFile();
  while (f) {
    String entryName = String(f.name());
    String absPath = entryName.startsWith("/") ? entryName : (absDir + "/" + entryName);
    String leaf = lastPathComponent(absPath);

    if (f.isDirectory()) {
      // Skip .progress directory
      if (leaf == ".progress") {
        f.close();
        f = dir.openNextFile();
        continue;
      }
      String childRel = relDir.length() ? (relDir + "/" + leaf) : leaf;
      addFolderIfMissing(childRel);
      scanBooksRecursive(absPath, childRel);
    } else if (g_library.bookCount < MAX_BOOKS && absPath.endsWith(".txt")) {
      String relFile = relDir.length() ? (relDir + "/" + leaf) : leaf;
      BookInfo& b = g_library.books[g_library.bookCount];

      strncpy(b.path, absPath.c_str(), 95);
      b.path[95] = '\0';
      strncpy(b.folder, relDir.c_str(), 63);
      b.folder[63] = '\0';

      String pretty = prettyRelativeLabel(relFile);
      strncpy(b.name, pretty.c_str(), 79);
      b.name[79] = '\0';
      b.size = f.size();
      g_library.bookCount++;
    }

    f.close();
    f = dir.openNextFile();
  }

  dir.close();
}

static void loadBooks() {
  int savedListSel = g_list.selectedIndex;

  char expandedBefore[MAX_FOLDERS][64];
  int expandedCount = 0;
  for (int i = 0; i < g_library.folderCount && expandedCount < MAX_FOLDERS; i++) {
    if (g_library.folderExpanded[i]) {
      strncpy(expandedBefore[expandedCount], g_library.folders[i], 63);
      expandedBefore[expandedCount][63] = '\0';
      expandedCount++;
    }
  }

  loadListItems();
  if (g_list.count > 0) {
    if (savedListSel >= g_list.count) savedListSel = g_list.count - 1;
    if (savedListSel < 0) savedListSel = 0;
    g_list.selectedIndex = savedListSel;
  } else {
    g_list.selectedIndex = 0;
  }

  g_library.bookCount = 0;
  g_library.folderCount = 0;
  for (int i = 0; i < MAX_FOLDERS; i++) g_library.folderExpanded[i] = false;
  resetOffsetCache();

  ensureBooksDir();
  scanBooksRecursive("/books", "");
  sortFolders();
  sortBooks();

  for (int i = 0; i < g_library.folderCount; i++) {
    for (int j = 0; j < expandedCount; j++) {
      if (strcmp(g_library.folders[i], expandedBefore[j]) == 0) {
        g_library.folderExpanded[i] = true;
        break;
      }
    }
  }

  buildLibraryEntries();
  if (g_library.selectedItem < 0) g_library.selectedItem = 0;
  if (g_library.selectedItem >= g_library.entryCount) g_library.selectedItem = max(0, g_library.entryCount - 1);
}


static bool libraryFolderExists(const String& folderRel) {
  if (folderRel.length() == 0) return true;
  for (int i = 0; i < g_library.folderCount; i++) {
    if (strcmp(g_library.folders[i], folderRel.c_str()) == 0) return true;
  }
  for (int i = 0; i < g_library.bookCount; i++) {
    if (strcmp(g_library.books[i].folder, folderRel.c_str()) == 0) return true;
  }
  return false;
}

static String libraryEntryLabel(int idx) {
  if (idx < 0 || idx >= g_library.entryCount) return "";
  switch (g_library.entryTypes[idx]) {
    case LIB_ENTRY_BACK:      return ".. Back";
    case LIB_ENTRY_FOLDER: {
      int folderIdx = g_library.entryRefs[idx];
      String prefix = isFolderExpanded(folderIdx) ? "- " : "+ ";
      return prefix + folderLeafLabel(String(g_library.folders[folderIdx]));
    }
    case LIB_ENTRY_BOOK:      return bookLeafLabel(g_library.entryRefs[idx]);
    case LIB_ENTRY_BOOKMARKS: return "Bookmarks";
    case LIB_ENTRY_LIST:      return "List";
    case LIB_ENTRY_ABOUT:     return "Device";
    case LIB_ENTRY_APPS:      return "Apps";
  }
  return "";
}

static void addLibraryBookEntry(int bookIdx, int depth) {
  if (g_library.entryCount >= MAX_LIBRARY_ENTRIES) return;
  g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_BOOK;
  g_library.entryRefs[g_library.entryCount] = bookIdx;
  g_library.entryDepths[g_library.entryCount] = depth;
  g_library.entryCount++;
}

static void addLibraryFolderTree(const String& parent, int depth) {
  for (int i = 0; i < g_library.folderCount && g_library.entryCount < MAX_LIBRARY_ENTRIES; i++) {
    String folderRel = String(g_library.folders[i]);
    if (folderParent(folderRel) != parent) continue;

    g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_FOLDER;
    g_library.entryRefs[g_library.entryCount] = i;
    g_library.entryDepths[g_library.entryCount] = depth;
    g_library.entryCount++;

    if (!isFolderExpanded(i)) continue;

    for (int b = 0; b < g_library.bookCount && g_library.entryCount < MAX_LIBRARY_ENTRIES; b++) {
      if (String(g_library.books[b].folder) == folderRel) {
        addLibraryBookEntry(b, depth + 1);
      }
    }

    addLibraryFolderTree(folderRel, depth + 1);
  }
}

static void buildLibraryEntries() {
  g_library.entryCount = 0;

  addLibraryFolderTree(String(""), 0);

  for (int b = 0; b < g_library.bookCount && g_library.entryCount < MAX_LIBRARY_ENTRIES; b++) {
    if (String(g_library.books[b].folder).length() == 0) {
      addLibraryBookEntry(b, 0);
    }
  }

  if (g_library.entryCount < MAX_LIBRARY_ENTRIES) {
    g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_BOOKMARKS;
    g_library.entryRefs[g_library.entryCount] = -1;
    g_library.entryDepths[g_library.entryCount] = 0;
    g_library.entryCount++;
  }
  if (listHasVisibleItems() && g_library.entryCount < MAX_LIBRARY_ENTRIES) {
    g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_LIST;
    g_library.entryRefs[g_library.entryCount] = -1;
    g_library.entryDepths[g_library.entryCount] = 0;
    g_library.entryCount++;
  }
  if (g_library.entryCount < MAX_LIBRARY_ENTRIES) {
    g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_ABOUT;
    g_library.entryRefs[g_library.entryCount] = -1;
    g_library.entryDepths[g_library.entryCount] = 0;
    g_library.entryCount++;
  }
  if (g_library.entryCount < MAX_LIBRARY_ENTRIES) {
    g_library.entryTypes[g_library.entryCount] = LIB_ENTRY_APPS;
    g_library.entryRefs[g_library.entryCount] = -1;
    g_library.entryDepths[g_library.entryCount] = 0;
    g_library.entryCount++;
  }

  if (g_library.selectedItem < 0) g_library.selectedItem = 0;
  if (g_library.selectedItem >= g_library.entryCount) g_library.selectedItem = max(0, g_library.entryCount - 1);
}


// ============================================================================
//  Typography normalization / UTF-8 helpers / bookmark labels
// ============================================================================
static inline bool isUtf8ContinuationByte(uint8_t b) {
  return (b & 0xC0) == 0x80;
}

static int utf8CharLenFromLead(uint8_t b) {
  if (b < 0x80) return 1;
  if ((b & 0xE0) == 0xC0) return 2;
  if ((b & 0xF0) == 0xE0) return 3;
  if ((b & 0xF8) == 0xF0) return 4;
  return 1;
}

static int utf8SafeCharLenAt(const String& s, int index) {
  if (index < 0 || index >= (int)s.length()) return 0;
  uint8_t b0 = (uint8_t)s[index];
  int len = utf8CharLenFromLead(b0);
  if (index + len > (int)s.length()) return 1;
  for (int i = 1; i < len; i++) {
    if (!isUtf8ContinuationByte((uint8_t)s[index + i])) return 1;
  }
  return len;
}

static String utf8CharAt(const String& s, int index) {
  int len = utf8SafeCharLenAt(s, index);
  if (len <= 0) return String("");
  return s.substring(index, index + len);
}

static bool isBreakableWhitespaceChar(const String& ch) {
  return ch == " " || ch == "\n" || ch == "\t";
}

static bool isBreakablePunctuationChar(const String& ch) {
  return ch == "." || ch == "," || ch == ";" || ch == ":" || ch == "!" || ch == "?" ||
         ch == ")" || ch == "]" || ch == "}" || ch == "-" || ch == "/";
}

static String normalizeTypography(const String& in) {

  String out;
  out.reserve(in.length() + 8);
  size_t i = 0;

  while (i < in.length()) {
    uint8_t b0 = (uint8_t)in[i];

    // UTF-8 BOM
    if (i == 0 && b0 == 0xEF && i + 2 < in.length() &&
        (uint8_t)in[i + 1] == 0xBB && (uint8_t)in[i + 2] == 0xBF) {
      i += 3;
      continue;
    }

    // Non-breaking space -> normal space
    if (b0 == 0xC2 && i + 1 < in.length() && (uint8_t)in[i + 1] == 0xA0) {
      out += ' ';
      i += 2;
      continue;
    }

    // 0xC2 0xAB = U+00AB <<  (left guillemet)
    // 0xC2 0xBB = U+00BB >>  (right guillemet)
    if (b0 == 0xC2 && i + 1 < in.length()) {
      uint8_t b1 = (uint8_t)in[i + 1];
      if (b1 == 0xAD) { i += 2; continue; } // soft hyphen (U+00AD) — discard
      if (b1 == 0xAB || b1 == 0xBB) { out += '"'; i += 2; continue; }
      // 0x91/0x92 are valid UTF-8 continuation bytes after 0xC2 (U+00D1, U+00D2)
      // but not quote chars. Leave the old 0x91/0x92 branch: those are
      // Windows-1252 smart quotes that sometimes leak through despite being
      // technically invalid UTF-8; matching them defensively is harmless.
      if (b1 == 0x91 || b1 == 0x92) { out += '\''; i += 2; continue; }
    }

    if (b0 == 0xE2 && i + 2 < in.length()) {
      uint8_t b1 = (uint8_t)in[i + 1];
      uint8_t b2 = (uint8_t)in[i + 2];
      if (b1 == 0x80) {
        if (b2 == 0x98 || b2 == 0x99 || b2 == 0x9A || b2 == 0x9B) { out += '\''; i += 3; continue; }
        if (b2 == 0x9C || b2 == 0x9D || b2 == 0x9E || b2 == 0x9F || b2 == 0xB9 || b2 == 0xBA) { out += '"'; i += 3; continue; }
        if (b2 == 0x93 || b2 == 0x94 || b2 == 0x95) { out += '-'; i += 3; continue; }
        if (b2 == 0xA6) { out += "..."; i += 3; continue; }
      }
    }

    out += (char)b0;
    i++;
  }

  return out;
}

// -------- Text compaction (storage optimization) --------
static String compactText(const String& in) {
  String out;
  out.reserve(in.length());

  bool lastWasSpace = false;
  int newlineCount = 0;

  for (size_t i = 0; i < in.length(); i++) {
    char c = in[i];

    if (c == '\r') continue;
    if (c == '\t') c = ' ';

    if (c == '\n') {
      while (out.length() > 0 && out[out.length() - 1] == ' ') {
        out.remove(out.length() - 1);
      }
      newlineCount++;
      if (newlineCount <= 2) out += '\n';
      lastWasSpace = false;
      continue;
    }

    newlineCount = 0;

    if (c == ' ') {
      if (!lastWasSpace) {
        out += ' ';
        lastWasSpace = true;
      }
      continue;
    }

    lastWasSpace = false;
    out += c;
  }

  while (out.length() > 0 &&
         (out[out.length() - 1] == ' ' || out[out.length() - 1] == '\n')) {
    out.remove(out.length() - 1);
  }

  return out;
}
static inline bool isBookmarkLabelWordChar(char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         ((uint8_t)c >= 128);
}

static String readBookmarkLabelAtOffset(File& f, uint32_t off, int page) {
  if (!f.seek(off)) return String("p. ") + String(page + 1);

  String label;
  label.reserve(80);

  const int maxWords = 5;
  const int maxChars = 44;
  int words = 0;
  int scanned = 0;
  bool inWord = false;
  bool pendingSpace = false;

  while (f.available() && scanned < 240) {
    char c = (char)f.read();
    scanned++;

    if (c == '\r') continue;
    if (c == '\n' || c == '\t') c = ' ';

    if (isBookmarkLabelWordChar(c)) {
      if (!inWord) {
        if (words >= maxWords) break;
        if (pendingSpace && label.length() > 0 && label.length() < maxChars) label += ' ';
        pendingSpace = false;
        inWord = true;
      }
      if (label.length() < maxChars) label += c;
      continue;
    }

    if (c == ' ') {
      if (inWord) {
        words++;
        inWord = false;
        pendingSpace = (words < maxWords);
        if (words >= maxWords) break;
      }
      continue;
    }

    if (inWord && label.length() < maxChars) {
      label += c;
    }
  }

  if (inWord) words++;
  label.trim();
  if (label.length() == 0) label = "Page";
  label += " - p. ";
  label += String(page + 1);
  return label;
}

// ============================================================================
//  Progress / bookmarks storage
// ============================================================================
static inline void resetSaveThrottle() {
  g_reader.lastSaveMs = 0;
  g_reader.lastSavedPage = -1;
}

static void saveProgress(bool force) {
  if (!g_reader.file) return;

  if (!force) {
    if (g_reader.pageIndex == g_reader.lastSavedPage) return;
  }

  Serial.print("[Progress] Saving progress for book: ");
  Serial.print(g_reader.currentBookKey);
  Serial.print(", page: ");
  Serial.print(g_reader.pageIndex);
  Serial.print(", offset: ");
  Serial.println(g_reader.pageOffsets[g_reader.pageIndex]);

  // Save page index and byte offset to filesystem
  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  FS.remove(progressPath.c_str());
  File f = FS.open(progressPath.c_str(), "w");
  if (f) {
    f.println(g_reader.pageIndex);
    f.println(g_reader.pageOffsets[g_reader.pageIndex]);
    f.println("0");  // relocation flag: 0 = no relocation needed
    f.close();
  }
  
  // Remove old word segment file
  String segmentPath = "/books/.progress/" + g_reader.currentBookKey + "_seg.txt";
  if (FS.exists(segmentPath.c_str())) {
    FS.remove(segmentPath.c_str());
  }
  
  g_reader.lastSaveMs = millis();
  g_reader.lastSavedPage = g_reader.pageIndex;
}

static uint8_t loadBookmarksForKey(const String& bookKey, uint16_t outPages[MAX_BOOKMARKS], uint32_t outOffsets[MAX_BOOKMARKS]) {
  const size_t NEW_SIZE = 1 + MAX_BOOKMARKS * 6;
  uint8_t buf[NEW_SIZE] = {0};
  size_t got = prefs.getBytes(bmKeyFor(bookKey).c_str(), buf, sizeof(buf));
  if (got < 1) return 0;

  uint8_t count = buf[0];
  if (count > MAX_BOOKMARKS) count = MAX_BOOKMARKS;

  bool hasOffsets = (got >= (size_t)(1 + count * 6));
  if (hasOffsets) {
    for (uint8_t i = 0; i < count; i++) {
      size_t base = 1 + (size_t)i * 6;
      outPages[i] = (uint16_t)(buf[base + 0] | (buf[base + 1] << 8));
      outOffsets[i] = (uint32_t)buf[base + 2] |
                      ((uint32_t)buf[base + 3] << 8) |
                      ((uint32_t)buf[base + 4] << 16) |
                      ((uint32_t)buf[base + 5] << 24);
    }
  } else {
    for (uint8_t i = 0; i < count; i++) {
      size_t base = 1 + (size_t)i * 2;
      outPages[i] = (uint16_t)(buf[base + 0] | (buf[base + 1] << 8));
      outOffsets[i] = 0xFFFFFFFFUL;
    }
  }
  return count;
}

static void saveBookmarksForKey(const String& bookKey, const uint16_t pages[MAX_BOOKMARKS], const uint32_t offsets[MAX_BOOKMARKS], uint8_t count) {
  if (count > MAX_BOOKMARKS) count = MAX_BOOKMARKS;
  const size_t BUF_SIZE = 1 + MAX_BOOKMARKS * 6;
  uint8_t buf[BUF_SIZE] = {0};
  buf[0] = count;
  for (uint8_t i = 0; i < count; i++) {
    size_t base = 1 + (size_t)i * 6;
    uint16_t page = pages[i];
    uint32_t off = offsets[i];
    buf[base + 0] = (uint8_t)(page & 0xFF);
    buf[base + 1] = (uint8_t)((page >> 8) & 0xFF);
    buf[base + 2] = (uint8_t)(off & 0xFF);
    buf[base + 3] = (uint8_t)((off >> 8) & 0xFF);
    buf[base + 4] = (uint8_t)((off >> 16) & 0xFF);
    buf[base + 5] = (uint8_t)((off >> 24) & 0xFF);
  }
  prefs.putBytes(bmKeyFor(bookKey).c_str(), buf, 1 + count * 6);
}

static const char* addBookmarkForCurrentBook() {
  if (g_reader.currentBookKey.length() == 0) return nullptr;

  uint16_t pages[MAX_BOOKMARKS];
  uint32_t offsets[MAX_BOOKMARKS];
  uint8_t count = loadBookmarksForKey(g_reader.currentBookKey, pages, offsets);

  for (uint8_t i = 0; i < count; i++) {
    if ((int)pages[i] == g_reader.pageIndex) return "Bookmark exists";
  }

  uint32_t currentOffset = g_reader.lastPageStartOffset;

  if (count < MAX_BOOKMARKS) {
    pages[count] = (uint16_t)g_reader.pageIndex;
    offsets[count] = currentOffset;
    count++;
  } else {
    for (uint8_t i = 1; i < MAX_BOOKMARKS; i++) {
      pages[i - 1] = pages[i];
      offsets[i - 1] = offsets[i];
    }
    pages[MAX_BOOKMARKS - 1] = (uint16_t)g_reader.pageIndex;
    offsets[MAX_BOOKMARKS - 1] = currentOffset;
    count = MAX_BOOKMARKS;
  }

  for (uint8_t i = 0; i < count; i++) {
    for (uint8_t j = i + 1; j < count; j++) {
      if (pages[j] < pages[i]) {
        uint16_t tp = pages[i];
        pages[i] = pages[j];
        pages[j] = tp;
        uint32_t to = offsets[i];
        offsets[i] = offsets[j];
        offsets[j] = to;
      }
    }
  }

  saveBookmarksForKey(g_reader.currentBookKey, pages, offsets, count);
  if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
  return "Bookmark saved";
}

// ============================================================================
//  Battery
// ============================================================================
#if HAS_BATTERY
static inline void adcSetupOnce() {
  pinMode(BAT_ADC_IN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(BAT_ADC_IN, ADC_11db);
}

static int cmpUint16(const void* a, const void* b) {
  uint16_t aa = *(const uint16_t*)a;
  uint16_t bb = *(const uint16_t*)b;
  if (aa < bb) return -1;
  if (aa > bb) return 1;
  return 0;
}

static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static uint32_t readAdcMilliVoltsStable() {
  pinMode(BAT_ADC_CTRL, OUTPUT);
  digitalWrite(BAT_ADC_CTRL, LOW);
  delay(12);

  (void)analogReadMilliVolts(BAT_ADC_IN);
  delay(3);
  (void)analogReadMilliVolts(BAT_ADC_IN);
  delay(3);

  // 11 samples, drop 2 low + 2 high, average 7 — accurate enough, ~20ms faster
  const int N = 11;
  uint16_t vals[N];
  for (int i = 0; i < N; i++) {
    vals[i] = (uint16_t)analogReadMilliVolts(BAT_ADC_IN);
    delay(2);
  }

  pinMode(BAT_ADC_CTRL, INPUT);
  qsort(vals, N, sizeof(vals[0]), cmpUint16);

  uint32_t sum = 0;
  for (int i = 2; i < (N - 2); i++) sum += vals[i];
  return sum / (uint32_t)(N - 4);
}

static float readBatteryVoltageRaw() {
  uint32_t mv = readAdcMilliVoltsStable();
  float v = ((float)mv / 1000.0f) * 2.0f;
  v *= g_battery.calibrationFactor;
  return v;
}

static int batteryPercentFromOCV(float v) {
  struct BatPoint { float v; int pct; };
  static const BatPoint lut[] = {
    {4.20f, 100}, {4.15f, 95}, {4.11f, 90}, {4.08f, 85},
    {4.05f, 80},  {4.02f, 75}, {3.99f, 70}, {3.96f, 62},
    {3.93f, 55},  {3.90f, 48}, {3.87f, 40}, {3.84f, 32},
    {3.81f, 24},  {3.78f, 18}, {3.75f, 13}, {3.72f, 9},
    {3.69f, 6},   {3.65f, 4},  {3.55f, 2},  {3.40f, 0}
  };

  if (v >= lut[0].v) return 100;
  const int n = (int)(sizeof(lut) / sizeof(lut[0]));
  if (v <= lut[n - 1].v) return 0;

  for (int i = 0; i < n - 1; i++) {
    float vHi = lut[i].v;
    float vLo = lut[i + 1].v;
    int pHi = lut[i].pct;
    int pLo = lut[i + 1].pct;
    if (v <= vHi && v >= vLo) {
      float t = (v - vLo) / (vHi - vLo);
      int pct = (int)(pLo + t * (float)(pHi - pLo) + 0.5f);
      if (pct < 0) pct = 0;
      if (pct > 100) pct = 100;
      return pct;
    }
  }
  return 0;
}

static void updateBatteryCached(bool force = false) {
  uint32_t now = millis();
  if (!force && (now - g_battery.lastMs) < BAT_CACHE_MS) return;
  g_battery.lastMs = now;

  float raw = readBatteryVoltageRaw();
  bool valid = (raw > 2.8f && raw < 4.5f);
  g_battery.valid = valid;
  if (!valid) return;

  g_battery.rawV = raw;
  if (g_battery.filteredV <= 0.0f) {
    g_battery.filteredV = raw;
  } else {
    const float alpha = 0.22f;
    g_battery.filteredV = (alpha * raw) + ((1.0f - alpha) * g_battery.filteredV);
  }
  g_battery.filteredV = clampf(g_battery.filteredV, 3.0f, 4.25f);
  g_battery.pctRaw = batteryPercentFromOCV(g_battery.filteredV);

  if (force) {
    g_battery.pctShown = g_battery.pctRaw;
  } else {
    if (g_battery.pctRaw < g_battery.pctShown) {
      if ((g_battery.pctShown - g_battery.pctRaw) >= 1) g_battery.pctShown--;
    } else if (g_battery.pctRaw > g_battery.pctShown + 2) {
      g_battery.pctShown++;
    }
  }

  if (g_battery.pctShown < 0) g_battery.pctShown = 0;
  if (g_battery.pctShown > 100) g_battery.pctShown = 100;

  if (!g_battery.low && g_battery.pctShown <= 8) g_battery.low = true;
  else if (g_battery.low && g_battery.pctShown >= 12) g_battery.low = false;
}

static void drawBatteryTopRight() {
  updateBatteryCached(false);

  int pct = g_battery.valid ? g_battery.pctShown : 0;
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;

  const int iconW = 18;
  const int iconH = 9;
  int xIcon = SCREEN_W - MARGIN_X - iconW - 2;
  int yIcon = 2;

  gfx.drawRect(xIcon, yIcon, iconW, iconH, 1);
  gfx.fillRect(xIcon + iconW, yIcon + 2, 2, iconH - 4, 1);

  int innerW = iconW - 2;
  int fillW = (innerW * pct) / 100;
  if (fillW > 0) gfx.fillRect(xIcon + 1, yIcon + 1, fillW, iconH - 2, 1);
  if (g_battery.low && pct > 0) gfx.drawLine(xIcon + 3, yIcon + 2, xIcon + 3, yIcon + iconH - 3, 0);

  u8g2.setFont(u8g2_font_6x10_tf);
  char buf[8];
  if (g_battery.valid) snprintf(buf, sizeof(buf), "%d%%", pct);
  else snprintf(buf, sizeof(buf), "--");
  int wTxt = u8g2.getUTF8Width(buf);
  u8g2.setCursor(xIcon - 4 - wTxt, yIcon + 8);
  u8g2.print(buf);
  u8g2.setFont(MAIN_FONT);
}
#endif

// ============================================================================
//  Drawing primitives
// ============================================================================
static void beginPageCanvas(bool clearMem = true) {
  if (clearMem) display.clearMemory();
  display.landscape();
  u8g2.setFontMode(1);
  u8g2.setForegroundColor(1);
  u8g2.setBackgroundColor(0);
}

static void prepareMenuFrame() {
  bool doFull = (menuDrawsSinceFull >= MENU_FULL_REFRESH_EVERY);
  if (doFull) {
    display.fastmodeOff();
    display.clear();
    menuDrawsSinceFull = 0;
  } else {
    display.fastmodeOn();
  }
  beginPageCanvas();
  menuDrawsSinceFull++;
}

static void drawCenter(const char* a, const char* b = nullptr) {
  display.fastmodeOff();
  display.clear();
  beginPageCanvas();
  u8g2.setFont(MAIN_FONT);

  const int lineH = 16;
  int y = (SCREEN_H / 2) - lineH / 2;
  if (b) y -= lineH / 2;

  int wA = u8g2.getUTF8Width(a);
  u8g2.setCursor((SCREEN_W - wA) / 2, y);
  u8g2.print(a);

  if (b) {
    y += lineH;
    int wB = u8g2.getUTF8Width(b);
    u8g2.setCursor((SCREEN_W - wB) / 2, y);
    u8g2.print(b);
  }
  display.update();
}


static void showToast(const String& msg) {
  g_toast.msg = msg;
  g_toast.untilMs = millis() + TOAST_MS;
}

static void drawToastIfActive() {
  if (g_toast.untilMs == 0) return;
  if ((int32_t)(millis() - g_toast.untilMs) > 0) {
    g_toast.untilMs = 0;
    g_toast.msg = "";
    return;
  }

  const int yTop = SCREEN_H - STATUS_H;
  gfx.fillRect(0, yTop, SCREEN_W, STATUS_H, 0);
  

  u8g2.setFont(u8g2_font_6x10_tf);
  int textY = SCREEN_H - 1;
  u8g2.setCursor(MARGIN_X, textY);
  u8g2.print(g_toast.msg.c_str());
  u8g2.setFont(MAIN_FONT);
}

// ============================================================================
//  Pagination / text layout
// ============================================================================
static void trimTrailingSpaces(String& s) {
  while (s.length() > 0 && s[s.length() - 1] == ' ') {
    s.remove(s.length() - 1);
  }
}

static void trimLeadingSpaces(String& s) {
  while (s.length() > 0 && s[0] == ' ') {
    s.remove(0, 1);
  }
}

static bool lineEndsWithSpace(const String& s) {
  return s.length() > 0 && s[s.length() - 1] == ' ';
}

// ============================================================================
//  Pagination / text layout
// ============================================================================
static uint32_t readPageFromFile(File& f, uint32_t startPos, bool draw, String* outText) {
  f.seek(startPos);
  u8g2.setFont(MAIN_FONT);
  const LayoutMetrics& m = getMetrics();

  int cursorY = TOP_PAD + m.ascent;
  int linesUsed = 0;

  String line;
  String token;
  line.reserve(96);
  token.reserve(48);

  uint32_t lineStartPos = startPos;
  uint32_t tokenStartPos = startPos;

  auto flushLine = [&](const String& toPrint) {
    String printable = toPrint;
    trimTrailingSpaces(printable);

    if (draw) {
      u8g2.setCursor(MARGIN_X, cursorY);
      u8g2.print(printable.c_str());
      cursorY += m.lineH;
    }
    if (outText) {
      String t = printable;
      t.trim();
      (*outText) += t;
      (*outText) += "\n";
    }
    linesUsed++;
  };

  auto safeReturn = [&](uint32_t off) -> uint32_t {
    if (off <= startPos) off = startPos + 1;
    size_t sz = f.size();
    if (sz > 0 && off > sz) off = sz;
    return off;
  };

  auto hardBreakToken = [&](String& t, uint32_t& tStartPos) -> uint32_t {
    while (t.length() > 0) {
      String chunk;
      chunk.reserve(32);
      int i = 0;
      while (i < (int)t.length()) {
        int clen = utf8SafeCharLenAt(t, i);
        if (clen <= 0) break;
        String candidate = chunk + t.substring(i, i + clen);
        if (u8g2.getUTF8Width(candidate.c_str()) > m.maxWidth) break;
        chunk = candidate;
        i += clen;
      }
      if (chunk.length() == 0) {
        int clen = utf8SafeCharLenAt(t, 0);
        if (clen <= 0) clen = 1;
        chunk = t.substring(0, clen);
      }
      flushLine(chunk);
      if (linesUsed >= m.maxLines) return safeReturn(tStartPos + (uint32_t)chunk.length());
      t.remove(0, chunk.length());
      tStartPos += (uint32_t)chunk.length();
    }
    return 0;
  };

  auto appendTokenToLine = [&](String& t, uint32_t tPos) -> uint32_t {
    if (t.length() == 0) return 0;

    if (line.length() == 0) {
      trimLeadingSpaces(t);
      if (t.length() == 0) return 0;
      if (u8g2.getUTF8Width(t.c_str()) > m.maxWidth) {
        return hardBreakToken(t, tPos);
      }
      line = t;
      lineStartPos = tPos;
      t = "";
      return 0;
    }

    trimLeadingSpaces(t);
    if (t.length() == 0) return 0;

    String candidate = line + t;
    if (u8g2.getUTF8Width(candidate.c_str()) > m.maxWidth) {
      trimTrailingSpaces(line);
      flushLine(line);
      if (linesUsed >= m.maxLines) return safeReturn(tPos);

      if (u8g2.getUTF8Width(t.c_str()) > m.maxWidth) {
        return hardBreakToken(t, tPos);
      } else {
        line = t;
        lineStartPos = tPos;
      }
    } else {
      line = candidate;
    }

    t = "";
    return 0;
  };

  while (f.available() && linesUsed < m.maxLines) {
    uint32_t charPos = f.position();
    char c = (char)f.read();
    if (c == '\r') continue;

    String ch;
    ch += c;

    if (c == '\n') {
      uint32_t forcedNext = appendTokenToLine(token, tokenStartPos);
      if (forcedNext != 0) return forcedNext;
      flushLine(line);
      if (linesUsed >= m.maxLines) return safeReturn(f.position());
      line = "";
      lineStartPos = f.position();
      continue;
    }

    if (c == '\t') ch = " ";

    if (isBreakableWhitespaceChar(ch)) {
      uint32_t forcedNext = appendTokenToLine(token, tokenStartPos);
      if (forcedNext != 0) return forcedNext;
      if (line.length() > 0 && !lineEndsWithSpace(line)) line += " ";
      continue;
    }

    if (token.length() == 0) tokenStartPos = charPos;
    token += ch;

    if (isBreakablePunctuationChar(ch)) {
      uint32_t forcedNext = appendTokenToLine(token, tokenStartPos);
      if (forcedNext != 0) return forcedNext;
    }
  }

  uint32_t forcedNext = appendTokenToLine(token, tokenStartPos);
  if (forcedNext != 0) return forcedNext;

  if (linesUsed < m.maxLines && line.length() > 0) {
    trimTrailingSpaces(line);
    flushLine(line);
  }

  return safeReturn(f.position());
}

static uint32_t buildNextOffsetFor(File& f, uint32_t startPos) {
  return readPageFromFile(f, startPos, false, nullptr);
}

static uint32_t buildNextOffset(uint32_t startPos) {
  uint32_t next = readPageFromFile(g_reader.file, startPos, false, nullptr);
  // Use file size instead of available() for reliable EOF detection.
  // available() is unreliable after internal seeks inside readPageFromFile.
  if (next >= (uint32_t)g_reader.file.size()) g_reader.eofReached = true;
  return next;
}

static uint32_t pageOffsetForPage(File& f, const String& path, int page) {
  if (page < 0) page = 0;

  int cachedPage = 0;
  uint32_t cachedOffset = 0;
  if (!lookupOffsetCache(path, page, cachedPage, cachedOffset)) {
    cachedPage = 0;
    cachedOffset = 0;
  }

  uint32_t off = cachedOffset;
  for (int p = cachedPage; p < page; p++) {
    uint32_t next = buildNextOffsetFor(f, off);
    if (next == off) break;
    off = next;
    storeOffsetCache(path, p + 1, off);
  }

  storeOffsetCache(path, page, off);
  return off;
}

static void ensureOffsetsUpTo(int targetPage) {
  if (g_reader.knownPages < 1) {
    g_reader.knownPages = 1;
    g_reader.pageOffsets[0] = 0;
  }

  bool addedOffsets = false;
  while (!g_reader.eofReached && g_reader.knownPages <= targetPage && g_reader.knownPages < MAX_PAGES) {
    uint32_t start = g_reader.pageOffsets[g_reader.knownPages - 1];
    uint32_t next = buildNextOffset(start);
    if (next <= start) {
      g_reader.eofReached = true;
      break;
    }
    g_reader.pageOffsets[g_reader.knownPages] = next;
    storeOffsetCache(g_reader.currentBookPath, g_reader.knownPages, next);
    g_reader.knownPages++;
    addedOffsets = true;
  }

  if (g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;
  if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;

  if (addedOffsets && (g_reader.knownPages % 50 == 0 || g_reader.eofReached)) {
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
  }
}

// ============================================================================
//  Reader open / render
// ============================================================================
static bool openBookByIndex(int idx) {
  safeCloseCurrentBook();
  if (idx < 0 || idx >= g_library.bookCount) return false;

  String path = String(g_library.books[idx].path);
  File f = FS.open(path, "r");
  if (!f || f.isDirectory()) {
    if (f) f.close();
    return false;
  }

  g_reader.file = f;
  g_reader.currentBookKey = prefKeyForBook(path);
  g_reader.currentBookPath = path;
  g_reader.knownPages = 1;
  g_reader.pageOffsets[0] = 0;
  g_reader.eofReached = false;
  loadPageOffsetCacheForBook(path, g_reader.file.size());
  
  // Load page index and byte offset from filesystem
  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  File progressFile = FS.open(progressPath.c_str(), "r");
  uint32_t savedOffset = 0;
  int needsRelocation = 0;
  
  if (progressFile) {
    String pageStr = progressFile.readStringUntil('\n');
    g_reader.pageIndex = pageStr.toInt();
    
    String offsetStr = progressFile.readStringUntil('\n');
    savedOffset = offsetStr.toInt();
    
    String relocateStr = progressFile.readStringUntil('\n');
    needsRelocation = relocateStr.toInt();
    
    progressFile.close();
    if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
    Serial.print("[Progress] Restored page index: ");
    Serial.print(g_reader.pageIndex);
    Serial.print(", offset: ");
    Serial.print(savedOffset);
    Serial.print(", needs relocation: ");
    Serial.println(needsRelocation);
  } else {
    g_reader.pageIndex = 0;
    Serial.println("[Progress] No saved progress, starting at page 0");
  }
  
  // Handle relocation if needed
  if (needsRelocation == 1 && savedOffset > 0) {
    Serial.print("[Progress] Relocating to offset ");
    Serial.println(savedOffset);
    relocateOpenBookToOffset(savedOffset);
  }
  
  g_reader.pageTurnsSinceFull = 0;
  resetSaveThrottle();
  syncWakeState(true);

  storeOffsetCache(path, 0, 0);

  int warmTarget = g_reader.pageIndex + PREFETCH_AHEAD_PAGES;
  if (warmTarget < 1) warmTarget = 1;
  ensureOffsetsUpTo(warmTarget);
  return true;
}

// Relocate to the page containing the given byte offset
// Used when layout changes to preserve reading position
static void relocateOpenBookToOffset(uint32_t targetOffset) {
  Serial.print("[Relocate] Starting relocation to offset ");
  Serial.println(targetOffset);
  
  int foundPage = 0;
  bool found = false;
  
  // Walk through pages to find the one containing the target offset
  for (int i = 0; i < MAX_PAGES && !found; i++) {
    // Ensure offset is computed for this page
    ensureOffsetsUpTo(i + 1);
    
    uint32_t pageStart = g_reader.pageOffsets[i];
    uint32_t pageEnd = (i < MAX_PAGES - 1) ? g_reader.pageOffsets[i + 1] : g_reader.file.size();
    
    // Check if target offset falls within this page
    if (targetOffset >= pageStart && targetOffset < pageEnd) {
      foundPage = i;
      found = true;
      Serial.print("[Relocate] Found offset on page ");
      Serial.println(foundPage);
      break;
    }
    
    // Yield periodically to avoid watchdog timeout on large books
    if (i % 10 == 0) {
      yield();
    }
  }
  
  if (found) {
    g_reader.pageIndex = foundPage;
    Serial.print("[Relocate] Relocated to page ");
    Serial.println(foundPage);
  } else {
    Serial.println("[Relocate] Could not find offset, staying at page 0");
    g_reader.pageIndex = 0;
  }
  
  // Clear relocation flag after relocation
  String progressPath = "/books/.progress/" + g_reader.currentBookKey + ".txt";
  File progressFile = FS.open(progressPath.c_str(), "r");
  if (progressFile) {
    String pageStr = progressFile.readStringUntil('\n');
    String offsetStr = progressFile.readStringUntil('\n');
    progressFile.readStringUntil('\n');  // skip old relocation flag
    progressFile.close();
    
    // Rewrite with relocation flag cleared
    FS.remove(progressPath.c_str());
    File f = FS.open(progressPath.c_str(), "w");
    if (f) {
      f.println(pageStr);
      f.println(offsetStr);
      f.println("0");  // clear relocation flag
      f.close();
    }
  }
}

static void drawStatusBar(uint32_t startOffset) {
  size_t total = g_reader.file.size();
  if (total == 0) total = 1;

  int pageTextW = 0;
  if (SHOW_PAGE_NUMBER) {
    u8g2.setFont(PAGE_FONT);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", g_reader.pageIndex + 1);
    pageTextW = u8g2.getUTF8Width(buf);
    u8g2.setCursor(SCREEN_W - MARGIN_X - pageTextW, SCREEN_H - 1);
    u8g2.print(buf);
    u8g2.setFont(MAIN_FONT);
  }

  if (SHOW_PROGRESS_BAR) {
    const int padR = SHOW_PAGE_NUMBER ? (pageTextW + 8) : 0;
    int w = (SCREEN_W - 2 * MARGIN_X) - padR;
    if (w < 40) w = 40;

    int x0 = MARGIN_X;
    int yTop = SCREEN_H - 7;
    int barH = 4;
    int filled = (int)((startOffset * (uint32_t)w) / (uint32_t)total);
    if (filled < 0) filled = 0;
    if (filled > w) filled = w;

    gfx.drawRect(x0, yTop, w, barH, 1);
    if (filled > 0) gfx.fillRect(x0 + 1, yTop + 1, max(0, filled - 2), barH - 2, 1);
  }
}

static void renderCurrentPage() {
  if (!g_reader.file && !reopenCurrentBookIfNeeded()) {
    drawCenter("Open failed", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  if (!g_reader.file || g_reader.file.isDirectory()) {
    drawCenter("Open failed", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  size_t bookSize = g_reader.file.size();
  if (bookSize == 0) {
    drawCenter("Book empty", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  ensureOffsetsUpTo(g_reader.pageIndex);
  if (g_reader.knownPages <= 0) {
    drawCenter("Book empty", "Back to library");
    enterLibraryRoot(true);
    return;
  }

  if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
  if (g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;

  if (g_reader.pageOffsets[g_reader.pageIndex] >= bookSize) {
    g_reader.pageIndex = 0;
    g_reader.knownPages = 1;
    g_reader.pageOffsets[0] = 0;
    g_reader.eofReached = false;
  }

  uint32_t start = g_reader.pageOffsets[g_reader.pageIndex];
  g_reader.lastPageStartOffset = start;
  g_reader.file.seek(start);

  bool doFull = (g_reader.pageTurnsSinceFull >= FULL_REFRESH_EVERY_N_PAGES);
  if (doFull) {
    display.fastmodeOff();
    display.clear();
    g_reader.pageTurnsSinceFull = 0;
  } else {
    display.fastmodeOn();
  }

  beginPageCanvas();
  u8g2.setFont(MAIN_FONT);

  uint32_t nextOff = readPageFromFile(g_reader.file, start, true, nullptr);

  bool toastActive = (g_toast.untilMs != 0) && ((int32_t)(millis() - g_toast.untilMs) <= 0);
  if (toastActive) drawToastIfActive();
  else drawStatusBar(start);
  display.update();
}

// ============================================================================
//  Menu drawing
// ============================================================================
static const int UI_HEADER_TOP = 6;
static const int UI_HEADER_GAP = 6;
static const int UI_LIST_LEFT = MARGIN_X + 4;
static const int UI_DEPTH_INDENT = 10;

static int drawSectionHeader(const char* title) {
  u8g2.setFont(BOLD_FONT);
  int ascent = u8g2.getFontAscent();
  int yTitle = UI_HEADER_TOP + ascent - 2;

  // Library = Pala One, other screens = their own title
  const char* headerText = "Pala One";
  if (title && strcmp(title, "Library") != 0) {
    headerText = title;
  }

  u8g2.setCursor(MARGIN_X, yTitle);
  u8g2.print(headerText);

#if HAS_BATTERY
  drawBatteryTopRight();
#endif

  int lineY = yTitle + 4;
  gfx.drawFastHLine(MARGIN_X, lineY, SCREEN_W - (MARGIN_X * 2), 1);

  int contentTop = lineY + UI_HEADER_GAP + 11;

  u8g2.setFont(MAIN_FONT);
  return contentTop;
}

static void drawMenuBulletRow(int yBaseline, const String& label, bool selected, bool boldText = false, int depth = 0, bool systemItem = false) {
  int textX = UI_LIST_LEFT + (depth * UI_DEPTH_INDENT);
  if (systemItem) textX += 2;

  u8g2.setForegroundColor(1);
  u8g2.setFont(boldText ? BOLD_FONT : MAIN_FONT);
  u8g2.setCursor(textX, yBaseline);
  u8g2.print(label.c_str());
  u8g2.setFont(MAIN_FONT);
}

static void splitListLabelForDisplay(const String& in, int maxWidth, String& line1, String& line2) {
  line1 = in;
  line2 = "";
  if (u8g2.getUTF8Width(in.c_str()) <= maxWidth) return;

  int bestBreak = -1;
  for (int i = 0; i < (int)in.length(); i++) {
    if (in[i] != ' ') continue;
    String left = in.substring(0, i);
    left.trim();
    if (left.length() == 0) continue;
    if (u8g2.getUTF8Width(left.c_str()) <= maxWidth) bestBreak = i;
    else break;
  }

  if (bestBreak < 0) {
    for (int i = 1; i < (int)in.length(); i++) {
      String left = in.substring(0, i);
      if (u8g2.getUTF8Width(left.c_str()) > maxWidth) {
        bestBreak = max(1, i - 1);
        break;
      }
    }
  }

  if (bestBreak < 0) return;

  line1 = in.substring(0, bestBreak);
  line1.trim();
  line2 = in.substring(bestBreak);
  line2.trim();

  while (line2.length() > 0 && u8g2.getUTF8Width(line2.c_str()) > maxWidth) {
    line2.remove(line2.length() - 1);
  }
}


static void drawLibrary() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  buildLibraryEntries();

  int ascent = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Library");

  int totalItems = g_library.entryCount;
  int visible = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 3) visible = 3;
  if (visible > 6) visible = 6;

  int top = g_library.selectedItem - (visible / 2);
  if (top < 0) top = 0;
  if (top > totalItems - visible) top = max(0, totalItems - visible);

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= totalItems) break;

    String label = libraryEntryLabel(idx);
    bool isSystem = (g_library.entryTypes[idx] == LIB_ENTRY_BOOKMARKS ||
                     g_library.entryTypes[idx] == LIB_ENTRY_LIST ||
                     g_library.entryTypes[idx] == LIB_ENTRY_ABOUT ||
                     g_library.entryTypes[idx] == LIB_ENTRY_APPS );
    bool boldText = (idx == g_library.selectedItem);
    drawMenuBulletRow(y, label, idx == g_library.selectedItem, boldText, g_library.entryDepths[idx], isSystem);
    y += lineH;
  }

  display.update();
}

static void drawListScreen() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("List");

  if (!listHasVisibleItems()) {
    drawMenuBulletRow(y, "No items", true, false, 0, false);
    display.update();
    return;
  }

  if (g_list.selectedIndex < 0) g_list.selectedIndex = 0;
  if (g_list.selectedIndex >= g_list.count) g_list.selectedIndex = g_list.count - 1;

  int visibleRows = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visibleRows < 3) visibleRows = 3;

  int top = g_list.selectedIndex - 2;
  if (top < 0) top = 0;
  if (top > g_list.count - 1) top = max(0, g_list.count - 1);

  int rowsUsed = 0;
  for (int idx = top; idx < g_list.count; idx++) {
    if (rowsUsed >= visibleRows) break;

    String label = String(g_list.items[idx].text);
    bool selected = (idx == g_list.selectedIndex);
    String line1, line2;
    int maxWidth = SCREEN_W - UI_LIST_LEFT - MARGIN_X;

    if (selected) splitListLabelForDisplay(label, maxWidth, line1, line2);
    else {
      line1 = label;
      line2 = "";
      while (line1.length() > 0 && u8g2.getUTF8Width(line1.c_str()) > maxWidth) {
        line1.remove(line1.length() - 1);
      }
    }

    drawMenuBulletRow(y, line1, selected, selected, 0, false);
    if (g_list.items[idx].done) {
      int w1 = u8g2.getUTF8Width(line1.c_str());
      int strikeY1 = y - ((ascent - descent) / 3);
      gfx.drawFastHLine(UI_LIST_LEFT, strikeY1, w1, 1);
    }
    y += lineH;
    rowsUsed++;

    if (selected && line2.length() > 0 && rowsUsed < visibleRows) {
      u8g2.setFont(BOLD_FONT);
      u8g2.setCursor(UI_LIST_LEFT, y);
      u8g2.print(line2.c_str());
      if (g_list.items[idx].done) {
        int w2 = u8g2.getUTF8Width(line2.c_str());
        int strikeY2 = y - ((ascent - descent) / 3);
        gfx.drawFastHLine(UI_LIST_LEFT, strikeY2, w2, 1);
      }
      y += lineH;
      rowsUsed++;
      u8g2.setFont(MAIN_FONT);
    }
  }

  display.update();
}

static void drawAbout() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent = u8g2.getFontAscent();
  int lineH = (ascent - u8g2.getFontDescent()) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Device");

  String rows[5] = {
    "Firmware " FW_VERSION,
    "1x next / down",
    "2x open / select",
    "3x home",
    "Hold bookmark"
  };

  for (int i = 0; i < 5; i++) {
    u8g2.setFont(i == 0 ? BOLD_FONT : MAIN_FONT);
    u8g2.setCursor(MARGIN_X, y);
    u8g2.print(rows[i].c_str());
    y += lineH;
  }

  display.update();
}

static void scanApps() {
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

// ---- PalaAPI wrapper implementations ----------------------------------------

static void api_clearScreen() {
  prepareMenuFrame();
}

static void api_drawHeader(const char* title) {
  drawSectionHeader(title);
}

static void api_drawTextAt(int x, int y, const char* text, int bold) {
  u8g2.setFont(bold ? BOLD_FONT : MAIN_FONT);
  u8g2.setCursor(x, y);
  u8g2.print(text);
}

static void api_drawCenteredLarge(const char* text) {
  u8g2.setFont(u8g2_font_helvB14_te);
  int w   = u8g2.getUTF8Width(text);
  int asc = u8g2.getFontAscent();
  // centre horizontally; vertically centred in the space below y=20 (approx header height)
  u8g2.setCursor((SCREEN_W - w) / 2, (SCREEN_H + 20 + asc) / 2);
  u8g2.print(text);
  u8g2.setFont(MAIN_FONT);
}

static void api_refreshDisplay() {
  display.update();
}

static uint8_t api_waitForEvent() {
  markUserActivity();
  while (true) {
    btns.poll();
    // Long press fires while the button is still held (not on release).
    if (btns.pressArmed && btns.stablePressed) {
      uint32_t now = (uint32_t)(esp_timer_get_time() / 1000ULL);
      if ((uint32_t)(now - btns.pressStart) >= LONG_MS) {
        btns.pressArmed = false;
        btns.pressStart = 0;
        markUserActivity();
        return PALA_LONG;
      }
    }
    if (btns.tripleClick) { btns.resetClicks(); markUserActivity(); return PALA_TRIPLE; }
    if (btns.doubleClick) { btns.resetClicks(); markUserActivity(); return PALA_DOUBLE; }
    if (btns.shortClick)  { btns.resetClicks(); markUserActivity(); return PALA_CLICK; }
    delay(1);
  }
}

static uint8_t api_pollEvent() {
  btns.poll();
  if (btns.longClick)   { btns.resetClicks(); markUserActivity(); return PALA_LONG; }
  if (btns.tripleClick) { btns.resetClicks(); markUserActivity(); return PALA_TRIPLE; }
  if (btns.doubleClick) { btns.resetClicks(); markUserActivity(); return PALA_DOUBLE; }
  if (btns.shortClick)  { btns.resetClicks(); markUserActivity(); return PALA_CLICK; }
  return 0;
}

static uint32_t api_millisNow() {
  return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static int api_buttonPressed() {
  return btns.stablePressed ? 1 : 0;
}

static void api_delayMs(uint32_t ms) {
  delay(ms);
}

static uint32_t api_pendingPresses() {
  btns.poll();
  uint32_t n = btns.rawPressCount;
  btns.rawPressCount = 0;
  return n;
}

static uint32_t api_rtcSeconds() {
  return (uint32_t)(esp_rtc_get_time_us() / 1000000ULL);
}

static int api_storageRead(const char* key, void* buf, int maxlen) {
  char path[64];
  snprintf(path, sizeof(path), "/apps/%s.dat", key);
  File f = LittleFS.open(path, "r");
  if (!f) return -1;
  int n = f.read((uint8_t*)buf, maxlen);
  f.close();
  return n;
}

static int api_storageWrite(const char* key, const void* buf, int len) {
  char path[64];
  snprintf(path, sizeof(path), "/apps/%s.dat", key);
  File f = LittleFS.open(path, "w");
  if (!f) return -1;
  int n = f.write((const uint8_t*)buf, len);
  f.close();
  return n;
}

static int api_snprintf_wrap(char* buf, int len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r = vsnprintf(buf, (size_t)len, fmt, args);
  va_end(args);
  return r;
}

static void initPalaAPI() {
  g_palaAPI.clearScreen       = api_clearScreen;
  g_palaAPI.drawHeader        = api_drawHeader;
  g_palaAPI.drawTextAt        = api_drawTextAt;
  g_palaAPI.drawCenteredLarge = api_drawCenteredLarge;
  g_palaAPI.refreshDisplay    = api_refreshDisplay;
  g_palaAPI.waitForEvent      = api_waitForEvent;
  g_palaAPI.snprintf_wrap     = api_snprintf_wrap;
  g_palaAPI.pollEvent         = api_pollEvent;
  g_palaAPI.millisNow         = api_millisNow;
  g_palaAPI.buttonPressed     = api_buttonPressed;
  g_palaAPI.delayMs           = api_delayMs;
  g_palaAPI.pendingPresses    = api_pendingPresses;
  g_palaAPI.storageRead       = api_storageRead;
  g_palaAPI.storageWrite      = api_storageWrite;
  g_palaAPI.rtcSeconds        = api_rtcSeconds;
}

// ---- App loader -------------------------------------------------------------

static void freeAppExecBuf() {
  if (g_appExecBuf) {
    heap_caps_free(g_appExecBuf);
    g_appExecBuf  = nullptr;
    g_appExecSize = 0;
  }
}

static bool loadAndRunApp(const char* path) {
  freeAppExecBuf();

  File f = FS.open(path, "r");
  if (!f) { drawCenter("App not found", path); delay(1500); return false; }

  size_t fileSize = f.size();
  if (fileSize < sizeof(PalaAppHeader) + 4) {
    f.close(); drawCenter("App too small", "Invalid file"); delay(1500); return false;
  }

  const size_t MAX_APP_BINARY = 48 * 1024;
  if (fileSize > MAX_APP_BINARY) {
    f.close(); drawCenter("App too large", "> 48 KB"); delay(1500); return false;
  }

  g_appExecBuf = heap_caps_malloc(fileSize, MALLOC_CAP_EXEC | MALLOC_CAP_32BIT);
  if (!g_appExecBuf) {
    f.close();
    char msg[32];
    snprintf(msg, sizeof(msg), "Need %u bytes", (unsigned)fileSize);
    drawCenter("No exec memory", msg);
    delay(1500);
    return false;
  }
  g_appExecSize = fileSize;

  // heap_caps_malloc(MALLOC_CAP_EXEC) returns an IRAM instruction-bus address (0x403xxxxx).
  // On ESP32-S3 that address is not writable from the data bus; use the DIRAM DRAM view
  // (same physical SRAM, shifted by SOC_I_D_OFFSET) for all reads and writes.
  uint8_t* dataBuf = (uint8_t*)MAP_IRAM_TO_DRAM((uint32_t)g_appExecBuf);

  size_t bytesRead = f.read(dataBuf, fileSize);
  f.close();
  if (bytesRead != fileSize) {
    freeAppExecBuf(); drawCenter("Read error", "Partial read"); delay(1500); return false;
  }

  PalaAppHeader* hdr = (PalaAppHeader*)dataBuf;

  if (hdr->magic != PALA_APP_MAGIC) {
    freeAppExecBuf(); drawCenter("Bad app file", "Wrong magic"); delay(1500); return false;
  }
  if (hdr->api_version != PALA_API_VERSION) {
    char msg[32];
    snprintf(msg, sizeof(msg), "API v%u, need v%u",
             (unsigned)hdr->api_version, (unsigned)PALA_API_VERSION);
    freeAppExecBuf(); drawCenter("API mismatch", msg); delay(1500); return false;
  }

  if (hdr->entry_offset < sizeof(PalaAppHeader) || hdr->entry_offset >= fileSize) {
    freeAppExecBuf(); drawCenter("Bad entry offset", nullptr); delay(1500); return false;
  }

  // l32r uses the instruction bus (can reach IRAM), but firmware data loads use the data
  // bus (cannot reach IRAM). Relocations must therefore resolve to DRAM addresses so that
  // string/data pointers passed to firmware API functions are data-bus accessible.
  uint32_t base = (uint32_t)dataBuf;
  if (hdr->reloc_count > 0) {
    if (hdr->reloc_offset < sizeof(PalaAppHeader) ||
        hdr->reloc_offset + hdr->reloc_count * 4u > fileSize) {
      freeAppExecBuf(); drawCenter("Bad reloc table", nullptr); delay(1500); return false;
    }
    uint32_t* relocs = (uint32_t*)(dataBuf + hdr->reloc_offset);
    for (uint32_t i = 0; i < hdr->reloc_count; i++) {
      uint32_t off = relocs[i];
      if (off + 4u > hdr->reloc_offset) {
        freeAppExecBuf(); drawCenter("Reloc out of range", nullptr); delay(1500); return false;
      }
      *(uint32_t*)(dataBuf + off) += base;
    }
  }

  pala_app_entry_t entry = (pala_app_entry_t)((uint8_t*)g_appExecBuf + hdr->entry_offset);

  resetInputFrontend();
  entry(&g_palaAPI);

  freeAppExecBuf();
  resetInputFrontend();
  return true;
}

// ---- Apps menu draw / handle ------------------------------------------------

static void drawAppsMenu() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent  = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH   = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Apps");

  if (g_apps.count == 0) {
    drawMenuBulletRow(y, "No apps installed", true, false, 0, false);
    display.update();
    return;
  }

  int visible = max(2, (SCREEN_H - y - BOT_PAD) / lineH);
  int top     = g_apps.selectedIndex - (visible / 2);
  top = max(0, min(top, g_apps.count - visible));

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= g_apps.count) break;
    bool sel = (idx == g_apps.selectedIndex);
    drawMenuBulletRow(y, String(g_apps.apps[idx].name), sel, sel, 0, false);
    y += lineH;
  }

  display.update();
}

static void drawBookmarksBookSelect() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH = (ascent - descent) + g_settings.lineGap + 1;
  int y = drawSectionHeader("Bookmarks");

  if (g_library.bookCount == 0) {
    drawMenuBulletRow(y, "No books", true, false, 0, false);
    display.update();
    return;
  }

  if (g_bookmarkUi.bookIndex < 0) g_bookmarkUi.bookIndex = 0;
  if (g_bookmarkUi.bookIndex >= g_library.bookCount) g_bookmarkUi.bookIndex = g_library.bookCount - 1;

  int visible = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 2) visible = 2;
  if (visible > 6) visible = 6;

  int top = g_bookmarkUi.bookIndex - (visible / 2);
  if (top < 0) top = 0;
  if (top > g_library.bookCount - visible) top = max(0, g_library.bookCount - visible);

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= g_library.bookCount) break;
    drawMenuBulletRow(y, String(g_library.books[idx].name), idx == g_bookmarkUi.bookIndex, idx == g_bookmarkUi.bookIndex, 0, false);
    y += lineH;
  }

  display.update();
}

static void drawBookmarksList() {
  prepareMenuFrame();
  u8g2.setFont(MAIN_FONT);
  int ascent = u8g2.getFontAscent();
  int descent = u8g2.getFontDescent();
  int lineH = (ascent - descent) + g_settings.lineGap;
  int y = drawSectionHeader("Bookmarks");

  String bookPath = String(g_library.books[g_bookmarkUi.bookIndex].path);
  String key = prefKeyForBook(bookPath);
  g_bookmarkUi.count = loadBookmarksForKey(key, g_bookmarkUi.pages, g_bookmarkUi.offsets);
  if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count) g_bookmarkUi.selectedIndex = max(0, (int)g_bookmarkUi.count - 1);

  if (g_bookmarkUi.count == 0) {
    drawMenuBulletRow(y, "No bookmarks", true, false, 0, false);
    display.update();
    return;
  }

  File f = FS.open(bookPath, "r");
  if (!f) {
    drawMenuBulletRow(y, "Open failed", true, false, 0, false);
    display.update();
    return;
  }

  int visible = (SCREEN_H - y - BOT_PAD) / lineH;
  if (visible < 1) visible = 1;
  if (visible > 5) visible = 5;

  int top = g_bookmarkUi.selectedIndex - (visible / 2);
  if (top < 0) top = 0;
  if (top > (int)g_bookmarkUi.count - visible) top = max(0, (int)g_bookmarkUi.count - visible);

  for (int i = 0; i < visible; i++) {
    int idx = top + i;
    if (idx >= (int)g_bookmarkUi.count) break;

    int targetPage = (int)g_bookmarkUi.pages[idx];
    if (targetPage < 0) targetPage = 0;

    uint32_t pageOff = resolveBookmarkOffset(bookPath, (uint16_t)targetPage, g_bookmarkUi.offsets[idx]);
    String sn = readBookmarkLabelAtOffset(f, pageOff, targetPage);
    drawMenuBulletRow(y, sn, idx == g_bookmarkUi.selectedIndex, idx == g_bookmarkUi.selectedIndex, 0, false);
    y += lineH;
  }

  f.close();
  display.update();
}

// ============================================================================
//  Web UI helpers
// ============================================================================
static String webUiStyle() {
  return String(
    "<style>"
    ":root{--bg:#f3efe7;--card:#fff;--line:#ddd4c7;--line-soft:#ece5d9;--text:#1f2328;--muted:#667085;--link:#3c5a7a;--ok:#216e39;--okbg:#e7f6ec;--warn:#8a5a00;--warnbg:#fff4d6;--danger:#6e2a2a}"
    "*{box-sizing:border-box}"
    "body{margin:0;background:var(--bg);color:var(--text);font:15px/1.45 system-ui,sans-serif}"
    ".wrap{max-width:820px;margin:0 auto;padding:18px}"
    ".wide{max-width:1020px}"
    ".top{display:flex;justify-content:space-between;align-items:flex-start;gap:12px;margin-bottom:14px}"
    ".top a,.link{color:var(--link);text-decoration:none}"
    ".top a:hover,.link:hover{text-decoration:underline}"
    ".muted{color:var(--muted);font-size:13px}"
    ".card{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:14px 15px;margin:0 0 14px;box-shadow:0 1px 0 rgba(0,0,0,.03)}"
    ".grid{display:grid;gap:12px}"
    ".actions{display:flex;flex-wrap:wrap;gap:10px;align-items:center;margin-top:14px}"
    ".nav{display:flex;flex-wrap:wrap;gap:10px 14px;font-size:14px}"
    ".nav a{color:var(--link);text-decoration:none}"
    ".list{list-style:none;padding:0;margin:0}"
    ".list li{padding:11px 0;border-top:1px solid var(--line-soft)}"
    ".list li:first-child{border-top:0;padding-top:0}"
    ".row{display:flex;justify-content:space-between;gap:12px;align-items:flex-start}"
    ".meta{color:var(--muted);font-size:13px}"
    ".pill{display:inline-block;background:#f6f2ea;color:#6b6358;border-radius:999px;padding:3px 8px;font-size:12px}"
    ".pre{white-space:pre-wrap;line-height:1.45;padding:12px;border:1px solid var(--line);border-radius:10px;background:#fcfaf7}"
    ".danger{background:var(--danger)}"
    ".banner-ok{background:var(--okbg);color:var(--ok);border:1px solid #cfe9d7;border-radius:12px;padding:12px 13px;margin-bottom:14px}"
    ".banner-warn{background:var(--warnbg);color:var(--warn);border:1px solid #ecd9a3;border-radius:12px;padding:12px 13px;margin-bottom:14px}"
    ".stats{display:grid;gap:10px;grid-template-columns:repeat(2,minmax(0,1fr));margin-top:12px}"
    ".stat{padding:11px 12px;border:1px solid var(--line-soft);border-radius:12px;background:#fcfaf7}"
    ".stat b{display:block;font-size:17px;line-height:1.2;margin-top:2px}"
    ".bar{height:10px;border-radius:999px;background:#ece5d9;overflow:hidden;border:1px solid #e0d7ca;margin-top:12px}"
    ".bar > span{display:block;height:100%;background:#3c5a7a}"
    ".stack{display:grid;gap:8px}"
    ".small{font-size:13px}"
    "button,.btn{display:inline-flex;align-items:center;justify-content:center;border:0;border-radius:10px;background:#1f2328;color:#fff;padding:10px 14px;font:600 14px system-ui,sans-serif;text-decoration:none;cursor:pointer}"
    ".btn.secondary{background:#eef2f6;color:#334e68;border:1px solid #d8e0e8}"
    "input[type=text],input[type=file],select{width:100%;box-sizing:border-box;border:1px solid #c9c2b8;border-radius:10px;background:#fff;padding:10px;font:inherit}"
    "h1,h2,h3,p{margin:0}"
    "h1,h2,h3{margin-bottom:6px}"
    "p + p{margin-top:10px}"
    "@media(min-width:760px){.stats{grid-template-columns:repeat(4,minmax(0,1fr))}}"
    "@media(max-width:640px){.row,.top{flex-direction:column}.wrap{padding:14px}}"
    "</style>"
  );
}

static String webPageStart(const String& title, const String& subtitle, const String& navHtml, bool wide = false) {
  String out;
  out.reserve(1100);
  out = "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>";
  out += title;
  out += "</title>";
  out += webUiStyle();
  out += "</head><body><div class='wrap";
  if (wide) out += " wide";
  out += "'><div class='top'><div><h1>";
  out += title;
  out += "</h1><div class='muted'>";
  out += subtitle;
  out += "</div></div>";
  if (navHtml.length() > 0) {
    out += "<div class='nav'>";
    out += navHtml;
    out += "</div>";
  }
  out += "</div>";
  return out;
}

static String webPageEnd() {
  return String("</div></body></html>");
}

static String htmlEscape(const String& in) {
  String out;
  out.reserve(in.length() + 16);
  for (size_t i = 0; i < in.length(); i++) {
    char c = in[i];
    if (c == '&') out += "&amp;";
    else if (c == '<') out += "&lt;";
    else if (c == '>') out += "&gt;";
    else if (c == '"') out += "&quot;";
    else if (c == '\'') out += "&#39;";
    else out += c;
  }
  return out;
}

static String humanBytes(size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  if (bytes < (1024UL * 1024UL)) return String(bytes / 1024.0f, 1) + " KB";
  return String(bytes / 1024.0f / 1024.0f, 2) + " MB";
}

static int storageUsedPct() {
  size_t totalBytes = fsTotalBytesSafe();
  size_t usedBytes = fsUsedBytesSafe();
  if (totalBytes == 0) return 0;
  int pct = (int)((usedBytes * 100UL) / totalBytes);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return pct;
}

static String storageCardHtml(const char* title = "Storage") {
  size_t totalBytes = fsTotalBytesSafe();
  size_t usedBytes = fsUsedBytesSafe();
  size_t freeBytes = fsFreeBytesSafe();
  int pct = storageUsedPct();

  String out;
  out.reserve(900);
  out += "<div class='card'><h2>";
  out += title;
  out += "</h2><div class='stats'>";
  out += "<div class='stat'><span class='muted'>Books</span><b>" + String(g_library.bookCount) + "</b></div>";
  out += "<div class='stat'><span class='muted'>Used</span><b>" + humanBytes(usedBytes) + "</b></div>";
  out += "<div class='stat'><span class='muted'>Free</span><b>" + humanBytes(freeBytes) + "</b></div>";
  out += "<div class='stat'><span class='muted'>Total</span><b>" + humanBytes(totalBytes) + "</b></div>";
  out += "</div><div class='bar'><span style='width:" + String(pct) + "%'></span></div>";
  out += "<div class='muted' style='margin-top:8px'>" + String(pct) + "% of internal storage currently used.</div></div>";
  return out;
}

static String successPage(const String& title, const String& subtitle, const String& banner, const String& innerHtml) {
  String out = webPageStart(title, subtitle, "<a href='/'>Home</a><a href='/files'>Files</a><a href='/settings'>Settings</a>");
  out += "<div class='banner-ok'>" + banner + "</div>";
  out += innerHtml;
  out += webPageEnd();
  return out;
}

static uint32_t resolveBookmarkOffset(const String& path, uint16_t page, uint32_t storedOffset) {
  File f = FS.open(path, "r");
  if (!f) return 0;

  size_t size = f.size();
  if (storedOffset != 0xFFFFFFFFUL && storedOffset < size) {
    storeOffsetCache(path, page, storedOffset);
    f.close();
    return storedOffset;
  }

  uint32_t off = pageOffsetForPage(f, path, page);
  f.close();
  return off;
}

static String readPageTextForWeb(const String& path, int page) {
  File f = FS.open(path, "r");
  if (!f) return String("Open failed.");
  uint32_t off = pageOffsetForPage(f, path, page);
  String out;
  out.reserve(900);
  (void)readPageFromFile(f, off, false, &out);
  f.close();
  out.trim();
  if (out.length() == 0) out = "(empty)";
  return out;
}

// ============================================================================
//  Sleep handling
// ============================================================================
static void drawSleepScreen() {
  display.fastmodeOff();
  display.clear();
  beginPageCanvas();

  File sf = FS.open("/sleep.bin", "r");
  if (sf && sf.size() >= 3904) {
    static uint8_t sleepBuf[3904];
    sf.read(sleepBuf, 3904);
    sf.close();
    gfx.fillScreen(1);
    gfx.drawXBitmap(0, 0, sleepBuf, SCREEN_W, SCREEN_H, 0);
  } else {
    if (sf) sf.close();
    gfx.fillScreen(1);
    gfx.drawXBitmap(0, 0, pala_one_sleep_black_icon_v4_bits, SCREEN_W, SCREEN_H, 0);
  }
  display.update();
}

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

  btStop();

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
//  Mode handlers
// ============================================================================
static void handleModeAbout() {
  if (btns.shortClick || btns.doubleClick || btns.longClick || btns.quadClick) {
    mode = MODE_LIBRARY;
    drawLibrary();
  }
}

static void handleModeApps() {
  if (btns.shortClick) {
    if (g_apps.count > 0)
      g_apps.selectedIndex = (g_apps.selectedIndex + 1) % g_apps.count;
    drawAppsMenu();
    return;
  }
  if (btns.doubleClick) {
    if (g_apps.count > 0 && g_apps.selectedIndex < g_apps.count) {
      loadAndRunApp(g_apps.apps[g_apps.selectedIndex].path);
      drawAppsMenu();
    }
    return;
  }
  // Triple-click handled globally → library root
}

static void handleModeBookmarkBookSelect() {
  if (btns.tripleClick) {
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  if (g_library.bookCount == 0) {
    if (btns.anyClick()) {
      mode = MODE_LIBRARY;
      drawLibrary();
    }
    return;
  }

  if (btns.shortClick) {
    g_bookmarkUi.bookIndex++;
    if (g_bookmarkUi.bookIndex >= g_library.bookCount) g_bookmarkUi.bookIndex = 0;
    drawBookmarksBookSelect();
    return;
  }

  if (btns.longClick) {
    g_bookmarkUi.bookIndex--;
    if (g_bookmarkUi.bookIndex < 0) g_bookmarkUi.bookIndex = g_library.bookCount - 1;
    drawBookmarksBookSelect();
    return;
  }

  if (btns.doubleClick) {
    g_bookmarkUi.selectedIndex = 0;
    mode = MODE_BM_LIST;
    drawBookmarksList();
  }
}

static void handleModeBookmarkList() {
  // NOTE: g_bookmarkUi.count/.pages/.offsets are loaded once by drawBookmarksList()
  // when entering this mode. No need to reload from Preferences every loop.
  if (!btns.anyClick()) return;

  if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count) g_bookmarkUi.selectedIndex = max(0, (int)g_bookmarkUi.count - 1);

  if (btns.shortClick) {
    if (g_bookmarkUi.count > 0) {
      g_bookmarkUi.selectedIndex++;
      if (g_bookmarkUi.selectedIndex >= (int)g_bookmarkUi.count) g_bookmarkUi.selectedIndex = 0;
    }
    drawBookmarksList();
    return;
  }

  if (btns.doubleClick) {
    if (g_bookmarkUi.count == 0) return;

    String previewPath = String(g_library.books[g_bookmarkUi.bookIndex].path);
    if (g_reader.currentBookPath == previewPath && g_reader.currentBookKey.length() > 0) {
      g_bookmarkUi.previewSavedPage = g_reader.pageIndex;
    } else {
      String previewKey = prefKeyForBook(previewPath);
      g_bookmarkUi.previewSavedPage = prefs.getInt((previewKey + "_p").c_str(), 0);
    }

    if (openBookByIndex(g_bookmarkUi.bookIndex)) {
      g_bookmarkUi.previewActive = true;
      g_reader.pageIndex = (int)g_bookmarkUi.pages[g_bookmarkUi.selectedIndex];
      if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
      mode = MODE_BM_PREVIEW;
      renderCurrentPage();
    } else {
      mode = MODE_LIBRARY;
      drawLibrary();
    }
    return;
  }

  if (btns.tripleClick) {
    // Triple-click = all the way home to library root
    enterLibraryRoot(true);
    markUserActivity();
    return;
  }

  if (btns.longClick) {
    // Long-click = back to book select
    mode = MODE_BM_BOOK_SELECT;
    drawBookmarksBookSelect();
    return;
  }
}

static void handleModeBookmarkPreview() {
  if (btns.tripleClick) {
    // Triple-click = back to bookmark list (where the user came from)
    g_bookmarkUi.previewActive = false;
    safeCloseCurrentBook();
    mode = MODE_BM_LIST;
    drawBookmarksList();
    return;
  }

  if (btns.longClick) {
    // Long-press = accept this bookmark position, continue reading from here
    g_bookmarkUi.previewActive = false;
    saveProgress(true);
    if (g_reader.file) savePageOffsetCacheForBook(g_reader.currentBookPath, g_reader.file.size());
    mode = MODE_READER;
    renderCurrentPage();
    return;
  }

  if (btns.doubleClick) {
    if (g_reader.pageIndex > 0) {
      g_reader.pageIndex--;
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
    }
    return;
  }

  if (btns.shortClick) {
    int oldPage = g_reader.pageIndex;
    g_reader.pageIndex++;
    ensureOffsetsUpTo(g_reader.pageIndex);
    if (g_reader.eofReached && g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;
    if (g_reader.pageIndex != oldPage) {
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
    }
    return;
  }
}

static void handleModeLibrary() {
  if (!btns.anyClick()) return; // nothing to do — don't rebuild entries every loop

  int totalItems = g_library.entryCount;

  if (btns.shortClick) {
    g_library.selectedItem++;
    if (g_library.selectedItem >= totalItems) g_library.selectedItem = 0;
    drawLibrary();
    return;
  }

  if (!btns.doubleClick) return;
  if (g_library.selectedItem < 0 || g_library.selectedItem >= g_library.entryCount) {
    drawLibrary();
    return;
  }

  LibraryEntryType entryType = g_library.entryTypes[g_library.selectedItem];
  int entryRef = g_library.entryRefs[g_library.selectedItem];

  

  if (entryType == LIB_ENTRY_FOLDER) {
    bool expanded = isFolderExpanded(entryRef);
    setFolderExpanded(entryRef, !expanded);
    drawLibrary();
    return;
  }

  if (entryType == LIB_ENTRY_BOOK) {
    if (openBookByIndex(entryRef)) {
      resetPreviewState();
      mode = MODE_READER;
      renderCurrentPage();
    } else {
      drawCenter("Open failed", "Try upload again");
      drawLibrary();
    }
    return;
  }

  if (entryType == LIB_ENTRY_BOOKMARKS) {
    g_bookmarkUi.bookIndex = 0;
    mode = MODE_BM_BOOK_SELECT;
    drawBookmarksBookSelect();
    return;
  }

  if (entryType == LIB_ENTRY_LIST) {
    g_list.selectedIndex = 0;
    mode = MODE_LIST;
    drawListScreen();
    return;
  }

  if (entryType == LIB_ENTRY_ABOUT) {
    mode = MODE_ABOUT;
    drawAbout();
    return;
  }

  if (entryType == LIB_ENTRY_APPS) {
    g_apps.selectedIndex = 0;
    scanApps();
    mode = MODE_APPS;
    drawAppsMenu();
    return;
  }
}

static void handleModeList() {
  if (!listHasVisibleItems()) {
    mode = MODE_LIBRARY;
    drawLibrary();
    return;
  }

  if (btns.shortClick) {
    g_list.selectedIndex++;
    if (g_list.selectedIndex >= g_list.count) g_list.selectedIndex = 0;
    drawListScreen();
    return;
  }

  if (btns.longClick) {
    if (g_list.selectedIndex >= 0 && g_list.selectedIndex < g_list.count) {
      g_list.items[g_list.selectedIndex].done = g_list.items[g_list.selectedIndex].done ? 0 : 1;
      saveListItems();
      drawListScreen();
    }
    return;
  }

  if (btns.doubleClick || btns.tripleClick) {
    mode = MODE_LIBRARY;
    drawLibrary();
    return;
  }
}

static void handleModeReader() {
  if (btns.longClick) {
    const char* msg = addBookmarkForCurrentBook();
    if (msg) showToast(msg);
    g_reader.pageTurnsSinceFull++;
    renderCurrentPage();
    return;
  }

  if (btns.doubleClick) {
    if (g_reader.pageIndex > 0) {
      g_reader.pageIndex--;
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
      saveProgress(false);  // Save after rendering to ensure correct page
    }
    return;
  }

  if (btns.shortClick) {
    int oldPage = g_reader.pageIndex;
    g_reader.pageIndex++;
    ensureOffsetsUpTo(g_reader.pageIndex);
    if (g_reader.eofReached && g_reader.pageIndex >= g_reader.knownPages) g_reader.pageIndex = g_reader.knownPages - 1;
    if (g_reader.pageIndex < 0) g_reader.pageIndex = 0;
    if (g_reader.pageIndex != oldPage) {
      g_reader.pageTurnsSinceFull++;
      renderCurrentPage();
      saveProgress(false);  // Save after rendering to ensure correct page
    }
    return;
  }

  idlePrefetchReader();
}

// ============================================================================
//  Main loop
// ============================================================================
static void idlePrefetchReader() {
  static uint32_t lastIdlePrefetchMs = 0;
  if (mode != MODE_READER) return;
  if (g_bookmarkUi.previewActive) return;
  if (!g_reader.file) return;
  if (g_reader.eofReached) return;
  uint32_t now = millis();
  if ((uint32_t)(now - lastIdlePrefetchMs) < 60) return;
  lastIdlePrefetchMs = now;
  ensureOffsetsUpTo(g_reader.pageIndex + READER_IDLE_PREFETCH_PAGES);
}

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

    // Send JSON in chunks to avoid BLE MTU truncation
    const int maxChunkSize = 200; // Conservative chunk size
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
      delay(50); // Increased delay between chunks
    }

    // Additional delay before sending status to ensure all chunks are delivered
    delay(100);

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
