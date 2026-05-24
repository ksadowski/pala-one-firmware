#include "src/pure/token_reader.h"
#include "src/pure/text_util.h"

bool readNextToken(File& f, char* buf, size_t cap, ScannedToken& out) {
  out.len = 0;
  out.startPos = f.position();
  out.end = TokenEnd::Eof;

  if (cap < 1) return false;  // degenerate caller; no room even for NUL

  bool consumedAny = false;
  while (f.available()) {
    const uint32_t bytePos = f.position();
    int rb = f.read();
    if (rb < 0) break;
    consumedAny = true;
    char c = (char)rb;

    if (c == '\r') {
      // CR is stripped silently (CRLF normalization is upstream of us, but
      // be defensive in case a raw stream slipped through).
      continue;
    }

    if (c == '\n') {
      buf[out.len] = 0;
      out.end = TokenEnd::Newline;
      return true;
    }

    if (isBreakableWhitespaceChar(String(c))) {
      buf[out.len] = 0;
      out.end = TokenEnd::Space;
      return true;
    }

    // We're about to accumulate a non-separator byte. Anchor startPos on
    // the FIRST such byte — empty tokens keep their separator-position
    // startPos set above.
    if (out.len == 0) {
      out.startPos = bytePos;
    }

    // Reserve one byte for the NUL terminator. If the buffer is full, put
    // the byte back so the next call starts cleanly with it.
    if (out.len >= cap - 1) {
      f.seek(bytePos);
      buf[out.len] = 0;
      out.end = TokenEnd::BufferFull;
      return true;
    }

    buf[out.len++] = c;

    if (isBreakablePunctuationChar(String(c))) {
      // Glue any immediately-following punctuation or close-quote bytes onto
      // this token. Without this, multi-punct sequences like `?!`, `:-)`,
      // `...`, or `last?"` tokenize as separate single-byte tokens and any
      // of them can land orphaned on their own line. We don't list `"` in
      // isBreakablePunctuationByte because we never want to break *before*
      // an opening quote — handling the close-quote case here keeps the
      // breakable-punct set unchanged for opening contexts.
      while (f.available() && out.len < cap - 1) {
        uint32_t peekPos = f.position();
        int peek = f.read();
        if (peek < 0) break;
        if (peek == '"' || isBreakablePunctuationChar(String((char)peek))) {
          buf[out.len++] = (char)peek;
          continue;
        }
        f.seek(peekPos);
        break;
      }
      buf[out.len] = 0;
      out.end = TokenEnd::Punctuation;
      return true;
    }
  }

  buf[out.len] = 0;
  out.end = TokenEnd::Eof;
  return consumedAny;
}
