#ifndef PALA_HAL_INPUT_H
#define PALA_HAL_INPUT_H

#include <Arduino.h>

void IRAM_ATTR btnISR();
void markUserActivity();
void clearButtonQueue();
void resetInputFrontend();

#endif // PALA_HAL_INPUT_H
