#include "Arduino.h"
#include "FastLED.h"

CRGB onboard_led[1];

void setup() {
  FastLED.addLeds<SK6812, PIN_RGB_LED>(onboard_led, 1);
  FastLED.setBrightness(255 / 100);
}

void loop() { 
  onboard_led[0] = CRGB::Red; FastLED.show(); delay(500);
  onboard_led[0] = CRGB::Blue; FastLED.show(); delay(500);        
}