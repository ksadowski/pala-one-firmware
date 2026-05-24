# BLE Server — `src/ble/`

Implements the Bluetooth Low Energy communication layer for Pala One. The device advertises as **"Pala One"** and exposes a single GATT service with three characteristics through which a companion app can transfer files, manage bookmarks, and configure device settings.

---

## Files

| File | Purpose |
|---|---|
| `ble_server.h` | Public API — `setupBLE()`, `sendBLEStatus()`, `handleBLECommand()` |
| `ble_server.cpp` | Callback classes, command dispatch, setup |

---

## GATT Layout

All UUIDs are defined in `src/config.h`.

```
Service:  4fafc201-1fb5-459e-8fcc-c5c9c331914b  (PALA_BLE_SERVICE_UUID)
│
├── CMD     beb5483e-36e1-4688-b7f5-ea07361b26a8   WRITE
│           Client writes text commands here.
│           Handled by BLECommandCallbacks → handleBLECommand().
│
├── DATA    beb5483f-36e1-4688-b7f5-ea07361b26a8   READ | WRITE | WRITE_NR | NOTIFY
│           Bidirectional bulk data channel.
│           - Client writes raw binary chunks here during upload.
│             Handled by BLEDataCallbacks → ring buffer.
│           - Device notifies JSON payloads or binary chunks here as responses.
│
└── STATUS  beb54840-36e1-4688-b7f5-ea07361b26a8   READ | NOTIFY
            Device notifies short status/ack strings here.
            Written by sendBLEStatus().
```

MTU is negotiated to **517 bytes** at connect time (`BLEDevice::setMTU(517)`).

---

## Callback Classes

### `PalaBLEServerCallbacks`
Connection lifecycle.

| Event | Behaviour |
|---|---|
| `onConnect` | Sets `g_bleConnected = true`, resets sleep timer, sends `"connected"` status, PHY negotiated automatically. |
| `onDisconnect` | Sets `g_bleConnected = false`, sends `"disconnected"` status, restarts advertising. |

### `BLECommandCallbacks`
Handles writes to the CMD characteristic. Resets the sleep timer on every write, then passes the value to `handleBLECommand()`.

