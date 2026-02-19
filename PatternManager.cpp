#include "PatternManager.h"

PatternManager::PatternManager(CRGB* leds, int count)
  : leds(leds), count(count) {}

void PatternManager::setPatterns(Pattern** list, size_t n) {
  patterns = list; 
  total = n;

  // Recreate durations array to match 'total'
  if (durationsMs) {
    delete[] durationsMs;
    durationsMs = nullptr;
    durationsCount = 0;
  }
  if (total > 0) {
    durationsMs = new uint32_t[total];
    durationsCount = total;
    for (size_t i = 0; i < durationsCount; ++i) durationsMs[i] = 0; // default: 0 => use rotateMs
  }

  if (total > 0) {
    current = 0;
    patterns[current]->begin(leds, count);
    lastSwitch = millis();
    currentStartMs = lastSwitch; // start elapsed timer for current pattern
  }
}

void PatternManager::setTargetFps(uint16_t fps) {
  targetFrameMs = (fps > 0) ? (1000UL / fps) : 1000UL;
}

void PatternManager::setRotateMs(uint32_t ms) {
  rotateMs = ms;
}

void PatternManager::setPatternDuration(size_t idx, uint32_t ms) {
  if (durationsMs && idx < durationsCount) {
    durationsMs[idx] = ms; // 0 => use rotateMs fallback (or no auto-advance if rotateMs==0)
  }
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
  currentStartMs = lastSwitch; // reset per-pattern elapsed timer
}

void PatternManager::handleSensor(const SensorEvent& e) {
  if (!total) return;
  patterns[current]->onSensor(e);
}

uint32_t PatternManager::currentDurationMs_() const {
  // Prefer per-pattern duration if configured (and durations array exists)
  if (durationsMs && current < durationsCount && durationsMs[current] > 0) {
    return durationsMs[current];
  }
  // Fallback to uniform rotateMs (may be 0 => no auto-advance)
  return rotateMs;
}

void PatternManager::startCurrent_(uint32_t now) {
  patterns[current]->begin(leds, count);
  lastSwitch = now;
  currentStartMs = now;
}

void PatternManager::loop(uint32_t now) {
  if (!total) return;

  // Determine the active duration: per-pattern first, then uniform fallback.
  const uint32_t dur = currentDurationMs_();

  // Auto-advance if a duration is active
  if (dur > 0 && (now - currentStartMs) >= dur) {
    next(); // switchTo() inside next() will reset currentStartMs
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