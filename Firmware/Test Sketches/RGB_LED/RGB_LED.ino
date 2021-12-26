#include <FastLED.h> // http://librarymanager/All#FastLED

#define LED_PIN     2 //Pin 2 on Thing Plus C is connected to WS2812 LED
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    1

#define BRIGHTNESS  25

CRGB leds[NUM_LEDS];

void setup() {
  delay(30); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(200);

  leds[0] = CRGB::Green;
  FastLED.show();
  delay(200);

  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(200);

  leds[0] = CRGB::Black;
  FastLED.show();
  delay(100);

  leds[0] = CRGB::White;
  FastLED.show();
  delay(2000);

  leds[0] = CRGB::Black;
  FastLED.show();
  delay(100);
}
