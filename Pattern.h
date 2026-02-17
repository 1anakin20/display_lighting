#pragma once
#include <FastLED.h>

struct SensorEvent {
  enum Type : uint8_t { BUTTON, MIC_LEVEL, MOTION, CUSTOM } type;
  int value;      // Generic payload (level, id, etc.)
};

class Pattern {
public:
  virtual ~Pattern() {}
  virtual const char* name() const = 0;
  virtual void begin(CRGB* leds, int count) { this->leds = leds; this->count = count; }
  // Called frequently; must be NON-BLOCKING. 'now' is millis().
  virtual void update(uint32_t now) = 0;
  // Optional: react to sensors
  virtual void onSensor(const SensorEvent& e) { (void)e; }

protected:
  CRGB* leds = nullptr;
  int    count = 0;
};
