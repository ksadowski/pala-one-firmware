#include "src/settings/settings.h"
#include "src/config.h"
#include "src/state.h"
#include "src/fonts.h"

// Helper to get font pointer based on family, size, and weight
const uint8_t* getMainFont(int fam, int sz, int wt) {
  if (fam == 0) {  // Atkinson
    if (wt == 0) {
      if (sz == 12) return u8g2_font_atkinson_regular_12;
      if (sz == 14) return u8g2_font_atkinson_regular_14;
      if (sz == 16) return u8g2_font_atkinson_regular_16;
      if (sz == 18) return u8g2_font_atkinson_regular_18;
      if (sz == 20) return u8g2_font_atkinson_regular_20;
    } else if (wt == 1) {
      if (sz == 12) return u8g2_font_atkinson_medium_12;
      if (sz == 14) return u8g2_font_atkinson_medium_14;
      if (sz == 16) return u8g2_font_atkinson_medium_16;
      if (sz == 18) return u8g2_font_atkinson_medium_18;
      if (sz == 20) return u8g2_font_atkinson_medium_20;
    } else if (wt == 2) {
      if (sz == 12) return u8g2_font_atkinson_semibold_12;
      if (sz == 14) return u8g2_font_atkinson_semibold_14;
      if (sz == 16) return u8g2_font_atkinson_semibold_16;
      if (sz == 18) return u8g2_font_atkinson_semibold_18;
      if (sz == 20) return u8g2_font_atkinson_semibold_20;
    } else if (wt == 3) {
      if (sz == 12) return u8g2_font_atkinson_bold_12;
      if (sz == 14) return u8g2_font_atkinson_bold_14;
      if (sz == 16) return u8g2_font_atkinson_bold_16;
      if (sz == 18) return u8g2_font_atkinson_bold_18;
      if (sz == 20) return u8g2_font_atkinson_bold_20;
    } else if (wt == 4) {
      if (sz == 12) return u8g2_font_atkinson_extrabold_12;
      if (sz == 14) return u8g2_font_atkinson_extrabold_14;
      if (sz == 16) return u8g2_font_atkinson_extrabold_16;
      if (sz == 18) return u8g2_font_atkinson_extrabold_18;
      if (sz == 20) return u8g2_font_atkinson_extrabold_20;
    }
  } else if (fam == 1) {  // Literata
    if (wt == 0) {
      if (sz == 12) return u8g2_font_literata_regular_12;
      if (sz == 14) return u8g2_font_literata_regular_14;
      if (sz == 16) return u8g2_font_literata_regular_16;
      if (sz == 18) return u8g2_font_literata_regular_18;
      if (sz == 20) return u8g2_font_literata_regular_20;
    } else if (wt == 1) {
      if (sz == 12) return u8g2_font_literata_medium_12;
      if (sz == 14) return u8g2_font_literata_medium_14;
      if (sz == 16) return u8g2_font_literata_medium_16;
      if (sz == 18) return u8g2_font_literata_medium_18;
      if (sz == 20) return u8g2_font_literata_medium_20;
    } else if (wt == 2) {
      if (sz == 12) return u8g2_font_literata_semibold_12;
      if (sz == 14) return u8g2_font_literata_semibold_14;
      if (sz == 16) return u8g2_font_literata_semibold_16;
      if (sz == 18) return u8g2_font_literata_semibold_18;
      if (sz == 20) return u8g2_font_literata_semibold_20;
    } else if (wt == 3) {
      if (sz == 12) return u8g2_font_literata_bold_12;
      if (sz == 14) return u8g2_font_literata_bold_14;
      if (sz == 16) return u8g2_font_literata_bold_16;
      if (sz == 18) return u8g2_font_literata_bold_18;
      if (sz == 20) return u8g2_font_literata_bold_20;
    } else if (wt == 4) {
      if (sz == 12) return u8g2_font_literata_extrabold_12;
      if (sz == 14) return u8g2_font_literata_extrabold_14;
      if (sz == 16) return u8g2_font_literata_extrabold_16;
      if (sz == 18) return u8g2_font_literata_extrabold_18;
      if (sz == 20) return u8g2_font_literata_extrabold_20;
    }
  } else if (fam == 2) {  // Noto Sans
    if (wt == 0) {
      if (sz == 12) return u8g2_font_notosans_regular_12;
      if (sz == 14) return u8g2_font_notosans_regular_14;
      if (sz == 16) return u8g2_font_notosans_regular_16;
      if (sz == 18) return u8g2_font_notosans_regular_18;
      if (sz == 20) return u8g2_font_notosans_regular_20;
    } else if (wt == 1) {
      if (sz == 12) return u8g2_font_notosans_medium_12;
      if (sz == 14) return u8g2_font_notosans_medium_14;
      if (sz == 16) return u8g2_font_notosans_medium_16;
      if (sz == 18) return u8g2_font_notosans_medium_18;
      if (sz == 20) return u8g2_font_notosans_medium_20;
    } else if (wt == 2) {
      if (sz == 12) return u8g2_font_notosans_semibold_12;
      if (sz == 14) return u8g2_font_notosans_semibold_14;
      if (sz == 16) return u8g2_font_notosans_semibold_16;
      if (sz == 18) return u8g2_font_notosans_semibold_18;
      if (sz == 20) return u8g2_font_notosans_semibold_20;
    } else if (wt == 3) {
      if (sz == 12) return u8g2_font_notosans_bold_12;
      if (sz == 14) return u8g2_font_notosans_bold_14;
      if (sz == 16) return u8g2_font_notosans_bold_16;
      if (sz == 18) return u8g2_font_notosans_bold_18;
      if (sz == 20) return u8g2_font_notosans_bold_20;
    } else if (wt == 4) {
      if (sz == 12) return u8g2_font_notosans_extrabold_12;
      if (sz == 14) return u8g2_font_notosans_extrabold_14;
      if (sz == 16) return u8g2_font_notosans_extrabold_16;
      if (sz == 18) return u8g2_font_notosans_extrabold_18;
      if (sz == 20) return u8g2_font_notosans_extrabold_20;
    }
  } else if (fam == 3) {  // Noto Serif
    if (wt == 0) {
      if (sz == 12) return u8g2_font_notoserif_regular_12;
      if (sz == 14) return u8g2_font_notoserif_regular_14;
      if (sz == 16) return u8g2_font_notoserif_regular_16;
      if (sz == 18) return u8g2_font_notoserif_regular_18;
      if (sz == 20) return u8g2_font_notoserif_regular_20;
    } else if (wt == 1) {
      if (sz == 12) return u8g2_font_notoserif_medium_12;
      if (sz == 14) return u8g2_font_notoserif_medium_14;
      if (sz == 16) return u8g2_font_notoserif_medium_16;
      if (sz == 18) return u8g2_font_notoserif_medium_18;
      if (sz == 20) return u8g2_font_notoserif_medium_20;
    } else if (wt == 2) {
      if (sz == 12) return u8g2_font_notoserif_semibold_12;
      if (sz == 14) return u8g2_font_notoserif_semibold_14;
      if (sz == 16) return u8g2_font_notoserif_semibold_16;
      if (sz == 18) return u8g2_font_notoserif_semibold_18;
      if (sz == 20) return u8g2_font_notoserif_semibold_20;
    } else if (wt == 3) {
      if (sz == 12) return u8g2_font_notoserif_bold_12;
      if (sz == 14) return u8g2_font_notoserif_bold_14;
      if (sz == 16) return u8g2_font_notoserif_bold_16;
      if (sz == 18) return u8g2_font_notoserif_bold_18;
      if (sz == 20) return u8g2_font_notoserif_bold_20;
    } else if (wt == 4) {
      if (sz == 12) return u8g2_font_notoserif_extrabold_12;
      if (sz == 14) return u8g2_font_notoserif_extrabold_14;
      if (sz == 16) return u8g2_font_notoserif_extrabold_16;
      if (sz == 18) return u8g2_font_notoserif_extrabold_18;
      if (sz == 20) return u8g2_font_notoserif_extrabold_20;
    }
  }
  if (fam == 0) return u8g2_font_atkinson_regular_12;
  if (fam == 1) return u8g2_font_literata_regular_12;
  if (fam == 2) return u8g2_font_notosans_regular_12;
  return u8g2_font_notoserif_regular_12;
}

