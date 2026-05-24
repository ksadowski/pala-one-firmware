# Fonts — `fonts/`

All custom bitmap fonts used by Pala One's e-ink reader. Each file is a C source file containing a single `const uint8_t[]` array in u8g2 format.

---

## Overview

Fonts are compiled directly into the firmware as flash-resident byte arrays. There are **100 font files** covering 4 typeface families × 5 weights × 5 sizes.

### Typeface Families

| Index | Family | Style |
|---|---|---|
| 0 | **Atkinson Hyperlegible Next** | Humanist sans-serif, designed for legibility |
| 1 | **Literata** | Book serif, designed for long-form reading |
| 2 | **Noto Sans** | Universal sans-serif |
| 3 | **Noto Serif** | Universal serif |

### Weights

| Index | Weight | Symbol | Notes |
|---|---|---|---|
| 0 | Regular | `regular` | Default body weight |
| 1 | Medium | `medium` | |
| 2 | Semibold | `semibold` | |
| 3 | Bold | `bold` | |
| 4 | Extrabold | `extrabold` | Also used as the bold emphasis font (`BOLD_FONT`) |

### Sizes

Sizes are in pixels (BDF pixel height, 72 dpi): **12, 14, 16, 18, 20**

---

## File Naming Convention

```
{family}_{weight}_{size}.c
```

Examples:
- `atkinson_regular_16.c` → Atkinson Hyperlegible, regular weight, 16 px
- `literata_semibold_14.c` → Literata, semibold, 14 px
- `noto_sans_bold_20.c` → Noto Sans, bold, 20 px

The array name inside each file follows the u8g2 convention:

```c
const uint8_t u8g2_font_{family}_{weight}_{size}[]
```

---

## Integration

### 1. Declarations — `src/fonts.h`

All 100 arrays are declared as `extern const uint8_t[]`. Any C++ source that needs a font pointer includes this header.

### 2. Definitions — `src/fonts_data.c`

A single C compilation unit (`fonts_data.c`) `#include`s every `.c` file in this directory. It must be compiled as **C** (not C++) to avoid linkage issues — the `U8G2_FONT_SECTION` macro places data in a special flash segment.

```c
#include <stdint.h>

#ifndef U8G2_FONT_SECTION
#define U8G2_FONT_SECTION(name)
#endif

#include "../fonts/atkinson_regular_16.c"
// ...all 100 files
```

### 3. Runtime selection — `src/settings/settings.cpp`

`getMainFont(family, size, weight)` returns the correct font pointer for the current settings. `getBoldFont(family, size)` always returns the extrabold weight for emphasis rendering.

`applyFontSize(sz)` is the entry point called whenever font settings change:

```cpp
void applyFontSize(int sz) {
    MAIN_FONT = getMainFont(g_settings.fontFamily, sz, g_settings.fontWeight);
    BOLD_FONT = getBoldFont(g_settings.fontFamily, sz);
    g_settings.fontSize = sz;
    invalidateMetrics();   // forces LayoutMetrics recalculation on next render
}
```

### 4. Layout metrics — `getMetrics()`

After a font change, `g_metricsValid` is set to `false`. The next call to `getMetrics()` recomputes:

```
ascent   = u8g2.getFontAscent()
descent  = u8g2.getFontDescent()
lineH    = (ascent - descent) + g_settings.lineGap
maxWidth = SCREEN_W - (MARGIN_X * 2)           // 250 - 12 = 238 px
maxLines = (SCREEN_H - TOP_PAD - BOT_PAD - STATUS_H) / lineH
```

`maxLines` controls how many lines the paginator fits per page. Changing `lineGap` (0–4 px) adjusts line height and therefore `maxLines` without changing the font itself.

---

## Adding a New Font

### Prerequisites

