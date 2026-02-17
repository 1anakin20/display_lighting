#include "PatternManager.h"
#include "ColorFade.h"
#include "Chase.h"

// ---------- Hardware ----------
#define NUM_LEDS     500
#define DATA_PIN     13
#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB
#define BRIGHTNESS   160

CRGB leds[NUM_LEDS];

ColorFade colorFadePattern;
Chase chase;

Pattern* patterns[] = {
  &colorFadePattern,
  &chase,
};

PatternManager manager(leds, NUM_LEDS);


void setup() {
  // put your setup code here, to run once:
  delay(5000);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  colorFadePattern.setTickMs(33);     // ~30Hz hue updates
  colorFadePattern.setHueStep(1);     // slow drift
  colorFadePattern.setSaturation(200);
  colorFadePattern.setValue(200);


  chase.setColor(CRGB::OrangeRed);
  chase.setStepMs(25);                              // movement speed (ms per step)
  chase.setDirection(Chase::Direction::FORWARD);    // or REVERSE
  chase.setTail(6);                                 // 0 = no tail
  chase.setBackground(CRGB::Black);                 // default

  manager.setPatterns(patterns, sizeof(patterns)/sizeof(patterns[0]));
  manager.setTargetFps(120);   // cap FastLED.show()
  manager.setRotateMs(20000);  // switch patterns every 15,000 ms
}


void loop() {
  manager.loop(millis());
}

