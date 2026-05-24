#ifndef PALA_STATE_H
#define PALA_STATE_H

#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>
#include <Adafruit_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>
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

#include "src/config.h"
#include "src/hal/display.h"
#include "pala_api.h"
#include "pala_app.h"

#define FS LittleFS

// ============================================================================
//  Enums
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

enum BLETransferState {
  BLE_TRANSFER_IDLE,
  BLE_TRANSFER_UPLOADING,
  BLE_TRANSFER_DOWNLOADING
};

// ============================================================================
//  Structs
// ============================================================================
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

  uint32_t rawPressCount = 0;  // every short press-release, unfiltered by multi-click windows

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
//  Global variable declarations
// ============================================================================
extern Preferences           prefs;
extern U8G2_FOR_ADAFRUIT_GFX u8g2;

extern const uint8_t* PAGE_FONT;
extern const uint8_t* MAIN_FONT;
extern const uint8_t* BOLD_FONT;

extern Mode            mode;
extern RuntimeSettings g_settings;
extern LibraryState    g_library;
extern ReaderState     g_reader;
extern BookmarkUiState g_bookmarkUi;
extern ToastState      g_toast;
extern ListState       g_list;
extern UploadState     g_upload;
extern BatteryState    g_battery;
extern ButtonState     btns;

extern OffsetCacheEntry g_offsetCache[OFFSET_CACHE_SIZE];
extern uint32_t         g_offsetCacheStamp;

extern uint32_t lastUserActionMs;
extern int      menuDrawsSinceFull;

extern AppsState g_apps;
extern PalaAPI   g_palaAPI;
extern void*     g_appExecBuf;
extern size_t    g_appExecSize;

extern LayoutMetrics g_metrics;
extern bool          g_metricsValid;

// BLE server objects
extern BLEServer*         g_bleServer;
extern BLECharacteristic* g_bleCharCmd;
extern BLECharacteristic* g_bleCharData;
extern BLECharacteristic* g_bleCharStatus;
extern bool               g_bleConnected;

// Deferred flags
extern bool     g_reloadLibrary;
extern bool     g_switchToLibrary;
extern bool     g_redrawCurrentMode;
extern bool     g_fetchAllBookmarks;
extern bool     g_initUpload;
extern bool     g_uploadIsApp;
extern String   g_uploadPath;
extern uint32_t g_uploadSize;

// BLE transfer state
extern BLETransferState  g_bleTransferState;
extern File              g_bleTransferFile;
extern String            g_bleTransferPath;
extern uint32_t          g_bleTransferOffset;
extern uint32_t          g_bleTransferTotalSize;
extern uint32_t          g_bleTransferBytesSent;
extern bool              g_finalizeUpload;

// BLE ring buffer (upload, SPSC: BLE task produces, main loop consumes)
extern uint8_t           g_bleRamBuf[BLE_RAM_BUF_SIZE];
extern volatile uint32_t g_bleRamBufHead;
extern volatile uint32_t g_bleRamBufTail;
extern volatile bool     g_bleAllReceived;

// Button ISR queue
extern volatile uint8_t  btnQHead;
extern volatile uint8_t  btnQTail;
extern volatile bool     btnQState[BTN_Q];
extern volatile uint32_t btnQTimeMs[BTN_Q];
extern volatile uint32_t g_isrDropCount;

#endif // PALA_STATE_H
