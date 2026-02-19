#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "Pattern.h"

/**
Handles changing the patterns through time and handles sensors
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
  Sets the list of patterns to cycle through
  @param list Array of patterns
  @param n Size of the array 
  */
  void setPatterns(Pattern** list, size_t n);

  /**
  Target refresh rate
  @param fps Refresh rate
  */
  void setTargetFps(uint16_t fps);

  /**
  Uniform time dedicated to each pattern (fallback if no per-pattern duration is set)
  @param ms Change time in milliseconds
  */
  void setRotateMs(uint32_t ms);

  /**
  Set pattern duration for a specific pattern in the array 
  @param idx Position of the pattern
  @param ms Duration in milliseconds (0 => use rotateMs fallback; if rotateMs is 0, no auto-advance)
  */
  void setPatternDuration(size_t idx, uint32_t ms);

  /**
  Change to the next pattern
  */
  void next();

  /**
  Changes to previous pattern
  */
  void prev();

  /**
  Switches to a specific pattern
  @param idx Array index for the one set in the setPatterns() function
  */
  void switchTo(size_t idx);

  /**
  Handles sensor events (pass-through to current pattern)
  */
  void handleSensor(const SensorEvent& e);

  /**
  Handles looping through the patterns. Call this in the main loop.
  @param now Current time in milliseconds
  */
  void loop(uint32_t now);

  const char* currentName() const;

private:
  void startCurrent_(uint32_t now);
  uint32_t currentDurationMs_() const;

  CRGB* leds; 
  int count;

  Pattern** patterns = nullptr; 
  size_t total = 0;
  size_t current = 0;

  uint32_t lastShow = 0;
  uint32_t lastSwitch = 0;     // retained for backwards-compatibility
  uint32_t targetFrameMs = 16; // ~60 FPS default
  uint32_t rotateMs = 0;       // 0 = disabled (fallback if no per-pattern duration set)

  // --- Per-pattern durations (dynamic, matches 'total') ---
  uint32_t* durationsMs = nullptr;  // length == total; entries default to 0
  size_t    durationsCount = 0;     // equals 'total' after setPatterns()
  uint32_t  currentStartMs = 0;     // millis() when current pattern started
};