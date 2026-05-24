#ifndef PALA_HAL_DISPLAY_H
#define PALA_HAL_DISPLAY_H

// ── Board selection: uncomment the line that matches your hardware ──────────
//#define BOARD_V1_1
#define BOARD_V1_2
// ────────────────────────────────────────────────────────────────────────────

#include <heltec-eink-modules.h>
#include <Adafruit_GFX.h>
#include "src/config.h"

#ifdef BOARD_V1_1
  using DisplayType = EInkDisplay_WirelessPaperV1_1;
#else
  using DisplayType = EInkDisplay_WirelessPaperV1_2;
#endif

class HeltecGFXAdapter : public Adafruit_GFX {
public:
  explicit HeltecGFXAdapter(DisplayType& d)
    : Adafruit_GFX(SCREEN_W, SCREEN_H), disp(d) {}

  void drawPixel(int16_t x, int16_t y, uint16_t color) override {
    if (x < 0 || y < 0 || x >= SCREEN_W || y >= SCREEN_H) return;
    uint16_t c = color ? BLACK : WHITE;
    int16_t xx = (SCREEN_W - 1) - x;
    int16_t yy = (SCREEN_H - 1) - y;
    disp.drawPixel(xx, yy, c);
  }

private:
  DisplayType& disp;
};

extern DisplayType      display;
extern HeltecGFXAdapter gfx;

#endif // PALA_HAL_DISPLAY_H