// Helper to get bold font pointer based on family and size
const uint8_t* getBoldFont(int fam, int sz) {
  if (fam == 0) {
    if (sz == 12) return u8g2_font_atkinson_extrabold_12;
    if (sz == 14) return u8g2_font_atkinson_extrabold_14;
    if (sz == 16) return u8g2_font_atkinson_extrabold_16;
    if (sz == 18) return u8g2_font_atkinson_extrabold_18;
    if (sz == 20) return u8g2_font_atkinson_extrabold_20;
  } else if (fam == 1) {
    if (sz == 12) return u8g2_font_literata_extrabold_12;
    if (sz == 14) return u8g2_font_literata_extrabold_14;
    if (sz == 16) return u8g2_font_literata_extrabold_16;
    if (sz == 18) return u8g2_font_literata_extrabold_18;
    if (sz == 20) return u8g2_font_literata_extrabold_20;
  } else if (fam == 2) {
    if (sz == 12) return u8g2_font_notosans_extrabold_12;
    if (sz == 14) return u8g2_font_notosans_extrabold_14;
    if (sz == 16) return u8g2_font_notosans_extrabold_16;
    if (sz == 18) return u8g2_font_notosans_extrabold_18;
    if (sz == 20) return u8g2_font_notosans_extrabold_20;
  } else if (fam == 3) {
    if (sz == 12) return u8g2_font_notoserif_extrabold_12;
    if (sz == 14) return u8g2_font_notoserif_extrabold_14;
    if (sz == 16) return u8g2_font_notoserif_extrabold_16;
    if (sz == 18) return u8g2_font_notoserif_extrabold_18;
    if (sz == 20) return u8g2_font_notoserif_extrabold_20;
  }
  return u8g2_font_atkinson_extrabold_12;
}

