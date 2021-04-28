/*
 This shows how to control the power to the Qwiic connector.

 This example also shows the Fuel Gauge IC at address 0x36.
 */

#include <Wire.h>

int qwiicPower = 0; //Thing Plus C 0 is connected to the v-reg that controls the Qwiic power

void setup()
{
  Serial.begin(115200);
  Serial.println("I2C Scanner");

  pinMode(qwiicPower, OUTPUT);
  digitalWrite(qwiicPower, HIGH);

  Wire.begin();
}

void loop()
{
  //Toggle the power on the Qwiic connector
  digitalWrite(qwiicPower, LOW);
  delay(2000); //Power LED on a Qwiic board should now be off
  digitalWrite(qwiicPower, HIGH);
  delay(1000); //Power LED on a Qwiic board should now be on

  Serial.println();
  for (byte address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Device found at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  Serial.println("Done");
  delay(100);
}
