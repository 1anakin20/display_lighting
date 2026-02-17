#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "Pattern.h"

class PatternManager {
public:
  PatternManager(CRGB* leds, int count);

  void setPatterns(Pattern** list, size_t n);
  void setTargetFps(uint16_t fps);
  void setRotateMs(uint32_t ms);

  void next();
  void prev();
  void switchTo(size_t idx);

  void handleSensor(const SensorEvent& e);
  void loop(uint32_t now);

  const char* currentName() const;

private:
  CRGB* leds; int count;
  Pattern** patterns = nullptr; size_t total = 0;
  size_t current = 0;

  uint32_t lastShow = 0;
  uint32_t lastSwitch = 0;
  uint32_t targetFrameMs = 16; // ~60 FPS default
  uint32_t rotateMs = 0;       // 0 = disabled
};