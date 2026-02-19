#include "SolidColor.h"

void SolidColor::begin(CRGB* leds, int count) {
  Pattern::begin(leds, count);
  lastHueTick_ = millis();

  // Render initial frame
  if (useHSV_) {
    CRGB c = CHSV(hue_, sat_, 255);
    if (valueScale_ < 255) c.nscale8_video(valueScale_);
    fill_solid(leds, count, c);
  } else {
    CRGB c = colorRGB_;
    if (valueScale_ < 255) c.nscale8_video(valueScale_);
    fill_solid(leds, count, c);
  }
}

void SolidColor::update(uint32_t now) {
  if (!leds || count <= 0) return;

  // // Optionally advance hue over time
  // if (hueDrift_ && (uint32_t)(now - lastHueTick_) >= hueTickMs_) {
  //   lastHueTick_ = now;
  //   hue_ += hueStep_;
  // }

  CRGB c;
  if (useHSV_) {
    c = CHSV(hue_, sat_, 255);
  } else {
    c = colorRGB_;
  }

  if (valueScale_ < 255) {
    c.nscale8_video(valueScale_);
  }

  fill_solid(leds, count, c);
}

// ---------- Config ----------
void SolidColor::setColor(const CRGB& c) {
  colorRGB_ = c;
  useHSV_ = false;
}

void SolidColor::setHSV(uint8_t h, uint8_t s, uint8_t v) {
  hue_ = h;
  sat_ = s;
  valueScale_ = v;
  useHSV_ = true;
}

void SolidColor::setValue(uint8_t v) {
  valueScale_ = v;
}

// void SolidColor::enableHueDrift(bool enable, uint16_t tickMs, uint8_t hueStep) {
//   hueDrift_ = enable;
//   hueTickMs_ = (tickMs == 0) ? 1 : tickMs;
//   hueStep_ = (hueStep == 0) ? 1 : hueStep;
//   if (enable && !useHSV_) {
//     // Convert current RGB to HSV baseline (approx) by taking hue from CHSV(…)
//     // We’ll simply start from current hue_ as-is; users can setHSV() explicitly if they want.
//     useHSV_ = true;
//   }
// }