/*TODO
  - Fix Lux Calculation
  - Convert Sound level to Decibel
  - Test Gas Sensor Calculation
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
  pinMode(LED, OUTPUT);

  loadPreferences();

  setupSensors();
  //enableSensors();

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }

  wifiConnect();
  setupWifiTime();
  getWifiTime();
  printTime();
}

void loop() {
  // setupModem();
  // modemConnect();
  //
  // printAllSensors();
  //
  // postFloatToFeed(getTemp(), "test");
  // modemDisconnect();
  // modemPoweroff();
  //
  // goToSleep(10);
}
