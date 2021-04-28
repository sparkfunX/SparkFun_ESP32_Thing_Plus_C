/*
  Attach a LiPo to the JST connector.
  
  We don't need to power up the Qwiic bus because the Fuel Gauge IC
  gets power from the LiPo. 

*/

#include <Wire.h> // Needed for I2C

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
SFE_MAX1704X lipo(MAX1704X_MAX17048); // Allow access to all the 17048 features

void setup()
{
	Serial.begin(115200); // Start serial, to output debug data
  Serial.println(F("MAX17048 Example"));

  Wire.begin();

  // Set up the MAX17048 LiPo fuel gauge:
  if (lipo.begin() == false) // Connect to the MAX17048 using the default wire port
  {
    Serial.println(F("MAX17048 not detected. Please check wiring. Freezing."));
    while (1)
      ;
  }
}

void loop()
{
  // Print the variables:
  Serial.print("Voltage: ");
  Serial.print(lipo.getVoltage());  // Print the battery voltage
  Serial.print("V");

  Serial.print(" Percentage: ");
  Serial.print(lipo.getSOC(), 2); // Print the battery state of charge with 2 decimal places
  Serial.print("%");

  Serial.print(" Change Rate: ");
  Serial.print(lipo.getChangeRate(), 2); // Print the battery change rate with 2 decimal places
  Serial.print("%/hr");

  Serial.println();

  delay(500);
}
