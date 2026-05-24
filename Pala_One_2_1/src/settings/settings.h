#ifndef PALA_SETTINGS_SETTINGS_H
#define PALA_SETTINGS_SETTINGS_H

#include <stdint.h>
#include "src/state.h"

const uint8_t*       getMainFont(int fam, int sz, int wt);
const uint8_t*       getBoldFont(int fam, int sz);
void                 invalidateMetrics();
const LayoutMetrics& getMetrics();
void                 applyFontSize(int sz);
void                 loadSettings();
uint32_t             sleepAfterMs();

#endif // PALA_SETTINGS_SETTINGS_H
