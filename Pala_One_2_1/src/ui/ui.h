#ifndef PALA_UI_UI_H
#define PALA_UI_UI_H

#include <Arduino.h>

// ---- Drawing primitives ----
#if HAS_BATTERY
void drawBatteryTopRight();
#endif
void beginPageCanvas(bool clearMem = true);
void prepareMenuFrame();
void drawCenter(const char* a, const char* b = nullptr);
void showToast(const String& msg);
void drawToastIfActive();

// ---- Menu drawing ----
int  drawSectionHeader(const char* title);
void drawMenuBulletRow(int yBaseline, const String& label, bool selected,
                       bool boldText = false, int depth = 0, bool systemItem = false);
void drawLibrary();
void drawListScreen();
void drawAbout();
void drawAppsMenu();
void drawBookmarksBookSelect();
void drawBookmarksList();

// ---- Sleep screen ----
void drawSleepScreen();

#endif // PALA_UI_UI_H
