#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "Pattern.h"

/**
Handles changing the patterns trough time and handles sensors
*/
class PatternManager {
public:
  /**
  PatternManager constructor
  @param CRGB FastLED CRGB construct: https://fastled.io/docs/d7/d82/struct_c_r_g_b.html
  @param count Amount of LEDs
  */
  PatternManager(CRGB* leds, int count);

  /**
  Sets the list of patterns to cycle trough
  @param list Array of patterns
  @param n Size of each element 
  */
  void setPatterns(Pattern** list, size_t n);

  /**
  Target refresh rate
  @param fps Refresh rate
  */
  void setTargetFps(uint16_t fps);

  /**
  How much time is dedicated to each pattern
  @param ms Change time in miliseconds
  */
  void setRotateMs(uint32_t ms);

  /**
  Change to the next pattern
  */
  void next();

  /**
  Changes to previous pattern
  */
  void prev();

  /**
  Switches to a sepecific pattern
  @param idx Array index for the one set in the setPatterns() function
  */
  void switchTo(size_t idx);

  /**
  TODO. Handles sensor events
  */
  void handleSensor(const SensorEvent& e);

  /**
  Handles looping trough the patterns. Call this in the main loop.
  @param now Current time in miliseconds
  */
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