#ifndef PALA_PURE_PAGINATOR_H
#define PALA_PURE_PAGINATOR_H

#include <Arduino.h>
#include <FS.h>
#include <stdint.h>

#include "src/state.h"

// Measure the rendered width (in pixels) of a UTF-8 string under the layout's
// current font. The paginator never sets fonts itself — callers must ensure
// the measurement function is consistent with the metrics they pass in.
using MeasureFn = int (*)(const char* str);

// Called once per emitted line, in order. `buf` is NUL-terminated and
// trailing-spaces-trimmed; `len` is its byte length (excluding NUL). The
// buffer is owned by the paginator and only valid for the duration of the
// call — copy if you need to keep it.
using LineCallback = void (*)(const char* buf, size_t len);

// Greedy pagination engine. Reads bytes from `f` starting at `startPos` and
// emits at most `metrics.maxLines` lines via `onLine`. Returns the absolute
// byte offset where the next page begins.
//
// Implementation: simple greedy line-filling. Tokens are accumulated until
// adding the next token would overflow the line width, at which point the
// line is flushed. Oversized tokens are hard-broken into width-fitting chunks.
// Punctuation is treated as glue (no space before it).
//
// `onLine` may be null (just compute the next offset). `measure` MUST be set.
uint32_t paginatePage(File& f,
                      uint32_t startPos,
                      const LayoutMetrics& metrics,
                      MeasureFn measure,
                      LineCallback onLine);

#endif  // PALA_PURE_PAGINATOR_H
