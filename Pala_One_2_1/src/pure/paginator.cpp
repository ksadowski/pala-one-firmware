#include "src/pure/paginator.h"
#include "src/pure/text_util.h"

// Fixed scratch buffers for the paginator's working state. Sized for
// "any line that could possibly fit on the display" + "any reasonable
// word/URL length" with margin. Stays on the call stack — no heap.
static constexpr size_t kLineMax  = 256;
static constexpr size_t kTokenMax = 512;

// DP paginator memory budget. We use fixed-size arrays instead of std::vector
// to avoid heap allocation on ESP32. The token window is capped to prevent
// stack overflow; paragraphs longer than this are processed in chunks.
// Reduced from 512 to 256 to prevent stack overflow on ESP32-S3.
static constexpr size_t kParagraphHardCap = 256;

// Static arrays to avoid stack overflow in paginatePage
static ScannedToken g_tokens[kParagraphHardCap];
static int g_lastWidth[kParagraphHardCap];
static int g_prefixNonLast[kParagraphHardCap + 1];
static int g_best[kParagraphHardCap + 1];
static uint16_t g_breakPoint[kParagraphHardCap + 1];

namespace {

// One-entry line-plan cache. Populated when the DP stops mid-paragraph
// — the only case where a future page within the same paragraph would
// benefit. Keyed by a layout fingerprint (font-size changes self-
// invalidate), by stream size (book switches self-invalidate), and by
// paragraph byte range (reading off the end self-invalidates).
struct LinePlanCache {
  bool valid = false;
  uint32_t fingerprint = 0;
  uint32_t streamSize = 0;
  uint32_t paragraphStart = 0;
  uint32_t paragraphEnd = 0;
  ScannedToken tokens[kParagraphHardCap];
  int tokenCount = 0;
  uint16_t breakPoint[kParagraphHardCap + 1];
};

LinePlanCache g_planCache;

// Mix maxWidth with three measure() probes that shift when the active
// font face/size changes. We don't probe lineH because the line-break
// plan is independent of line gap — gap only affects vertical layout,
// not where breaks land.
uint32_t computeLayoutFingerprint(const MeasureFn& measure, int maxWidth) {
  uint32_t f = (uint32_t)maxWidth;
  f = f * 1000003u + (uint32_t)measure(" ");
  f = f * 1000003u + (uint32_t)measure("M");
  f = f * 1000003u + (uint32_t)measure("xgpy");
  return f;
}

// Binary-search tokens (monotonic startPos) for the entry whose
// startPos equals queryPos.
int findTokenAtPos(const ScannedToken* toks, int count, uint32_t pos) {
  int lo = 0, hi = count;
  while (lo < hi) {
    int mid = (lo + hi) >> 1;
    if (toks[mid].startPos < pos) lo = mid + 1;
    else hi = mid;
  }
  if (lo < count && toks[lo].startPos == pos) return lo;
  return -1;
}

// Split a single token whose rendered width exceeds maxWidth into UTF-8-
// safe chunks that each fit. Pushes one synthetic ScannedToken per chunk
// to `tokens` (plus corresponding entries to the width arrays),
// so the rest of the DP can treat them as ordinary tokens.
//
// Each non-last chunk gets `end = Punctuation` — that means "no inter-
// word space follows" in the DP's cost calc, which is exactly the right
// semantic between hard-break chunks. The last chunk inherits the parent
// token's `end` so any real terminator (Space / Newline / Eof) survives.
void splitOversizedToken(const ScannedToken& parent,
                         const char* tokenBuf,
                         int maxWidth,
                         int spaceLen,
                         const MeasureFn& measure,
                         ScannedToken* tokens,
                         int* lastWidth,
                         int* prefixNonLast,
                         int* outTokenCount,
                         int maxTokens) {
  char scratch[kTokenMax + 2];
  memcpy(scratch, tokenBuf, parent.len);
  scratch[parent.len] = 0;

  size_t consumed = 0;
  while (consumed < parent.len) {
    // Stop if we've hit the token cap
    if (*outTokenCount >= maxTokens) break;

    const size_t remaining = parent.len - consumed;

    // Find the largest UTF-8-safe prefix that fits.
    size_t fitLen = 0;
    while (fitLen < remaining) {
      int clen = utf8SafeCharLenAt(String(scratch + consumed), fitLen);
      if (clen <= 0) break;
      if (fitLen + (size_t)clen > remaining) break;
      char saved = scratch[consumed + fitLen + clen];
      scratch[consumed + fitLen + clen] = 0;
      const bool fits = measure(scratch + consumed) <= maxWidth;
      scratch[consumed + fitLen + clen] = saved;
      if (!fits) break;
      fitLen += (size_t)clen;
    }

    // Degenerate: even a single character doesn't fit.
    if (fitLen == 0) {
      int clen = utf8SafeCharLenAt(String(scratch + consumed), 0);
      if (clen <= 0) clen = 1;
      if ((size_t)clen > remaining) clen = (int)remaining;
      fitLen = (size_t)clen;
    }

    const bool isLastChunk = (consumed + fitLen) >= parent.len;

    ScannedToken chunk;
    chunk.startPos = parent.startPos + (uint32_t)consumed;
    chunk.len = fitLen;
    chunk.end = isLastChunk ? parent.end : TokenEnd::Punctuation;
    tokens[*outTokenCount] = chunk;

    // lastWidth: chunk on its own.
    char saved = scratch[consumed + fitLen];
    scratch[consumed + fitLen] = 0;
    lastWidth[*outTokenCount] = measure(scratch + consumed);

    // Append the chunk's "non-last" width to the running prefix sum.
    scratch[consumed + fitLen] = ' ';
    scratch[consumed + fitLen + 1] = 0;
    const int nonLast = measure(scratch + consumed) - spaceLen;
    prefixNonLast[*outTokenCount + 1] = prefixNonLast[*outTokenCount] + nonLast;

    scratch[consumed + fitLen] = saved;
    consumed += fitLen;
    (*outTokenCount)++;
  }
}

// Emit lines from `tokens[fromIdx ..]` walking `breakPoint`. Mirrors
// the inter-word-space rule from the DP's cost calc.
int emitFromPlan(File& f,
                 const ScannedToken* tokens,
                 const uint16_t* breakPoint,
                 int fromIdx,
                 int tokenCount,
                 int maxLines,
                 LineCallback onLine,
                 void* userData,
                 int* outNextIdx) {
  char line[kLineMax];
  int linesEmitted = 0;
  int i = fromIdx;
  while (i < tokenCount && linesEmitted < maxLines) {
    int lineEndToken = breakPoint[i];
    size_t lineLen = 0;
    bool hasContent = false;
    for (int j = i; j < lineEndToken; j++) {
      if (hasContent && tokens[j - 1].end == TokenEnd::Space) {
        line[lineLen++] = ' ';
      }
      f.seek(tokens[j].startPos);
      for (size_t k = 0; k < tokens[j].len; k++) {
        int rb = f.read();
        if (rb < 0) break;
        line[lineLen + k] = (char)rb;
      }
      lineLen += tokens[j].len;
      if (tokens[j].len > 0) hasContent = true;
    }
    // Trim trailing inter-word spaces.
    while (lineLen > 0 && line[lineLen - 1] == ' ') lineLen--;
    if (lineLen < kLineMax) line[lineLen] = 0;
    if (onLine) onLine(line, lineLen, userData);
    linesEmitted++;
    i = lineEndToken;
  }
  if (outNextIdx) *outNextIdx = i;
  return linesEmitted;
}

// Shared tail for both the cache-hit and the freshly-DP'd paths.
uint32_t emitAndAdvance(File& f,
                        uint32_t startPos,
                        uint32_t streamSize,
                        uint32_t paragraphEnd,
                        const ScannedToken* tokens,
                        const uint16_t* breakPoint,
                        int tokenCount,
                        int fromIdx,
                        const LayoutMetrics& m,
                        MeasureFn measure,
                        LineCallback onLine,
                        void* userData,
                        int* outNextIdx) {
  int nextIdx = fromIdx;
  const int linesEmitted = emitFromPlan(f, tokens, breakPoint, fromIdx,
                                        tokenCount, m.maxLines, onLine, userData, &nextIdx);
  if (outNextIdx) *outNextIdx = nextIdx;

  // Stopped mid-paragraph (or at the natural end). Resume on the next
  // page at the first un-emitted token's byte position.
  uint32_t nextPos = (nextIdx < tokenCount)
      ? tokens[nextIdx].startPos
      : paragraphEnd;
  if (nextPos <= startPos) nextPos = startPos + 1;
  if (streamSize > 0 && nextPos > streamSize) nextPos = streamSize;
  return nextPos;
}

}  // namespace

