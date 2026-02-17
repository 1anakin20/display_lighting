#pragma once
#include <FastLED.h>
#include "Pattern.h"

/**
 * Chase: a simple moving "runner" along the strip.
 * - Non-blocking update (millis())
 * - Configurable speed (step interval), color, direction, and tail length
 * - Writes directly into the shared leds[] buffer
 */
class Chase : public Pattern {
public:
  enum class Direction : int8_t {
    FORWARD = 1,
    REVERSE = -1
  };

  Chase() = default;

  const char* name() const override { return "Chase"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // ---- Configuration API ----
  void setColor(const CRGB& c)        { color_ = c; }
  void setStepMs(uint16_t ms);        // milliseconds per step (movement speed)
  void setDirection(Direction d)      { dir_ = d; }
  void setTail(uint8_t len);          // 0 = no tail, >0 = fade trailing pixels
  void setBackground(const CRGB& bg)  { bg_ = bg; } // background color (default: black)

private:
  // Draw head + optional tail
  void renderFrame_();

  // Config
  CRGB     color_     = CRGB::White;
  CRGB     bg_        = CRGB::Black;
  uint16_t stepMs_    = 30;                 // lower = faster motion
  Direction dir_      = Direction::FORWARD; // motion direction
  uint8_t  tailLen_   = 0;                  // 0..(count-1), fades behind head

  // State
  uint32_t lastStep_  = 0;
  int      pos_       = 0;
};