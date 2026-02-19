#include "PatternManager.h"

PatternManager::PatternManager(CRGB* leds, int count)
  : leds(leds), count(count) {}

void PatternManager::setPatterns(Pattern** list, size_t n) {
  patterns = list; 
  total = n;
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
  // Forward to whichever pattern is currently rendering
  if (inOverride) {
    if (overridePattern) overridePattern->onSensor(e);
  } else {
    if (!total) return;
    patterns[current]->onSensor(e);
  }
}

// ------------- NEW: Override control -------------
void PatternManager::triggerOverride(Pattern* pattern, uint32_t durationMs) {
  if (!pattern) return;

  uint32_t now = millis();

  // If entering override from list mode, remember where to return
  if (!inOverride && total > 0) {
    resumeIndex = current;
  }

  startOverridePattern_(pattern, now, (durationMs == 0 ? 1 : durationMs));
}

void PatternManager::cancelOverride() {
  if (!inOverride) return;
  inOverride = false;
  overridePattern = nullptr;

  // Return to previous list pattern if available
  if (total > 0) {
    switchTo(resumeIndex);
  }
}

void PatternManager::startOverridePattern_(Pattern* p, uint32_t now, uint32_t durationMs) {
  overridePattern = p;
  overridePattern->begin(leds, count);

  inOverride = true;
  overrideUntil = now + durationMs;
  lastSwitch = now; // keep internal timing consistent
}

void PatternManager::startListPattern_(uint32_t now) {
  if (!total) return;
  patterns[current]->begin(leds, count);
  lastSwitch = now;
}
// ------------------------------------------------

void PatternManager::loop(uint32_t now) {
  // If no patterns registered and no override, nothing to do
  if (!inOverride && !total) return;

  // --- Override path: takes priority over rotation ---
  if (inOverride) {
    // End override when time is up
    if ((int32_t)(now - overrideUntil) >= 0) {
      inOverride = false;
      overridePattern = nullptr;
      // Return to the list pattern we were on before override
      if (total > 0) {
        // switchTo will call begin() and reset lastSwitch
        switchTo(resumeIndex);
      }
    } else {
      // Still in override: update that pattern
      if (overridePattern) {
        overridePattern->update(now);
      }
    }
  }
  else {
    // --- Normal rotation over the list ---
    if (rotateMs > 0 && (now - lastSwitch) >= rotateMs) {
      next();
    }
    // Update the active list pattern
    patterns[current]->update(now);
  }

  // Throttle FastLED.show() to target FPS
  if ((now - lastShow) >= targetFrameMs) {
    FastLED.show();
    lastShow = now;
  }
}

const char* PatternManager::currentName() const {
  if (inOverride && overridePattern) return overridePattern->name();
  return total ? patterns[current]->name() : "";
}