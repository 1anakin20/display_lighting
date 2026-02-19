#include "Button.h"

Button::Button(uint8_t pin, bool activeLow, uint16_t debounceMs)
  : pin_(pin), activeLow_(activeLow), debounceMs_(debounceMs) {}

void Button::begin() {
  if (activeLow_) {
    pinMode(pin_, INPUT_PULLUP);
    lastRead_ = HIGH;
    stable_   = HIGH;
  } else {
    pinMode(pin_, INPUT);
    lastRead_ = LOW;
    stable_   = LOW;
  }
  lastChange_ = millis();
  lastPress_  = 0;
}

bool Button::poll(uint32_t now) {
  bool r = digitalRead(pin_);
  if (r != lastRead_) {
    lastRead_ = r;
    lastChange_ = now;
  }
  if ((now - lastChange_) >= debounceMs_ && r != stable_) {
    stable_ = r; // state change confirmed
    bool pressed = activeLow_ ? (stable_ == LOW) : (stable_ == HIGH);
    if (pressed) { lastPress_ = now; return true; }
  }
  return false;
}

bool Button::held() const {
  return activeLow_ ? (stable_ == LOW) : (stable_ == HIGH);
}

uint32_t Button::lastChangeTime() const { return lastChange_; }

bool Button::longPress(uint32_t now, uint32_t holdMs) {
  if (!held()) return false;
  if (lastPress_ == 0) return false;
  return (now - lastPress_) >= holdMs;
}
