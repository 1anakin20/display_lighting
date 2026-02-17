#include "ColorFade.h"

void ColorFade::begin(CRGB* leds, int count) {
  Pattern::begin(leds, count);
  lastTickMs_ = millis();
  startMs_    = lastTickMs_;
  // Draw initial frame
  fill_solid(leds, count, CHSV(hue_, sat_, val_));
}

void ColorFade::update(uint32_t now) {
  // Advance hue on schedule (non-blocking)
  if ((uint32_t)(now - lastTickMs_) >= tickMs_) {
    lastTickMs_ = now;
    hue_ += hueStep_; // wraps naturally at 255
  }

  // Determine brightness (with optional breathing)
  uint8_t brightness = val_;
  if (breathing_ && breathDepth_ > 0 && breathPeriodMs_ > 0) {
    uint8_t scale = computeBreathScale_(now); // 0..255
    // Mix base brightness with modulation; center around val_
    // depth controls how much of the modulation applies
    uint16_t delta = ((uint16_t)scale * breathDepth_) >> 7; // 0..(~2*depth)
    int16_t modVal = (int16_t)val_ - (int16_t)breathDepth_ + (int16_t)delta;
    brightness = (uint8_t)constrain(modVal, 0, 255);
  }

  // Render solid color
  fill_solid(leds, count, CHSV(hue_, sat_, brightness));
}

// ---------------- Configuration ----------------
void ColorFade::setTickMs(uint16_t ms) { tickMs_ = (ms == 0) ? 1 : ms; }
void ColorFade::setHueStep(uint8_t s)  { hueStep_ = (s == 0) ? 1 : s; }
void ColorFade::setSaturation(uint8_t s){ sat_ = s; }
void ColorFade::setValue(uint8_t v)     { val_ = v; }

void ColorFade::enableBreathing(bool enable, uint16_t periodMs, uint8_t depth) {
  breathing_ = enable;
  breathPeriodMs_ = (periodMs == 0) ? 1 : periodMs;
  breathDepth_ = (depth > 127) ? 127 : depth;
  startMs_ = millis();
}

// ---------------- Internals --------------------
// Smooth breathing curve using FastLED's beat8 + easing-like shaping.
// Returns a scale 0..255 (0 darkest, 255 brightest)
uint8_t ColorFade::computeBreathScale_(uint32_t now) const {
  // phase goes 0..255 over the period; beat8 handles wrap neatly
  uint8_t phase = beat8( (uint16_t)(1000UL * 256UL / breathPeriodMs_), (uint16_t)(now - startMs_) );
  // Map sine-ish: 0..255 -> 0..255 with a gentle curve
  // Use cubic shaping for softer ends
  uint16_t x = phase;                // 0..255
  uint16_t a = (x * x) >> 8;         // ~quadratic
  uint16_t b = (a * x) >> 8;         // ~cubic
  return (uint8_t)b;                 // 0..255
}
