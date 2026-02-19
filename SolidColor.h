#pragma once
#include <FastLED.h>
#include "Pattern.h"

/**
 * SolidColor: fills the strip with a single color.
 * - Default: static color
 * - Optional: gentle hue drift over time (non-blocking)
 */
class SolidColor : public Pattern {
public:
  SolidColor() = default;

  const char* name() const override { return "SolidColor"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // Configuration
  void setColor(const CRGB& c);  // sets fixed RGB color and disables hue drift
  void setHSV(uint8_t h, uint8_t s, uint8_t v); // sets HSV and disables hue drift
  void setValue(uint8_t v);      // brightness scaling 0..255 applied per frame

  // Optional hue drift (enabled when you call enableHueDrift)
  void enableHueDrift(bool enable, uint16_t tickMs = 33, uint8_t hueStep = 1);

private:
  // Current color (either maintained as RGB or derived from HSV)
  CRGB     colorRGB_      = CRGB::Blue;
  uint8_t  valueScale_    = 255;     // per-pattern brightness scale

  // Hue drift config/state
  bool     hueDrift_      = false;
  uint16_t hueTickMs_     = 33;
  uint8_t  hueStep_       = 1;
  uint32_t lastHueTick_   = 0;
  uint8_t  hue_           = 0;
  uint8_t  sat_           = 255;
  bool     useHSV_        = false;   // if true, derive color from (hue_,sat_,valueScale_)
};