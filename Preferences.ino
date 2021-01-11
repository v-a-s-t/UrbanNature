#include <Preferences.h>

/*
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
  prefs.end();
}

void saveWifi(String ssid, String pass) {
  prefs.begin("urban-nature");
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}

void clearPreferences() {
  prefs.begin("urban-nature");
  prefs.clear();
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