### `BLEDataCallbacks`
Handles raw data writes to the DATA characteristic during an active upload (`g_bleTransferState == BLE_TRANSFER_UPLOADING`). Writes chunks into the lock-free ring buffer (see [Upload Flow](#upload-flow)) and immediately ACKs via STATUS.

---

## Command Protocol

Commands are plain UTF-8 strings written to the **CMD characteristic**. Responses are JSON or plain strings notified on **DATA** and/or **STATUS**.

### Query Commands

| Command | Description | DATA response | STATUS response |
|---|---|---|---|
| `LIST_FILES` | List all books in the library | JSON array of `{name, path, size}` | `files_list_sent` |
| `LIST_APPS` | List all `.bin` files under `/apps/` | JSON array of `{name, path, size}` | `apps_list_sent` |
| `GET_SETTINGS` | Read current device settings | JSON object (see below) | `settings_sent` |
| `GET_BOOKMARKS:<path>` | Get all bookmarks for a book file | JSON `{book, bookmarks:[{page, offset, label}]}` | `bookmarks_sent` |
| `GET_ALL_BOOKMARKS` | Sets `g_fetchAllBookmarks` flag (handled in main loop) | — | — |

**GET_SETTINGS JSON fields:**

```json
{
  "fontSize": 16,
  "fontFamily": 0,
  "fontWeight": 2,
  "sleepSecs": 120,
  "lineGap": 1,
  "readerLongPressAction": 0,
  "screensaverType": 0
}
```

---

### Upload Commands

Upload is a three-phase handshake:

```
Client                             Device
  │                                  │
  │── CMD: UPLOAD:<path>:<size> ───►  │  (or UPLOAD_BINARY / UPLOAD_APP)
  │                                  │  [main loop: opens .tmp file]
  │◄── STATUS: upload_ready ─────────│
  │                                  │
  │── DATA: <chunk bytes> ─────────► │  (repeat until all bytes sent)
  │◄── STATUS: ACK:<offset> ─────────│
  │                                  │
  │                                  │  [main loop: flush ring buffer → flash]
  │◄── STATUS: upload_complete ──────│  (or upload_error:...)
```

| Command | Syntax | Max size | Notes |
|---|---|---|---|
| `UPLOAD` | `UPLOAD:<path>:<size>` | < 10 MB | Plain text file (e.g. `.txt` book) |
| `UPLOAD_BINARY` | `UPLOAD_BINARY:<path>:<size>` | < 10 MB | Binary file (e.g. image assets) |
| `UPLOAD_APP` | `UPLOAD_APP:<path>:<size>` | ≤ 48 KB | App `.bin` file, written to `/apps/` |

All three commands only set deferred flags (`g_initUpload`, `g_uploadPath`, `g_uploadSize`, etc.) and return immediately. Actual file opening and finalization happen in the **main loop** to avoid flash I/O inside the BLE ISR context.

**Error statuses:** `upload_error:invalid_size`, `upload_error:invalid_params`, `upload_error:no_space`, `upload_error:cannot_open`

---

### Download Commands

Pull-based chunked download — the client drives pacing.

```
Client                             Device
  │                                  │
  │── CMD: DOWNLOAD:<path> ────────► │
  │◄── STATUS: DOWNLOAD_READY:<size> │
  │                                  │
  │── CMD: DOWNLOAD_NEXT_CHUNK ────► │  (repeat until done)
  │◄── DATA: <chunk bytes> ──────────│
  │◄── STATUS: CHUNK_SENT:<offset>   │  (or download_complete)
```

Chunk size: **512 bytes** (`BLE_CHUNK_SIZE`).

**Error statuses:** `download_error:not_found`, `download_error:cannot_open`, `download_error:read_failed`, `download_error:not_active`

---

### Settings Command

| Command | Syntax | Notes |
|---|---|---|
| `SET_SETTINGS` | `SET_SETTINGS:<json>` | JSON with any subset of settings fields |

**Accepted fields and valid ranges:**

| Field | Type | Valid values |
|---|---|---|
| `fontSize` | int | 12, 14, 16, 18, 20 |
| `fontFamily` | int | 0–3 |
| `fontWeight` | int | 0–4 |
| `sleepSecs` | int | 10–3600 |
| `lineGap` | int | 0–4 |
| `screensaverType` | int | 0–2 |

If any layout-affecting field changes (`fontSize`, `fontFamily`, `fontWeight`, `lineGap`), all page caches are invalidated and the reader reflows on the next page turn. STATUS: `settings_updated`.

---

### Bookmark Commands

| Command | Syntax | STATUS responses |
|---|---|---|
| `ADD_BOOKMARK` | `ADD_BOOKMARK:{"path":"<path>","page":<n>}` | `bookmark_added`, `bookmark_exists`, `bookmarks_full`, `invalid_json` |
| `DELETE_BOOKMARK` | `DELETE_BOOKMARK:{"path":"<path>","page":<n>}` | `bookmark_deleted`, `bookmark_not_found`, `invalid_json` |
| `VIEW_BOOKMARK` | `VIEW_BOOKMARK:<path>:<page>` | — (sets `g_viewBookmark` flag, handled in main loop) |

---

### Navigation Commands

| Command | Description | STATUS response |
|---|---|---|
| `GOTO_BEGINNING` | Jump to the first page of the book (displayed as page 1) | `goto_beginning_ok`, `goto_beginning_error:not_reading` |
| `SEARCH:<phrase>` | Search for phrase in current book and navigate to first match | `search_found:<page>`, `search_not_found`, `search_error:not_reading`, `search_error:page_not_found` |

The search is a linear scan through the file in 1 KB chunks with phrase overlap handling. It returns the page number of the first match. The search is case-sensitive and matches the exact phrase as provided.

---

### Miscellaneous Commands

| Command | Description | STATUS response |
|---|---|---|
| `CANCEL_TRANSFER` | Aborts an active upload or download, cleans up `.tmp` file | `transfer_cancelled`, `no_active_transfer` |
| `DELETE:<path>` | Deletes a file from LittleFS. If a book is open, closes it and switches to library. Triggers library/app reload as appropriate. | `file_delete_success`, `app_delete_success`, `delete_error:not_found`, `delete_error:cannot_remove` |
| *(unknown)* | Any unrecognised command | `unknown_command` |

---

## Upload Flow — Ring Buffer Architecture

Uploads use a **SPSC (single-producer, single-consumer) lock-free ring buffer** to decouple the BLE interrupt context from flash I/O:

```
BLE task (ISR)                        Main loop (Arduino loop())
─────────────────────────────────     ──────────────────────────────────────
BLEDataCallbacks::onWrite()           Every loop() iteration:
  ├─ Write chunk → g_bleRamBuf          ├─ If buffered ≥ 4 KB → flush 4 KB to flash
  ├─ Advance g_bleRamBufHead            ├─ If g_bleAllReceived && buffer empty
  ├─ ACK via STATUS: ACK:<offset>       │    → close file, set g_finalizeUpload
  └─ If all bytes received              └─ If g_finalizeUpload → rename .tmp → final path,
       → set g_bleAllReceived                 run normalisation, reload library
```

**Key constants** (`src/config.h`):

| Constant | Value | Notes |
|---|---|---|
| `BLE_RAM_BUF_SIZE` | 16384 (16 KB) | Must be a power of 2 |
| `BLE_CHUNK_SIZE` | 512 bytes | Max chunk per DATA write |
| `FLUSH_THRESHOLD` | 4096 bytes | Batches flash writes in main loop |

The buffer uses power-of-2 masking (`& (BLE_RAM_BUF_SIZE - 1)`) for wrap-around without modulo. Head and tail are `volatile uint32_t` — they never wrap (32-bit counters used raw), so the used/free calculation `head - tail` is always correct.

---

## Transfer State Machine

```
         ┌──────────────────────────────────────────────────────┐
         │                      IDLE                            │
         └───────┬────────────────────────────┬─────────────────┘
                 │ UPLOAD/UPLOAD_BINARY/       │ DOWNLOAD:<path>
                 │ UPLOAD_APP (main loop)      │
                 ▼                             ▼
         ┌──────────────┐              ┌──────────────────┐
         │  UPLOADING   │              │   DOWNLOADING    │
         │              │              │                  │
         │ DATA writes  │              │ DOWNLOAD_NEXT_   │
         │ → ring buf   │              │ CHUNK pulls      │
         │              │              │ chunks           │
         └──────┬───────┘              └────────┬─────────┘
                │ all received +                │ offset ≥ total
                │ buffer flushed                │ or read error
                ▼                               ▼
         ┌──────────────────────────────────────────────────────┐
         │                      IDLE                            │
         └──────────────────────────────────────────────────────┘
```

`CANCEL_TRANSFER` returns to IDLE from either transfer state.

---

## Global State (declared in `src/state.h`)

| Variable | Type | Description |
|---|---|---|
| `g_bleServer` | `BLEServer*` | BLE server instance |
| `g_bleCharCmd` | `BLECharacteristic*` | CMD characteristic |
| `g_bleCharData` | `BLECharacteristic*` | DATA characteristic |
| `g_bleCharStatus` | `BLECharacteristic*` | STATUS characteristic |
| `g_bleConnected` | `bool` | True while a central is connected |
| `g_bleTransferState` | `BLETransferState` | `IDLE / UPLOADING / DOWNLOADING` |
| `g_bleTransferFile` | `File` | Open file handle for active transfer |
| `g_bleTransferPath` | `String` | Destination path of active transfer |
| `g_bleTransferOffset` | `uint32_t` | Bytes transferred so far |
| `g_bleTransferTotalSize` | `uint32_t` | Expected total bytes |
| `g_bleTransferBytesSent` | `uint32_t` | Bytes confirmed sent (download) |
| `g_bleRamBuf[16384]` | `uint8_t[]` | Upload ring buffer |
| `g_bleRamBufHead` | `volatile uint32_t` | Producer index (BLE task) |
| `g_bleRamBufTail` | `volatile uint32_t` | Consumer index (main loop) |
| `g_bleAllReceived` | `volatile bool` | Set when last upload chunk arrives |
| `g_finalizeUpload` | `bool` | Set when ring buffer fully flushed |
| `g_initUpload` | `bool` | Deferred flag: open file in main loop |
| `g_uploadPath` | `String` | Path from UPLOAD command |
| `g_uploadSize` | `uint32_t` | Size from UPLOAD command |
| `g_uploadIsApp` | `bool` | True for `UPLOAD_APP` |
| `g_uploadIsBinary` | `bool` | True for `UPLOAD_BINARY` |
| `g_viewBookmark` | `bool` | Deferred flag: navigate to bookmark page |
| `g_viewBookmarkPath` | `String` | Book path for deferred navigation |
| `g_viewBookmarkPage` | `int` | Page number for deferred navigation |

---

## Shutdown / Sleep

BLE is shut down in `goToSleep()` (`Pala_One_2_1.ino`) before the device enters deep sleep:

```cpp
if (g_bleServer) {
    g_bleServer->getAdvertising()->stop();
}
```

Only advertising is stopped explicitly. `BLEDevice::deinit()` / `btStop()` are **not** called — `btStop()` was found to cause heap corruption. Deep sleep automatically deinitializes all peripherals including BLE at the hardware level, so a full software teardown is unnecessary.

There is no runtime BLE disable path outside of sleep.

---

## Public API

```cpp
// Initialize BLE device, service, characteristics and start advertising.
// Call once from setup().
void setupBLE();

// Notify msg on the STATUS characteristic if connected.
// Safe to call from any context.
void sendBLEStatus(const char* msg);

// Dispatch a command string received on the CMD characteristic.
// Called from BLECommandCallbacks::onWrite() on the BLE task.
void handleBLECommand(std::string cmd);
```