uint32_t paginatePage(File& f,
                      uint32_t startPos,
                      const LayoutMetrics& m,
                      MeasureFn measure,
                      LineCallback onLine,
                      void* userData) {
  const uint32_t streamSize = (uint32_t)f.size();
  const uint32_t fp = computeLayoutFingerprint(measure, m.maxWidth);

  // -------- 1. Cache check ------------------------------------------------
  if (g_planCache.valid
      && g_planCache.fingerprint == fp
      && g_planCache.streamSize == streamSize
      && startPos >= g_planCache.paragraphStart
      && startPos <  g_planCache.paragraphEnd) {
    const int idx = findTokenAtPos(g_planCache.tokens, g_planCache.tokenCount, startPos);
    if (idx >= 0) {
      return emitAndAdvance(f, startPos, streamSize,
                            g_planCache.paragraphEnd,
                            g_planCache.tokens, g_planCache.breakPoint,
                            g_planCache.tokenCount, idx, m, measure, onLine, userData, nullptr);
    }
  }

  // -------- 2. Scan + per-token width precompute --------------------------
  f.seek(startPos);
  int tokenCount = 0;
  g_prefixNonLast[0] = 0;

  const int spaceLen = measure(" ");

  uint32_t pos = startPos;
  while (pos < (uint32_t)f.size() && tokenCount < kParagraphHardCap) {
    char tokenBuf[kTokenMax + 2];
    ScannedToken t;
    if (!readNextToken(f, tokenBuf, kTokenMax, t)) break;

    const int tokenWidth = measure(tokenBuf);
    if (tokenWidth > m.maxWidth) {
      splitOversizedToken(t, tokenBuf, m.maxWidth, spaceLen, measure,
                          g_tokens, g_lastWidth, g_prefixNonLast, &tokenCount, kParagraphHardCap);
    } else {
      if (tokenCount >= kParagraphHardCap) break;
      g_lastWidth[tokenCount] = tokenWidth;
      tokenBuf[t.len] = ' ';
      tokenBuf[t.len + 1] = 0;
      const int nonLast = measure(tokenBuf) - spaceLen;
      g_prefixNonLast[tokenCount + 1] = g_prefixNonLast[tokenCount] + nonLast;
      g_tokens[tokenCount] = t;
      tokenCount++;
    }
    pos = f.position();

    // A blank line signals the paragraph end.
    if (t.end == TokenEnd::Newline && tokenCount >= 2 &&
        g_tokens[tokenCount - 2].end == TokenEnd::Newline) {
      break;
    }
  }

  // -------- 3. DP cost calc -----------------------------------------------
  // best[i] = min total badness for laying out tokens[i..end].
  // breakPoint[i] = index of the first token on the line *after* the one
  // that starts at i, in the optimal solution.
  for (int i = 0; i <= tokenCount; i++) {
    g_best[i] = 1000000;  // Large value for "infinity"
    g_breakPoint[i] = 0;
  }
  g_best[0] = 0;
  g_best[tokenCount] = 0;

  for (int i = tokenCount - 1; i >= 0; i--) {
    g_best[i] = 1000000;
    int spaces = 0;
    bool hasContent = false;
    for (int j = i; j < tokenCount; j++) {
      if (hasContent && g_tokens[j - 1].end == TokenEnd::Space) {
        spaces++;
      }

      const int lineWidth =
          (g_prefixNonLast[j] - g_prefixNonLast[i])
          + g_lastWidth[j]
          + spaces * spaceLen;
      if (lineWidth > m.maxWidth) break;
      if (g_tokens[j].len > 0) hasContent = true;

      const int remainingSpace = m.maxWidth - lineWidth;
      const bool isLastLine =
          (j == tokenCount - 1) || (g_tokens[j].end == TokenEnd::Newline);
      const int badness = squaredSlackBadness(remainingSpace, isLastLine);
      if (g_best[i] > badness + g_best[j + 1]) {
        g_best[i] = badness + g_best[j + 1];
        g_breakPoint[i] = (uint16_t)(j + 1);
      }
    }
  }

  // Defensive fallback.
  if (tokenCount > 0 && g_breakPoint[0] == 0) {
    return startPos + 1;
  }

  // -------- 4. Emit + cache -----------------------------------------------
  int nextIdx = 0;
  const uint32_t nextPos = emitAndAdvance(f, startPos, streamSize, pos,
                                          g_tokens, g_breakPoint, tokenCount, 0,
                                          m, measure, onLine, userData, &nextIdx);

  // Cache the plan when tokens are left over.
  if (nextIdx < tokenCount) {
    g_planCache.valid = true;
    g_planCache.fingerprint = fp;
    g_planCache.streamSize = streamSize;
    g_planCache.paragraphStart = startPos;
    g_planCache.paragraphEnd = pos;
    g_planCache.tokenCount = tokenCount;
    memcpy(g_planCache.tokens, g_tokens, sizeof(ScannedToken) * tokenCount);
    memcpy(g_planCache.breakPoint, g_breakPoint, sizeof(uint16_t) * (tokenCount + 1));
  }

  return nextPos;
}
