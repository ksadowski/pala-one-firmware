#include "src/state.h"

// ============================================================================
//  Global variable definitions
// ============================================================================
Preferences           prefs;
U8G2_FOR_ADAFRUIT_GFX u8g2;

Mode            mode           = MODE_LIBRARY;
RuntimeSettings g_settings;
LibraryState    g_library;
ReaderState     g_reader;
BookmarkUiState g_bookmarkUi;
ToastState      g_toast;
ListState       g_list;
UploadState     g_upload;
BatteryState    g_battery;
ButtonState     btns;

OffsetCacheEntry g_offsetCache[OFFSET_CACHE_SIZE];
uint32_t         g_offsetCacheStamp = 1;

uint32_t lastUserActionMs   = 0;
int      menuDrawsSinceFull = 0;

AppsState g_apps;
PalaAPI   g_palaAPI;
void*     g_appExecBuf  = nullptr;
size_t    g_appExecSize = 0;

LayoutMetrics g_metrics;
bool          g_metricsValid = false;

// BLE server objects
BLEServer*         g_bleServer     = nullptr;
BLECharacteristic* g_bleCharCmd    = nullptr;
BLECharacteristic* g_bleCharData   = nullptr;
BLECharacteristic* g_bleCharStatus = nullptr;
bool               g_bleConnected  = false;

// Deferred flags
bool     g_reloadLibrary     = false;
bool     g_switchToLibrary   = false;
bool     g_redrawCurrentMode = false;
bool     g_fetchAllBookmarks = false;
bool     g_initUpload        = false;
bool     g_uploadIsApp       = false;
String   g_uploadPath;
uint32_t g_uploadSize        = 0;

// BLE transfer state
BLETransferState  g_bleTransferState    = BLE_TRANSFER_IDLE;
File              g_bleTransferFile;
String            g_bleTransferPath;
uint32_t          g_bleTransferOffset   = 0;
uint32_t          g_bleTransferTotalSize = 0;
uint32_t          g_bleTransferBytesSent = 0;
bool              g_finalizeUpload       = false;

// BLE ring buffer (upload, SPSC: BLE task produces, main loop consumes)
uint8_t           g_bleRamBuf[BLE_RAM_BUF_SIZE];
volatile uint32_t g_bleRamBufHead = 0;
volatile uint32_t g_bleRamBufTail = 0;
volatile bool     g_bleAllReceived = false;

// Button ISR queue
volatile uint8_t  btnQHead = 0;
volatile uint8_t  btnQTail = 0;
volatile bool     btnQState[BTN_Q];
volatile uint32_t btnQTimeMs[BTN_Q];
volatile uint32_t g_isrDropCount = 0;
