#include "src/pure/text_util.h"
#include "src/config.h"

int utf8CharLenFromLead(uint8_t b) {
  if (b < 0x80) return 1;
  if ((b & 0xE0) == 0xC0) return 2;
  if ((b & 0xF0) == 0xE0) return 3;
  if ((b & 0xF8) == 0xF0) return 4;
  return 1;
}

int utf8SafeCharLenAt(const String& s, int index) {
  if (index < 0 || index >= (int)s.length()) return 0;
  uint8_t b0 = (uint8_t)s[index];
  int len = utf8CharLenFromLead(b0);
  if (index + len > (int)s.length()) return 1;
  for (int i = 1; i < len; i++) {
    if (!isUtf8ContinuationByte((uint8_t)s[index + i])) return 1;
  }
  return len;
}

String utf8CharAt(const String& s, int index) {
  int len = utf8SafeCharLenAt(s, index);
  if (len <= 0) return String("");
  return s.substring(index, index + len);
}

bool isBreakableWhitespaceChar(const String& ch) {
  return ch == " " || ch == "\n" || ch == "\t";
}

bool isBreakablePunctuationChar(const String& ch) {
  return ch == "." || ch == "," || ch == ";" || ch == ":" || ch == "!" || ch == "?" ||
         ch == ")" || ch == "]" || ch == "}" || ch == "-" || ch == "/";
}

String normalizeTypography(const String& in) {
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

    if (b0 == 0xC2 && i + 1 < in.length()) {
      uint8_t b1 = (uint8_t)in[i + 1];
      if (b1 == 0xAD) { i += 2; continue; }
      if (b1 == 0xAB || b1 == 0xBB) { out += '"'; i += 2; continue; }
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

String compactText(const String& in) {
  bool lastWasSpace = false;
  int newlineCount = 0;
  return compactText(in, &lastWasSpace, &newlineCount, true, true);
}

String compactText(const String& in,
                   bool* ioLastWasSpace,
                   int* ioNewlineCount,
                   bool trimTail,
                   bool reflowSingleNewlines) {
  String out;
  out.reserve(in.length());

  bool lastWasSpace = ioLastWasSpace ? *ioLastWasSpace : false;
  int newlineCount = ioNewlineCount ? *ioNewlineCount : 0;
  size_t trimAnchor = out.length();

  for (size_t i = 0; i < in.length(); i++) {
    char c = in[i];

    if (c == '\r') continue;
    if (c == '\t') c = ' ';

    if (c == '\n') {
      if (out.length() > trimAnchor) out.remove(trimAnchor);
      newlineCount++;
      if (reflowSingleNewlines) {
        // Defer until we see what follows. A lone newline becomes a space
        // (resolved when the next non-newline arrives, below). The second
        // newline in a run promotes the deferred state to a paragraph break.
        // 3+ in a run are absorbed into the same break.
        if (newlineCount == 2) {
          out += '\n';
          out += '\n';
          trimAnchor = out.length();
        }
      } else {
        if (newlineCount <= 2) {
          out += '\n';
          trimAnchor = out.length();
        }
      }
      lastWasSpace = false;
      continue;
    }

    // Non-newline. If reflow mode left a single \n pending, resolve it as a
    // space now (we know the run was just one). A run of length >=2 already
    // emitted its paragraph break above, so nothing more to do.
    const bool resolveDeferredNewline =
        reflowSingleNewlines && newlineCount == 1;
    newlineCount = 0;

    if (c == ' ') {
      if (!lastWasSpace) {
        out += ' ';
        lastWasSpace = true;
      }
      continue;
    }

    if (resolveDeferredNewline && !lastWasSpace) {
      out += ' ';
      // Don't update trimAnchor — this space should still get stripped if a
      // newline arrives before any non-space character does.
    }

    lastWasSpace = false;
    out += c;
    trimAnchor = out.length();
  }

  if (trimTail) {
    while (out.length() > 0 &&
           (out[out.length() - 1] == ' ' || out[out.length() - 1] == '\n')) {
      out.remove(out.length() - 1);
    }
  }

  if (ioLastWasSpace) *ioLastWasSpace = lastWasSpace;
  if (ioNewlineCount) *ioNewlineCount = newlineCount;

  return out;
}

String readBookmarkLabelAtOffset(File& f, uint32_t off, int page) {
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

void trimTrailingSpaces(String& s) {
  while (s.length() > 0 && s[s.length() - 1] == ' ') {
    s.remove(s.length() - 1);
  }
}

void trimLeadingSpaces(String& s) {
  while (s.length() > 0 && s[0] == ' ') {
    s.remove(0, 1);
  }
}

bool lineEndsWithSpace(const String& s) {
  return s.length() > 0 && s[s.length() - 1] == ' ';
}

void sanitizeListText(String& s) {
  s.replace("\r", "");
  s.replace("\n", " ");
  s.replace("\t", " ");
  while (s.indexOf("  ") != -1) s.replace("  ", " ");
  s.trim();
  if ((int)s.length() > MAX_LIST_TEXT) s = s.substring(0, MAX_LIST_TEXT);
}

String bmKeyFor(const String& bookKey) {
  return bookKey + "_bm";
}
