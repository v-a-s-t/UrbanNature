/*TODO
  - Fix Lux Calculation
  - Convert Sound level to Decibel
  - Test Gas Sensor Calculation
*/

#include "Config.h"
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

String ssid, pass;
bool isSimPresent = false;
String aio_key = "";
String user = "";
String sensorFeeds[NUM_OF_SENSORS];
bool enabledSensors[NUM_OF_SENSORS];
int numOfFeeds;

enum Sensor {
  sensor_p03um,
  sensor_p05um,
  sensor_p10um,
  sensor_p25um,
  sensor_p50um,
  sensor_p100um,
  sensor_pm10,
  sensor_pm25,
  sensor_pm100,
  sensor_temp,
  sensor_humidity,
  sensor_pressure,
  sensor_altitude,
  sensor_noise,
  sensor_oxidising,
  sensor_reducing,
  sensor_nh3,
  sensor_light
};

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
  scheduleHandler();
}
