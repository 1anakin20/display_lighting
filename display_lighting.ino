#include "PatternManager.h"
#include "ColorFade.h"
#include "Chase.h"
#include "SegmentAlternating.h"

// ---------- Hardware ----------
#define NUM_LEDS     500
#define DATA_PIN     13
#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB
#define BRIGHTNESS   160

CRGB leds[NUM_LEDS];

ColorFade colorFadePattern;
Chase chase;
SegmentAlternating segmentPattern;

Pattern* patterns[] = {
  &segmentPattern,
  &colorFadePattern,
  &chase,
};

PatternManager manager(leds, NUM_LEDS);


void setup() {
  // 5 seconds delay for making it easy to reupload code to the arduino
  delay(5000);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  

  // Setup pattern parameters
  colorFadePattern.setTickMs(33);
  colorFadePattern.setHueStep(1);
  colorFadePattern.setSaturation(200);
  colorFadePattern.setValue(200);

  chase.setColor(CRGB::OrangeRed);
  chase.setStepMs(25);
  chase.setDirection(Chase::Direction::FORWARD);
  chase.setTail(6);
  chase.setBackground(CRGB::Black);
  
  segmentPattern.setSegments(30); // divide strip into 6 parts
  segmentPattern.setColors(CRGB::Orange, CRGB::Purple);
  segmentPattern.setFlipInterval(2000); // flip every 700ms

  manager.setPatterns(patterns, sizeof(patterns)/sizeof(patterns[0]));
  manager.setTargetFps(120);
  // Set the time in milliseconds for cycling trough each pattern
  manager.setRotateMs(20000);
}


void loop() {
  manager.loop(millis());
}

