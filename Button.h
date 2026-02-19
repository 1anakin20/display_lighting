#pragma once
#include <Arduino.h>

/**
 * Debounced momentary button.
 * - Active-low (default with INPUT_PULLUP) or active-high.
 * - Call begin() in setup().
 * - Call poll(now) in loop(); returns true exactly once per debounced PRESS.
 * - held() tells you current logical press state.
 */
class Button {
public:
  Button(uint8_t pin, bool activeLow = true, uint16_t debounceMs = 30);

  void begin();
  bool poll(uint32_t now);           // fires once on PRESS edge
  bool held() const;                  // is it currently pressed?
  uint32_t lastChangeTime() const;    // millis() when last debounced change occurred
  bool longPress(uint32_t now, uint32_t holdMs); // optional helper

private:
  uint8_t  pin_;
  bool     activeLow_;
  uint16_t debounceMs_;

  bool     lastRead_   = HIGH;  // raw instantaneous sample
  bool     stable_     = HIGH;  // debounced stable value
  uint32_t lastChange_ = 0;     // when raw last changed
  uint32_t lastPress_  = 0;     // when last PRESS edge fired
};