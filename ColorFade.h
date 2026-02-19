#pragma once
#include <FastLED.h>
#include "Pattern.h"

/**
 * ColorFade: progressively changes the solid color over time by drifting hue.
 */
class ColorFade : public Pattern {
public:
  ColorFade() = default;

  const char* name() const override { return "ColorFade"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // ---- Configuration API ----
  /**
  Speed of changing the colors
  @param ms time of the color in miliseconds. Default 20ms
  */
  void setTickMs(uint16_t ms);

  /**
  @param step Amount increment the hue per tick. Default 1
  */
  void setHueStep(uint8_t step);

  /**
  @param s Saturation. 0..255, default 255 
  */
  void setSaturation(uint8_t s);

  /**
  @param base brightness. 0..255, default 255
  */
  void setValue(uint8_t v);

  /**
  Breathing animation, smooths the in and out
  @param enable True to enable the breathing, default is disblaed.
  @param periodMs Duration of a cycle in miliseconds
  @param depth 0 to 127. How bright the top of the breathing cycle is. Note 0 will not show a cycle even if enabled. 
  */
  void enableBreathing(bool enable, uint16_t periodMs = 4000, uint8_t depth = 64);
  
private:
  // Animation state
  uint32_t lastTickMs_ = 0;
  uint8_t  hue_        = 0;

  // Default config
  uint16_t tickMs_     = 20;   // advance roughly every 20ms
  uint8_t  hueStep_    = 1;    // add 1 to hue each tick
  uint8_t  sat_        = 255;  // saturation
  uint8_t  val_        = 255;  // base brightness

  // Default breathing config
  bool     breathing_  = false;
  uint16_t breathPeriodMs_ = 4000; // 4s cycle
  uint8_t  breathDepth_    = 64;   // 0..127 (how strong the effect)
  uint32_t startMs_    = 0;        // reference time for breathing phase

  /**
  Computes the breathing curve
  @returns 0..255
  */
  uint8_t computeBreathScale_(uint32_t now) const;
};