#include <Arduino.h>
#include "Chase.h"

void Chase::begin(CRGB* leds, int count) {
  Pattern::begin(leds, count);
  lastStep_ = millis();

  // Start phase offset at an edge depending on direction
  if (dir_ == Direction::FORWARD) {
    pos_ = 0;
  } else {
    pos_ = (count > 0) ? (count - 1) : 0;
  }

  renderFrame_();
}

void Chase::update(uint32_t now) {
  if (count <= 0 || !leds) return;

  // Move the global phase on schedule (non-blocking)
  if ((uint32_t)(now - lastStep_) >= stepMs_) {
    lastStep_ = now;

    // Advance phase with wraparound
    if (dir_ == Direction::FORWARD) {
      pos_ = (pos_ + 1) % count;
    } else {
      pos_ = (pos_ - 1 + count) % count;
    }

    renderFrame_();
  }
}

void Chase::renderFrame_() {
  if (!leds || count <= 0) return;

  // Clear to background each frame
  fill_solid(leds, count, bg_);

  // Determine spacing between heads
  const uint16_t spacing = effectiveSpacing_();
  const uint8_t  hcount  = (heads_ == 0) ? 1 : heads_;

  // For each head (evenly spaced), draw head + tail
  for (uint16_t h = 0; h < hcount; ++h) {
    // Base positions spaced by 'spacing', then shifted by global phase pos_
    int headPos = wrap_((int)(h * spacing) + pos_);

    // Draw head
    leds[headPos] = color_;

    // Draw tail
    if (tailLen_ > 0) {
      uint8_t denom = (tailLen_ == 0) ? 1 : tailLen_;
      for (uint8_t t = 1; t <= tailLen_; ++t) {
        int tailPos = (dir_ == Direction::FORWARD)
                      ? wrap_(headPos - (int)t)
                      : wrap_(headPos + (int)t);

        // Fade profile: brighter near head, dimmer farther
        // approx range 220 .. ~40
        uint16_t step = (uint16_t)((180U * (t - 1)) / denom);
        uint8_t scale = (step >= 220U) ? 20U : (uint8_t)(220U - step);
        if (scale < 20) scale = 20;

        CRGB c = color_;
        c.nscale8_video(scale);

        // Composite if tails overlap
        leds[tailPos].r = (leds[tailPos].r > c.r) ? leds[tailPos].r : c.r;
        leds[tailPos].g = (leds[tailPos].g > c.g) ? leds[tailPos].g : c.g;
        leds[tailPos].b = (leds[tailPos].b > c.b) ? leds[tailPos].b : c.b;
      }
    }
  }
}
