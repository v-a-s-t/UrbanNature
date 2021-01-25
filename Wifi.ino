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
    Serial.println("No WiFi credentials in preferences.");
    return false;
  }
}

void wifiDisconnect() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

String getScanAsJsonString() {
  String jsonString;

  StaticJsonDocument<1000> jsonDoc;
  getScanAsJson(jsonDoc);
  serializeJson(jsonDoc[0], jsonString);

  return (jsonString);
}

void getScanAsJson(JsonDocument& jsonDoc) {
  JsonArray networks = jsonDoc.createNestedArray();

  int n = WiFi.scanNetworks();

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


bool wifiPost(String requestData, String url) {
  WiFiClient client;
  bool success = false;
  Serial.print("Connecting to ");
  Serial.print(server);
  if (!client.connect(server, port)) {
    Serial.println(" fail");
    Serial.println("Error: Could not connect to server.");
    return success;
  }
  else {
    Serial.println(" OK");
  }
  client.print(String("POST ") + url + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(requestData.length());
  client.print("X-AIO-Key: ");
  client.println(aio_key);
  client.println();
  client.println(requestData);

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 20000L) {
    // Print available data (HTTP response from server)
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
      success = true;
    }
    // TODO handle HTTP errors and return false.
  }

  client.stop();
  Serial.println(F("Server disconnected"));
  if (success) {
    Serial.println("Success!");
  }
  else {
    Serial.println("Error: No reply from server.");
  }
  return success;
}
