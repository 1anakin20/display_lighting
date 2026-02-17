#pragma once
#include <FastLED.h>
#include "Pattern.h"

/**
 * ColorFade: progressively changes the solid color over time by drifting hue.
 * - Non-blocking: uses millis()
 * - Minimal state: base hue, tick timing
 * - Optional breathing brightness modulation (off by default)
 */
class ColorFade : public Pattern {
public:
  ColorFade() = default;

  const char* name() const override { return "ColorFade"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // ---- Configuration API ----
  void setTickMs(uint16_t ms);       // how often to advance hue (default 20ms ≈ 50Hz)
  void setHueStep(uint8_t step);     // hue increment per tick (default 1)
  void setSaturation(uint8_t s);     // HSV saturation (0..255, default 255)
  void setValue(uint8_t v);          // base brightness (0..255, default 255)

  // Optional "breathing" brightness modulation (smooth in/out)
  void enableBreathing(bool enable, uint16_t periodMs = 4000, uint8_t depth = 64);
  // periodMs: full in+out cycle duration
  // depth: modulation depth (0..127). 0 disables effect even if enabled.

private:
  // Animation state
  uint32_t lastTickMs_ = 0;
  uint8_t  hue_        = 0;

  // Config
  uint16_t tickMs_     = 20;   // advance roughly every 20ms
  uint8_t  hueStep_    = 1;    // add 1 to hue each tick
  uint8_t  sat_        = 255;  // saturation
  uint8_t  val_        = 255;  // base brightness

  // Breathing
  bool     breathing_  = false;
  uint16_t breathPeriodMs_ = 4000; // 4s cycle
  uint8_t  breathDepth_    = 64;   // 0..127 (how strong the effect)
  uint32_t startMs_    = 0;        // reference time for breathing phase

  uint8_t computeBreathScale_(uint32_t now) const; // returns 0..255
};