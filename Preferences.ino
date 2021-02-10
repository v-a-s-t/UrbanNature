#include <Preferences.h>

/*
   Example of the sensorFeeds json object:

  {
  "sensor_p03um": "myfeedforthissensor",
  "sensor_p05um": "myfeedforthissensor",
  "sensor_p10um": "myfeedforthissensor",
  "sensor_p25um": "myfeedforthissensor",
  "sensor_p50um": "myfeedforthissensor",
  "sensor_p100um": "myfeedforthissensor",
  "sensor_pm10": "myfeedforthissensor",
  "sensor_pm25": "myfeedforthissensor",
  "sensor_pm100": "myfeedforthissensor",
  "sensor_temp": "myfeedforthissensor",
  "sensor_humidity": "myfeedforthissensor",
  "sensor_pressure": "myfeedforthissensor",
  "sensor_altitude": "myfeedforthissensor",
  "sensor_noise": "myfeedforthissensor",
  "sensor_oxidising": "myfeedforthissensor",
  "sensor_reducing": "myfeedforthissensor",
  "sensor_nh3": "myfeedforthissensor",
  "sensor_light": "myfeedforthissensor"
  }

  Things stored here:
  - WiFi station
  - WiFi Password
  - active sensor with key
  - aio key
  - aio user
  - interval
  - on time
  - off time
*/

Preferences prefs;


void loadPreferences() {
  // Load all settings
  prefs.begin("urban-nature");
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");
  user = prefs.getString("user", "");
  aio_key = prefs.getString("aio_key", "");
  startHour = prefs.getInt("startHour", 0);
  interval = prefs.getInt("interval", 60);
#ifdef DEBUG_INTERVAL
  interval = DEBUG_INTERVAL;
#endif
  lat = prefs.getString("lat", "");
  lon = prefs.getString("lon", "");
  String sensorFeedsBuffer = prefs.getString("sensorFeeds", "");
  sensorFeeds.clear();
  deserializeJson(sensorFeeds, sensorFeedsBuffer);
  prefs.end();

  Serial.println("Loaded preferences.");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("AIO user: ");
  Serial.println(user);
  Serial.print("Start hour: ");
  Serial.println(startHour);
  Serial.print("Interval: ");
  Serial.println(interval);
  Serial.print("Lat: ");
  Serial.println(lat);
  Serial.print("Lon: ");
  Serial.println(lon);
  Serial.print("Sensor feeds: ");
  serializeJson(sensorFeeds, Serial);
  Serial.println();
}

void savePreferences() {
  String prevLat, prevLon;
  prefs.begin("urban-nature");
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.putString("user", user);
  prefs.putString("aio_key", aio_key);
  prefs.putInt("startHour", startHour);
  prefs.putInt("interval", interval);
  prevLat = prefs.getString("lat", "");
  prevLon = prefs.getString("lon", "");
  prefs.putString("lat", lat);
  prefs.putString("lon", lon);
  if (prevLat == lat && prevLon == lon) {
    prefs.putUInt("hasUpdated", 0);
  } else {
    prefs.putUInt("hasUpdated", 1);
  }
  String sensorFeedsBuffer;
  serializeJson(sensorFeeds, sensorFeedsBuffer);
  prefs.putString("sensorFeeds", sensorFeedsBuffer);
  prefs.end();
  Serial.println("Saved preferences.");
}

bool isNewLocation() {
  //Only returns true once if updated location
  uint8_t newLocation = false;
  prefs.begin("urban-nature");
  newLocation = prefs.getUInt("hasUpdated", 0);
  prefs.putUInt("hasUpdated", 0);
  prefs.end();
  if (newLocation == 1) {
    Serial.println("Monitoring in new setup location!");
  } else {
    Serial.println("Monitoring in the same Location as previously. If you has moved your sensor to a new location, please update the Longitude and Latitude on the captive portal");
  }
  return newLocation;
}

// void saveFeeds(String * feeds) {
//   // TODO add to sensorFeeds[] and enable in enabledSensors[]
//
// }
//
// void clearFeed(byte sensor) {
//   // TODO clear feed from sensorFeeds[] and disable in enabledSensors[]
// }
//
// void saveWifiCredentials(String ssid, String pass) {
//   prefs.begin("urban-nature");
//   prefs.putString("ssid", ssid);
//   prefs.putString("pass", pass);
//   prefs.end();
// }
//
// void saveAioCredentials(String key, String usr) {
//   prefs.begin("urban-nature");
//   prefs.putString("aio_key", key);
//   prefs.putString("user", usr);
//   prefs.end();
// }

void clearPreferences() {
  prefs.begin("urban-nature");
  prefs.clear();
  prefs.end();
}


float prefsGetFloat(char * name) {
  prefs.begin("urban-nature");
  float output = prefs.getFloat(name, 0);
  prefs.end();
  return output;
}

int prefsGetInt(char * name) {
  prefs.begin("urban-nature");
  int output = prefs.getUInt(name, 0);
  prefs.end();
  return output;
}

void prefsSetInt(char * name, int value) {
  prefs.begin("urban-nature");
  prefs.putUInt(name, value);
  prefs.end();
}

void prefsSetFloat(char * name, float value) {
  prefs.begin("urban-nature");
  prefs.putUInt(name, value);
  prefs.end();
}
