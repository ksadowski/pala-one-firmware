#ifndef PALA_CONFIG_H
#define PALA_CONFIG_H

#include <stdint.h>

// ============================================================================
//  Firmware / Product constants
// ============================================================================
#define FW_VERSION "2.1"

static const int SCREEN_W = 250;
static const int SCREEN_H = 122;

static const uint8_t MAX_BOOKMARKS      = 12;
static const int     MAX_BOOKS          = 80;
static const int     MAX_FOLDERS        = 32;
static const int     MAX_PAGES          = 10000;
static const int     MAX_LIBRARY_ENTRIES = (MAX_BOOKS * 2) + (MAX_FOLDERS * 2) + 8;
static const int     OFFSET_CACHE_SIZE  = 96;
static const int     MAX_LIST_ITEMS     = 16;
static const int     MAX_LIST_TEXT      = 64;

static const int MAX_APPS     = 16;
static const int MAX_APP_NAME = 32;
static const int MAX_APP_PATH = 80;

static const int PREFETCH_AHEAD_PAGES       = 1;
static const int READER_IDLE_PREFETCH_PAGES = 1;

// ============================================================================
//  BLE Service / Characteristic UUIDs
// ============================================================================
#define PALA_BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define PALA_BLE_CHAR_CMD_UUID       "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define PALA_BLE_CHAR_DATA_UUID      "beb5483f-36e1-4688-b7f5-ea07361b26a8"
#define PALA_BLE_CHAR_STATUS_UUID    "beb54840-36e1-4688-b7f5-ea07361b26a8"

static const uint32_t BLE_CHUNK_SIZE   = 512;        // Safe chunk size for BLE
static const uint32_t BLE_RAM_BUF_SIZE = 16384;      // 16 KB — must be a power of 2

static const uint8_t  BTN_Q                      = 64;
static const uint32_t BTN_QUEUE_RECOVER_THRESHOLD = 10;

// ============================================================================
//  Timing
// ============================================================================
static const uint32_t DOUBLE_MS           = 300;
static const uint32_t TRIPLE_MS           = 550;
static const uint32_t LONG_MS             = 850;
static const uint32_t DEBOUNCE_MS         = 14;
static const uint32_t SAVE_EVERY_MS       = 7000;
static const uint32_t TOAST_MS            = 650;
static const uint32_t UPLOAD_AUTO_EXIT_MS = 15UL * 60UL * 1000UL;
static const uint32_t BAT_CACHE_MS        = 180000;  // 3 min — battery changes slowly

// ============================================================================
//  Hardware pins
// ============================================================================
#define BTN         0
#define HAS_BATTERY 1
#if HAS_BATTERY
  #define BAT_ADC_CTRL 19
  #define BAT_ADC_IN   20
#endif

// ============================================================================
//  UI / display
// ============================================================================
static const int FULL_REFRESH_EVERY_N_PAGES = 100;
static const int MENU_FULL_REFRESH_EVERY    = 60;

static const int MARGIN_X = 6;
static const int TOP_PAD  = 0;
static const int BOT_PAD  = 0;
static const int STATUS_H = 8;

static const bool SHOW_PROGRESS_BAR = true;
static const bool SHOW_PAGE_NUMBER  = true;
static const bool ENABLE_DEEP_SLEEP = true;

#endif // PALA_CONFIG_H
