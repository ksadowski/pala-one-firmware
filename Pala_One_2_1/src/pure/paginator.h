#ifndef PALA_PURE_PAGINATOR_H
#define PALA_PURE_PAGINATOR_H

#include <Arduino.h>
#include <FS.h>
#include <stdint.h>

#include "src/state.h"
#include "src/pure/token_reader.h"

// Function that measures the rendered width of a null-terminated string
// in pixels. The caller provides this — typically a thin wrapper around
// u8g2.getUTF8Width(). The returned value must be stable for the same
// input string across the lifetime of a paginatePage call.
using MeasureFn = int (*)(const char* str);

// Callback invoked for each emitted line. The buffer is null-terminated
// and valid only for the duration of the callback — copy if you need to keep it.
// `userData` is passed through from paginatePage for context.
using LineCallback = void (*)(const char* buf, size_t len, void* userData);

// Score for how "bad" a laid-out line is. Smaller is better; 0 = perfect.
// `slack` is `maxWidth - actualLineWidth` in pixels (non-negative; an over-
// flowing line is illegal upstream of this fn). `isLastLine` is true for the
// final line of a paragraph — the optimizer should usually give it a free
// pass, since it's normal for the last line to be short.
//
// Not consumed by the current (greedy) strategy. Reserved for the future
// DP strategy and for harness/diagnostics that want a single comparable
// number across line-breaking algorithms.
using BadnessFn = int (*)(int slack, bool isLastLine);

// Default badness: squared slack, free pass on the last line.
// Penalises one really short line much more than several mildly short ones.
inline int squaredSlackBadness(int slack, bool isLastLine) {
  if (isLastLine) return 0;
  if (slack < 0) return 0;          // shouldn't happen, but defend
  return slack * slack;
}

// Pure pagination engine. Reads bytes from `f` starting at `startPos` and
// emits at most `metrics.maxLines` lines via `onLine`. Returns the absolute
// byte offset where the next page begins.
//
// Implementation: paragraph-level dynamic programming that minimises the
// sum of squared slack across each paragraph's lines (Knuth–Plass-flavoured
// total-fit). Oversized tokens are pre-split into width-fitting chunks
// during the scan; overlong paragraphs are processed in cap-sized token
// windows across consecutive pages. A one-entry line-plan cache keyed on a
// layout fingerprint lets subsequent pages within the same paragraph skip
// the scan and DP entirely.
//
// `onLine` may be null (just compute the next offset). `measure` MUST be set.
// `userData` is passed to the callback for context.
uint32_t paginatePage(File& f,
                      uint32_t startPos,
                      const LayoutMetrics& m,
                      MeasureFn measure,
                      LineCallback onLine,
                      void* userData = nullptr);

#endif  // PALA_PURE_PAGINATOR_H