```bash
# Install otf2bdf (converts OTF/TTF → BDF bitmap)
brew install otf2bdf   # macOS

# Install bdfconv (part of u8g2 tools, converts BDF → C array)
# Clone u8g2 and build the tool:
git clone https://github.com/olikraus/u8g2
cd u8g2/tools/bdfconv
make
```

### Step 1 — Generate BDF from OTF/TTF

```bash
otf2bdf -r 72 -p {SIZE} -o {family}_{weight}_{size}.bdf MyFont-Weight.otf
```

- `-r 72` — 72 dpi (matches u8g2 convention used by all existing fonts)
- `-p {SIZE}` — pixel size: 12, 14, 16, 18, or 20
- The input file must be the specific weight variant (e.g. `MyFont-Bold.otf` for bold)

Example — generating Atkinson Regular at 16 px:
```bash
otf2bdf -r 72 -p 16 -o atkinson_regular_16.bdf AtkinsonHyperlegibleNext-Regular.otf
```

### Step 2 — Convert BDF to u8g2 C array

```bash
./bdfconv -f 1 -m "32-383" -n u8g2_font_{family}_{weight}_{size} \
    -o {family}_{weight}_{size}.c {family}_{weight}_{size}.bdf
```

| Flag | Meaning |
|---|---|
| `-f 1` | u8g2 format 1 (standard, supports proportional fonts) |
| `-m "32-383"` | Glyph range: ASCII (32–127) + Latin-1 Supplement (128–255) + Latin Extended-A (256–383) |
| `-n` | C array name — must match the `extern` declaration in `src/fonts.h` |
| `-o` | Output `.c` file |

The range `32-383` covers all basic European Latin scripts including Polish, Czech, Croatian, Hungarian, Romanian, and other accented characters. Narrow it to `32-127` if flash space is tight and only ASCII is needed.

Example — full command:
```bash
./bdfconv -f 1 -m "32-383" \
    -n u8g2_font_atkinson_regular_16 \
    -o atkinson_regular_16.c \
    atkinson_regular_16.bdf
```

### Step 3 — Place the file

Copy the generated `.c` file into `Pala_One_2_1/fonts/`.

### Step 4 — Register the array

**`src/fonts.h`** — add the extern declaration:
```c
extern const uint8_t u8g2_font_myfont_regular_16[];
```

**`src/fonts_data.c`** — add the include:
```c
#include "../fonts/myfont_regular_16.c"
```

### Step 5 — Wire up selection

In `src/settings/settings.cpp`, add a new `fam` branch to `getMainFont()` and `getBoldFont()`, increment the valid range check in `applyFontSize()`, and update the `fontFamily` range check in `ble_server.cpp` and anywhere else family indices are validated.

---

## Font File Format

Each `.c` file was produced by `bdfconv` and looks like:

```c
/*
  Fontname: -FreeType-Atkinson Hyperlegible Next-Medium-R-Normal--16-...
  Copyright: ...
  Glyphs: 282/362
  BBX Build Mode: 0
*/
const uint8_t u8g2_font_atkinson_regular_16[4893]
    U8G2_FONT_SECTION("u8g2_font_atkinson_regular_16") = "\32\0\3\2...";
```

- The array length is baked into the declaration — do not edit it manually.
- `U8G2_FONT_SECTION` places the array in the `.rodata` flash segment on ESP32 (saves precious RAM).
- Glyph count in the comment ("282/362") indicates how many requested glyphs were found in the source font.

---

## Current Font Matrix

100 files total: 4 families × 5 weights × 5 sizes.

```
               12px  14px  16px  18px  20px
atkinson
  regular       ✓     ✓     ✓     ✓     ✓
  medium        ✓     ✓     ✓     ✓     ✓
  semibold      ✓     ✓     ✓     ✓     ✓
  bold          ✓     ✓     ✓     ✓     ✓
  extrabold     ✓     ✓     ✓     ✓     ✓

literata        (same matrix)

noto_sans       (same matrix)

noto_serif      (same matrix)
```