void invalidateMetrics() {
  g_metricsValid = false;
}

const LayoutMetrics& getMetrics() {
  if (!g_metricsValid) {
    u8g2.setFont(MAIN_FONT);
    g_metrics.ascent  = u8g2.getFontAscent();
    g_metrics.descent = u8g2.getFontDescent();
    g_metrics.lineH   = (g_metrics.ascent - g_metrics.descent) + g_settings.lineGap;

    int w = SCREEN_W - (MARGIN_X * 2);
    if (w < 50) w = 50;
    g_metrics.maxWidth = w;

    int maxHeight = SCREEN_H - TOP_PAD - BOT_PAD;
    if (SHOW_PROGRESS_BAR || SHOW_PAGE_NUMBER) maxHeight -= STATUS_H;

    g_metrics.maxLines = maxHeight / g_metrics.lineH;
    if (g_metrics.maxLines < 1) g_metrics.maxLines = 1;
    g_metricsValid = true;
  }
  return g_metrics;
}

void applyFontSize(int sz) {
  int family = g_settings.fontFamily;
  int weight = g_settings.fontWeight;

  if (sz != 12 && sz != 14 && sz != 16 && sz != 18 && sz != 20) sz = 12;
  if (family < 0 || family > 3) family = 0;
  if (weight < 0 || weight > 4) weight = 0;

  MAIN_FONT = getMainFont(family, sz, weight);
  BOLD_FONT = getBoldFont(family, sz);

  g_settings.fontSize     = sz;
  g_settings.fontFamily   = family;
  g_settings.fontWeight   = weight;
  invalidateMetrics();
}

void loadSettings() {
  g_settings.fontFamily = prefs.getInt("cfg_font_family", 0);
  if (g_settings.fontFamily < 0 || g_settings.fontFamily > 3) g_settings.fontFamily = 0;

  g_settings.fontWeight = prefs.getInt("cfg_font_weight", 0);
  if (g_settings.fontWeight < 0 || g_settings.fontWeight > 4) g_settings.fontWeight = 0;

  applyFontSize(prefs.getInt("cfg_font", 12));

  g_settings.sleepSecs = (uint32_t)prefs.getInt("cfg_sleep", 120);
  if (g_settings.sleepSecs < 10)   g_settings.sleepSecs = 10;
  if (g_settings.sleepSecs > 3600) g_settings.sleepSecs = 3600;

  g_settings.lineGap = prefs.getInt("cfg_lgap", 0);
  if (g_settings.lineGap < 0) g_settings.lineGap = 0;
  if (g_settings.lineGap > 4) g_settings.lineGap = 4;

  g_settings.screensaverType = prefs.getInt("cfg_screensaver", SCREENSAVER_SLEEP_ICON);
  if (g_settings.screensaverType < 0) g_settings.screensaverType = SCREENSAVER_SLEEP_ICON;
  if (g_settings.screensaverType > 2) g_settings.screensaverType = SCREENSAVER_SLEEP_ICON;

  g_settings.readerLongPressAction = LONGPRESS_BOOKMARK;
  invalidateMetrics();
}

uint32_t sleepAfterMs() {
  return g_settings.sleepSecs * 1000UL;
}
