#include "PatternManager.h"

PatternManager::PatternManager(CRGB* leds, int count)
  : leds(leds), count(count) {}

void PatternManager::setPatterns(Pattern** list, size_t n) {
  patterns = list; total = n;
  if (total > 0) {
    current = 0;
    patterns[current]->begin(leds, count);
    lastSwitch = millis();
  }
}

void PatternManager::setTargetFps(uint16_t fps) {
  targetFrameMs = (fps > 0) ? (1000UL / fps) : 1000UL;
}

void PatternManager::setRotateMs(uint32_t ms) {
  rotateMs = ms;
}

void PatternManager::next() {
  if (!total) return;
  switchTo((current + 1) % total);
}

void PatternManager::prev() {
  if (!total) return;
  switchTo((current + total - 1) % total);
}

void PatternManager::switchTo(size_t idx) {
  if (!total) return;
  idx %= total;
  current = idx;
  patterns[current]->begin(leds, count);
  lastSwitch = millis();
}

void PatternManager::handleSensor(const SensorEvent& e) {
  if (!total) return;
  patterns[current]->onSensor(e);
}

void PatternManager::loop(uint32_t now) {
  if (!total) return;

  // Time-based rotation
  if (rotateMs > 0 && (now - lastSwitch) >= rotateMs) {
    next();
  }

  // Let the active pattern render into leds[]
  patterns[current]->update(now);

  // Throttle FastLED.show() to target FPS
  if ((now - lastShow) >= targetFrameMs) {
    FastLED.show();
    lastShow = now;
  }
}

const char* PatternManager::currentName() const {
  return total ? patterns[current]->name() : "";
}
