#define MAX_WIFI_NETWORKS 10

bool wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) 
    return true;
    
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
    Serial.println("No WiFi credentials in preferences.");
    return false;
  }
}

void wifiDisconnect() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

String jsonString;


void getScanAsJsonString() {
  
  StaticJsonDocument<1000> jsonDoc;
  getScanAsJson(jsonDoc);
  serializeJson(jsonDoc[0], jsonString);

}

String returnScan(){
  return (jsonString);
}

void getScanAsJson(JsonDocument& jsonDoc) {
  JsonArray networks = jsonDoc.createNestedArray();

  int n = WiFi.scanNetworks();
  if(n > MAX_WIFI_NETWORKS)
    n = MAX_WIFI_NETWORKS;
  //Array is ordered by signal strength - strongest first
  for (int i = 0; i < n; ++i) {
    String networkSSID = WiFi.SSID(i);
    if (networkSSID.length() <= SSID_MAX_LENGTH) {
      JsonObject network  = networks.createNestedObject();
      network["SSID"] = WiFi.SSID(i);
      network["BSSID"] = WiFi.BSSIDstr(i);
      network["RSSI"] = WiFi.RSSI(i);
    }
  }
}
