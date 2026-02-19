#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "Pattern.h"

/**
 * Chase: multiple moving "runners" (heads + optional tail) along the strip.
 * - Non-blocking update via millis()
 * - Configurable speed, color, direction, tail, and number of heads
 * - Heads are evenly spaced by default; you can also set explicit pixel spacing
 */
class Chase : public Pattern {
public:
  enum class Direction : int8_t { FORWARD = 1, REVERSE = -1 };

  Chase() = default;

  const char* name() const override { return "Chase"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // ---- Configuration API ----
  void setColor(const CRGB& c)        { color_ = c; }
  void setStepMs(uint16_t ms)         { stepMs_ = (ms == 0) ? 1 : ms; }  // ms per step
  void setDirection(Direction d)      { dir_ = d; }
  void setTail(uint8_t len)           { tailLen_ = len; }                // 0 = no tail
  void setBackground(const CRGB& bg)  { bg_ = bg; }

  // New: multiple heads (density) and optional explicit spacing
  void setHeads(uint8_t n)            { heads_ = (n == 0) ? 1 : n; }     // >=1
  void setSpacing(uint16_t px)        { spacingPx_ = px; }               // 0 = auto even spacing

private:
  void renderFrame_();
  inline int wrap_(int idx) const {
    if (count <= 0) return 0;
    while (idx < 0) idx += count;
    while (idx >= count) idx -= count;
    return idx;
  }
  uint16_t effectiveSpacing_() const {
    if (count <= 0) return 1;
    if (spacingPx_ > 0) return spacingPx_;
    // Even spacing by heads
    uint16_t s = (heads_ > 0) ? (uint16_t)(count / heads_) : (uint16_t)count;
    return (s == 0) ? 1 : s;
  }

  // Config
  CRGB       color_      = CRGB::White;
  CRGB       bg_         = CRGB::Black;
  uint16_t   stepMs_     = 30;
  Direction  dir_        = Direction::FORWARD;
  uint8_t    tailLen_    = 0;

  // NEW
  uint8_t    heads_      = 4;      // number of simultaneous heads
  uint16_t   spacingPx_  = 0;      // 0 => auto (count / heads)

  // State
  uint32_t   lastStep_   = 0;
  int        pos_        = 0;      // global phase offset
};