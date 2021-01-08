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
