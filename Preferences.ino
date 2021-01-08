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
  numOfFeeds = prefs.getInt("numOfFeeds", 0);
  // TODO load sensorFeeds[] and enabledSensors[]
  prefs.end();
}

void saveFeed(byte sensor, String feed) {
  // TODO add to sensorFeeds[] and enable in enabledSensors[]
  
}

void clearFeed(byte sensor) {
  // TODO clear feed from sensorFeeds[] and disable in enabledSensors[]
}

void saveWifiCredentials(String ssid, String pass) {
  prefs.begin("urban-nature");
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}

void saveAioCredentials(String key, String usr) {
  prefs.begin("urban-nature");
  prefs.putString("aio_key", key);
  prefs.putString("user", usr);
  prefs.end();
}

void clearPreferences() {
  prefs.begin("urban-nature");
  prefs.clear();
}
