#ifndef PALA_BLE_BLE_SERVER_H
#define PALA_BLE_BLE_SERVER_H

#include <string>

void sendBLEStatus(const char* msg);
void handleBLECommand(std::string cmd);
void setupBLE();

#endif // PALA_BLE_BLE_SERVER_H
