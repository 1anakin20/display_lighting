#include "PatternManager.h"
#include "ColorFade.h"
#include "Chase.h"
#include "SegmentAlternating.h"
#include "SolidColor.h"

// ---------- Hardware ----------
#define NUM_LEDS     500
#define DATA_PIN     13
#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB
#define BRIGHTNESS   160

CRGB leds[NUM_LEDS];

SolidColor whiteColorPattern;
SegmentAlternating segmentPattern;
Chase chase;
ColorFade allColorFadePattern;

Pattern* patterns[] = {
  &whiteColorPattern,
  &segmentPattern,
  &chase,
  &allColorFadePattern,
};

PatternManager manager(leds, NUM_LEDS);

void setup() {
  // 5 seconds delay for making it easy to reupload code to the arduino
  delay(5000);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Setup pattern parameters
  whiteColorPattern.setColor(CRGB::White);
  
  segmentPattern.setSegments(30); // divide strip into 6 parts
  segmentPattern.setColors(CRGB::Orange, CRGB::Purple);
  segmentPattern.setFlipInterval(2000); // flip every 700ms

  chase.setColor(CRGB::Green);
  chase.setStepMs(25);
  chase.setDirection(Chase::Direction::FORWARD);
  chase.setTail(6);
  chase.setBackground(CRGB::Black);
  chase.setHeads(12);

  manager.setPatterns(patterns, sizeof(patterns)/sizeof(patterns[0]));
  manager.setTargetFps(120);

  allColorFadePattern.setTickMs(33);
  allColorFadePattern.setHueStep(1);
  allColorFadePattern.setSaturation(200);
  allColorFadePattern.setValue(200);

  manager.setPatternDuration(0, 60000);
  manager.setPatternDuration(3, 45000);

  // Optional uniform fallback for patterns left at 0:
  manager.setRotateMs(20000);
}


void loop() {
  manager.loop(millis());
}

