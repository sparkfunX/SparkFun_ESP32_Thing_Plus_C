/*
  Test:
  WS2812 all three RGB dies
  Fuel Gauge existance
  SD interface
  Stat LED
*/

const int statLED = 13;

//RGB LED
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <FastLED.h>

#define LED_PIN     2 //Pin 2 on Thing Plus C is connected to WS2812 LED
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    1

#define BRIGHTNESS  25

CRGB leds[NUM_LEDS];
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Fuel gauge
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
SFE_MAX1704X lipo(MAX1704X_MAX17048); // Allow access to all the 17048 features
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//SD Interface
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "FS.h"
#include "SD.h"
#include "SPI.h"

const int sd_cs = 5; //CS pin for Thing Plus C
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//EEPROM for storing settings
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <EEPROM.h>
#define EEPROM_SIZE 2048 //ESP32 emulates EEPROM in non-volatile storage (external flash IC). Max is 508k.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//Global vars
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool batteryGood = false;
bool sdGood = false;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void setup()
{
  Serial.begin(115200);
  //delay(500);
  Serial.println("ESP32 Thing Plus Test");

  //  while (Serial.available()) Serial.read();
  //  delay(100);
  //  Serial.println("Press key to start");
  //  while (Serial.available() == 0) delay(10);

  Wire.begin();

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );

  pinMode(statLED, OUTPUT);

  //Check to see if EEPROM is blank
  if (EEPROM.begin(EEPROM_SIZE) == false)
    Serial.println(F("Failed to initialize EEPROM"));
  uint32_t testRead = 0;
  EEPROM.get(0, testRead);
  if (testRead == 0xFFFFFFFF)
  {
    Serial.println(F("EEPROM is blank. Running test"));

    testSD();

    testBattery();

    if (batteryGood == false)// || sdGood == false) //Don't make the sd test required
    {
      //Display red
      leds[0] = CRGB::Red;
      FastLED.show();
      while (1);
    }
    else
    {
      //Mark EEPROM as all good
      uint8_t myVal = 0xAA;
      EEPROM.put(0, myVal);
      EEPROM.commit();
    }
  }
}

void loop()
{
  //Loop LED display
  digitalWrite(statLED, !digitalRead(statLED));

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
  delay(1000);

  leds[0] = CRGB::Black;
  FastLED.show();
  delay(100);
}

void testBattery()
{
  // Set up the MAX17048 LiPo fuel gauge:
  if (lipo.begin() == false) // Connect to the MAX17048 using the default wire port
  {
    Serial.println(F("MAX17048 BAD"));
    return;
  }

  Serial.println("Fuel gauge detected");
  batteryGood = true;
}

void testSD()
{
  if (!SD.begin(sd_cs)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.print("MMC");
  } else if (cardType == CARD_SD) {
    Serial.print("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.print("SDHC");
  } else {
    Serial.print("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf(" %lluMB ", cardSize);

  Serial.print("SD Good");

  Serial.println();

  sdGood = true;
}
