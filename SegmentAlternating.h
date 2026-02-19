#pragma once
#include <FastLED.h>
#include "Pattern.h"

/**
 * SegmentAlternating:
 * - Divides strip into N segments.
 * - Alternates segment colors (A/B).
 * - After a delay, colors flip.
 */
class SegmentAlternating : public Pattern {
public:
  SegmentAlternating() = default;

  const char* name() const override { return "SegmentAlternating"; }
  void begin(CRGB* leds, int count) override;
  void update(uint32_t now) override;

  // Configuration API
  /**
  @param n number of segments
  */
  void setSegments(uint8_t n);

  /**
  @param CRGB a Color 1
  @param CRGB b Color 2
  */
  void setColors(const CRGB& a, const CRGB& b);
  void setFlipInterval(uint32_t ms);        // how fast to swap colors

private:
  uint8_t   segments_     = 4;      // number of segments
  CRGB      colorA_       = CRGB::Red;
  CRGB      colorB_       = CRGB::Blue;
  uint32_t  intervalMs_   = 500;    // swap interval
  uint32_t  lastFlip_     = 0;
  bool      flipped_      = false;

  void render_();
};