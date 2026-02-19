#include "SegmentAlternating.h"

void SegmentAlternating::begin(CRGB* leds, int count) {
  Pattern::begin(leds, count);
  lastFlip_ = millis();
  flipped_ = false;
  render_();
}

void SegmentAlternating::update(uint32_t now) {
  if ((uint32_t)(now - lastFlip_) >= intervalMs_) {
    lastFlip_ = now;
    flipped_ = !flipped_;
    render_();
  }
}

void SegmentAlternating::setSegments(uint8_t n) {
  segments_ = (n == 0) ? 1 : n;
}

void SegmentAlternating::setColors(const CRGB& a, const CRGB& b) {
  colorA_ = a;
  colorB_ = b;
}

void SegmentAlternating::setFlipInterval(uint32_t ms) {
  intervalMs_ = (ms == 0) ? 1 : ms;
}

void SegmentAlternating::render_() {
  if (!leds || count <= 0) return;

  uint16_t segLength = count / segments_;
  if (segLength == 0) segLength = 1;   // safety for too many segments

  for (uint8_t i = 0; i < segments_; ++i) {

    // Choose color based on segment index and flip state
    bool useA = ((i % 2 == 0) ^ flipped_);
    CRGB c = useA ? colorA_ : colorB_;

    uint16_t start = i * segLength;
    uint16_t end   = (i == segments_ - 1) ? count : start + segLength;

    for (uint16_t p = start; p < end; ++p) {
      leds[p] = c;
    }
  }
}