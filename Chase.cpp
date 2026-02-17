#include "Chase.h"

void Chase::begin(CRGB* leds, int count) {
  Pattern::begin(leds, count);
  lastStep_ = millis();

  // Start at edge depending on direction
  if (dir_ == Direction::FORWARD) {
    pos_ = 0;
  } else {
    pos_ = (count > 0) ? (count - 1) : 0;
  }

  // Draw initial frame
  renderFrame_();
}

void Chase::update(uint32_t now) {
  if (count <= 0 || !leds) return;

  // Move the head on schedule (non-blocking)
  if ((uint32_t)(now - lastStep_) >= stepMs_) {
    lastStep_ = now;

    // Advance position with wraparound
    if (dir_ == Direction::FORWARD) {
      pos_ = (pos_ + 1) % count;
    } else {
      pos_ = (pos_ - 1 + count) % count;
    }

    renderFrame_();
  }
}

void Chase::setStepMs(uint16_t ms) {
  stepMs_ = (ms == 0) ? 1 : ms;
}

void Chase::setTail(uint8_t len) {
  tailLen_ = len; // will be clamped at render time to (count-1)
}

void Chase::renderFrame_() {
  if (!leds || count <= 0) return;

  // Start from background each frame to keep the tail precise
  fill_solid(leds, count, bg_);

  // Draw head
  leds[pos_] = color_;

  // Draw tail (fading trail behind head)
  if (tailLen_ > 0) {
    uint8_t maxTail = (count > 0) ? (count - 1) : 0;
    uint8_t tail = (tailLen_ > maxTail) ? maxTail : tailLen_;

    // Fade steps: stronger near head, weaker further away
    // Using nscale8_video to keep color vividness
    for (uint8_t t = 1; t <= tail; ++t) {
      int idx = (dir_ == Direction::FORWARD)
              ? (pos_ - t)
              : (pos_ + t);
      // wrap
      while (idx < 0)      idx += count;
      while (idx >= count) idx -= count;

      // Scale color by a fading factor
      // Example fade profile: 220, 180, 140, ...
      uint8_t scale = 220 - (t - 1) * (180 / (tail > 0 ? tail : 1));
      if (scale < 10) scale = 10;

      CRGB c = color_;
      c.nscale8_video(scale);
      leds[idx] = c;
    }
  }
}