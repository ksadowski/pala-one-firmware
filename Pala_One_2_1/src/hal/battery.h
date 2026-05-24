#ifndef PALA_HAL_BATTERY_H
#define PALA_HAL_BATTERY_H

#include "src/config.h"

#if HAS_BATTERY
void adcSetupOnce();
void updateBatteryCached(bool force = false);
#endif

#endif // PALA_HAL_BATTERY_H
