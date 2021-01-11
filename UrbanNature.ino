/*Sensor order

  Cycle 1
  BME280 (30 seconds sample)
  LTR559 (30 seconds sample)
  Microphone (30 seconds sample)
  
  -Turn on gas heater
  -Turn on PMS Fan
  -PMS5003 (30 seconds sample)
  -Wait 2 minutes for heater to fan up

  Cycle 2
  MICS6814 (30 seconds sample)


*/

#include "Config.h"
#include <Wire.h>
#include <WiFi.h>
#include "time.h"

String ssid, pass;
bool isSimPresent = false;
String aio_key = "";
String user = "";

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setupPins();
  checkButtonOnStarUp();

  //loadPreferences();

  setupSensors();
  enableSensors();

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }

  //wifiConnect();
  //setupWifiTime();
  //getWifiTime();
  //printTime();
}

void loop() {

 //  setupModem();
 //  modemConnect();
  //
  // printAllSensors();
  //
  // postFloatToFeed(getTemp(), "test");
  // modemDisconnect();
 // modemPoweroff();
  //
  turnOffPMU();
  disableSensors();
  Serial.println("SLEEP");
  goToSleep(100);
}
