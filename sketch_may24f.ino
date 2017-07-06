#include <stdint.h>
#include "SparkFunBME280.h"
#include "Wire.h"
#include "SPI.h"
 
//Global sensor object
BME280 mySensor;
 
void setup()
{
  Serial.begin(9600);
  //SPI
  mySensor.settings.commInterface = SPI_MODE;
  mySensor.settings.chipSelectPin = 10;
 
  //Operation settings
  mySensor.settings.runMode = 3; //Normal mode
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;
 
  Serial.print("Starting BME280... result of .begin(): 0x");
  delay(10);  //BME280 requires 2ms to start up.
  Serial.println(mySensor.begin(), HEX);
 
}
 
void loop()
{
  //Each loop, take a reading.
 
  Serial.print("Temperature: ");
  Serial.print(mySensor.readTempC(), 2);
  Serial.println(" degrees C");
 
  Serial.print("Temperature: ");
  Serial.print(mySensor.readTempF(), 2);
  Serial.println(" degrees F");
 
  Serial.print("Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 2);
  Serial.println(" Pa");
 
  Serial.print("Altitude: ");
  Serial.print(mySensor.readFloatAltitudeMeters(), 2);
  Serial.println("m");
 
  Serial.print("Altitude: ");
  Serial.print(mySensor.readFloatAltitudeFeet(), 2);
  Serial.println("ft"); 
 
  Serial.print("%RH: ");
  Serial.print(mySensor.readFloatHumidity(), 2);
  Serial.println(" %");
 
  Serial.println();
 
  delay(1000);
 
}
