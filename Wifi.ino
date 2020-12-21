bool wifiConnect() {
  if (ssid.length() && pass.length()) {
    WiFi.begin(ssid.c_str(), pass.c_str());

    Serial.print("Connecting to WiFi");
    unsigned long timeout = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - timeout < WIFI_CONNECTION_TIMEOUT)) {
      if (millis() - timeout > WIFI_CONNECTION_TIMEOUT) {
        Serial.println("");
        Serial.println("WARN - WiFi connection timeout!");
        return false;
      }
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    return true;
  }
  else {
    return false;
  }
}

void wifiDisconnect() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
