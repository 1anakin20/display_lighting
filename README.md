# Display Lighting

## Project structure
- display_lighting.ino
    - Main class

Setup hardware config.
```cpp
#define NUM_LEDS     500
#define DATA_PIN     13
#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB
#define BRIGHTNESS   160
```

To instance the patterns
```cpp
ColorFade colorFadePattern;
Chase chase;

// Add them to the list in the order they will play
Pattern* patterns[] = {
  &colorFadePattern,
  &chase,
};
```

Then in the `setup()` method, set the paremeters for each pattern
```cpp
void setup() {  
  [...]
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
  [...]
}
```

- Pattern.h
    - Parent class for all patterns. Inherit from it for making yours.
- PatternManager.h/.cpp
    - Manages changing the patterns, also manages handling sensor events. Currently it loops trough the list of patterns changing them trough time.
- Chase.h/cpp, ColorFade.h/.cpp
    - Lighting patterns.