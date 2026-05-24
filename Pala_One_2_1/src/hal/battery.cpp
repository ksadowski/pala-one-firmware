#include "src/hal/battery.h"
#include "src/state.h"

#if HAS_BATTERY

void adcSetupOnce() {
  pinMode(BAT_ADC_IN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(BAT_ADC_IN, ADC_11db);
}

static int cmpUint16(const void* a, const void* b) {
  uint16_t aa = *(const uint16_t*)a;
  uint16_t bb = *(const uint16_t*)b;
  if (aa < bb) return -1;
  if (aa > bb) return 1;
  return 0;
}

static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static uint32_t readAdcMilliVoltsStable() {
  pinMode(BAT_ADC_CTRL, OUTPUT);
  digitalWrite(BAT_ADC_CTRL, LOW);
  delay(12);

  (void)analogReadMilliVolts(BAT_ADC_IN);
  delay(3);
  (void)analogReadMilliVolts(BAT_ADC_IN);
  delay(3);

  // 11 samples, drop 2 low + 2 high, average 7 — accurate enough, ~20ms faster
  const int N = 11;
  uint16_t vals[N];
  for (int i = 0; i < N; i++) {
    vals[i] = (uint16_t)analogReadMilliVolts(BAT_ADC_IN);
    delay(2);
  }

  pinMode(BAT_ADC_CTRL, INPUT);
  qsort(vals, N, sizeof(vals[0]), cmpUint16);

  uint32_t sum = 0;
  for (int i = 2; i < (N - 2); i++) sum += vals[i];
  return sum / (uint32_t)(N - 4);
}

static float readBatteryVoltageRaw() {
  uint32_t mv = readAdcMilliVoltsStable();
  float v = ((float)mv / 1000.0f) * 2.0f;
  v *= g_battery.calibrationFactor;
  return v;
}

static int batteryPercentFromOCV(float v) {
  struct BatPoint { float v; int pct; };
  static const BatPoint lut[] = {
    {4.20f, 100}, {4.15f, 95}, {4.11f, 90}, {4.08f, 85},
    {4.05f, 80},  {4.02f, 75}, {3.99f, 70}, {3.96f, 62},
    {3.93f, 55},  {3.90f, 48}, {3.87f, 40}, {3.84f, 32},
    {3.81f, 24},  {3.78f, 18}, {3.75f, 13}, {3.72f, 9},
    {3.69f, 6},   {3.65f, 4},  {3.55f, 2},  {3.40f, 0}
  };

  if (v >= lut[0].v) return 100;
  const int n = (int)(sizeof(lut) / sizeof(lut[0]));
  if (v <= lut[n - 1].v) return 0;

  for (int i = 0; i < n - 1; i++) {
    float vHi = lut[i].v;
    float vLo = lut[i + 1].v;
    int pHi = lut[i].pct;
    int pLo = lut[i + 1].pct;
    if (v <= vHi && v >= vLo) {
      float t = (v - vLo) / (vHi - vLo);
      int pct = (int)(pLo + t * (float)(pHi - pLo) + 0.5f);
      if (pct < 0) pct = 0;
      if (pct > 100) pct = 100;
      return pct;
    }
  }
  return 0;
}

void updateBatteryCached(bool force) {
  uint32_t now = millis();
  if (!force && (now - g_battery.lastMs) < BAT_CACHE_MS) return;
  g_battery.lastMs = now;

  float raw = readBatteryVoltageRaw();
  bool valid = (raw > 2.8f && raw < 4.5f);
  g_battery.valid = valid;
  if (!valid) return;

  g_battery.rawV = raw;
  if (g_battery.filteredV <= 0.0f) {
    g_battery.filteredV = raw;
  } else {
    const float alpha = 0.22f;
    g_battery.filteredV = (alpha * raw) + ((1.0f - alpha) * g_battery.filteredV);
  }
  g_battery.filteredV = clampf(g_battery.filteredV, 3.0f, 4.25f);
  g_battery.pctRaw = batteryPercentFromOCV(g_battery.filteredV);

  if (force) {
    g_battery.pctShown = g_battery.pctRaw;
  } else {
    if (g_battery.pctRaw < g_battery.pctShown) {
      if ((g_battery.pctShown - g_battery.pctRaw) >= 1) g_battery.pctShown--;
    } else if (g_battery.pctRaw > g_battery.pctShown + 2) {
      g_battery.pctShown++;
    }
  }

  if (g_battery.pctShown < 0) g_battery.pctShown = 0;
  if (g_battery.pctShown > 100) g_battery.pctShown = 100;

  if (!g_battery.low && g_battery.pctShown <= 8) g_battery.low = true;
  else if (g_battery.low && g_battery.pctShown >= 12) g_battery.low = false;
}

#endif
