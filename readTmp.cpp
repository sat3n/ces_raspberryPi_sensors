#include <Wire.h>      //I2C library
#include <RtcDS3231.h> //RTC library
 
//RtcDS3231 rtcObject;              //Uncomment for version 1.0.1 of the rtc library
RtcDS3231<TwoWire> rtcObject(Wire); //Uncomment for version 2.0.0 
 
void setup() {
 
  Serial.begin(115200);
  rtcObject.Begin(); //Start I2C connection
 
}
 
void loop() {
 
  RtcTemperature temperature = rtcObject.GetTemperature();  //read temperature
 
  Serial.print(temperature.AsFloat());  //print temperature as a float
  Serial.println(" C");
 
  delay(10000);  //10 seconds delay
 
}
