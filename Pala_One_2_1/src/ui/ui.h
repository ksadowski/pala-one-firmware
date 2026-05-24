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

// ---- Per-screen modules ----
#include "src/ui/screens/about_screen.h"
#include "src/ui/screens/apps_screen.h"
#include "src/ui/screens/bookmarks_screen.h"
#include "src/ui/screens/library_screen.h"
#include "src/ui/screens/list_screen.h"
#include "src/ui/screens/reader_screen.h"
#include "src/ui/screens/sleep_screen.h"

#endif // PALA_UI_UI_H
