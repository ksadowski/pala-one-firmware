#ifndef PALA_WEBUI_WEB_HELPERS_H
#define PALA_WEBUI_WEB_HELPERS_H

#include <Arduino.h>
#include <stdint.h>

String   webUiStyle();
String   webPageStart(const String& title, const String& subtitle, const String& navHtml, bool wide = false);
String   webPageEnd();
String   htmlEscape(const String& in);
String   humanBytes(size_t bytes);
int      storageUsedPct();
String   storageCardHtml(const char* title = "Storage");
String   successPage(const String& title, const String& subtitle, const String& banner, const String& innerHtml);
uint32_t resolveBookmarkOffset(const String& path, uint16_t page, uint32_t storedOffset);
String   readPageTextForWeb(const String& path, int page);

#endif // PALA_WEBUI_WEB_HELPERS_H
