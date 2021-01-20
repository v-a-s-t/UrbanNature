/*TODO
  - Fix Lux Calculation
  - Convert Sound level to Decibel
  - Test Gas Sensor Calculation
*/

#include "Config.h"
#include <Wire.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <DNSServer.h>
#include "time.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

String ssid, pass;
bool usingWifi = false;
bool usingCaptivePortal = false;
String aio_key = "";
String user = "";
StaticJsonDocument<1024> sensorFeeds;
int startHour;
String lat, lon;
int interval; // In minutes!

// Captive portal
const byte DNS_PORT = 53;
DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
AsyncWebServer apServer(80);
String ap_ssid = "";
String ap_pass = "oak-ash-poplar";

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


bool useCaptivePortal = false;
bool hasBeenOnCharge = false;

//global sensor average variables
double redSample, oxSample, nh3Sample;
float temperatureSample, humiditySample, pressureSample, altitudeSample;
int luxSample;
float micPPSample;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setupPins();

  SPIFFS.begin();

  loadPreferences();

  setupPMU();

#ifdef DEBUG_OUTPUT
  printIP5306Settings();
#endif

  setupSensors();
  debugSensors();

#ifndef DEBUG_OUTPUT
  if (!isCharging()) {
    checkShortSleep();
  }
#else
  checkShortSleep();
#endif

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }


  checkButtonOnStartUp();
  if (sensorFeeds.size() == 0) usingCaptivePortal = true;
  if (usingCaptivePortal) {
    setupCaptivePortal();
  }
}

void loop() {
#ifndef DEBUG_OUTPUT
  bool isDeviceCharging = isCharging();
  if (isDeviceCharging) {
    chargingBlink();
  } 
#endif
  if (usingCaptivePortal)
    captivePortalHandler();
  else {
#ifndef DEBUG_OUTPUT
    if (!isDeviceCharging) {
      scheduleHandler();
    }
#else
    scheduleHandler();
#endif
  }
}
