TinyGsmClient client(modem);

// Posts a payload to a url on adafruit.io
void modemPost(String requestData, String url) {
  Serial.print("Connecting to ");
  Serial.print(server);
  if (!client.connect(server, port)) {
    Serial.println(" fail");
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
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data (HTTP response from server)
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
    }
    // TODO handle HTTP errors and return false.
  }

  client.stop();
  Serial.println(F("Server disconnected"));
}

// Posts an integer to an adafruit.io feed
void postIntToFeed(int data, String feed) {
  String payload = "{\"value\": " + String(data) + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  modemPost(payload, url);
}

// Posts a float to an adafruit.io feed
void postFloatToFeed(float data, String feed) {
  String payload = "{\"value\": " + String(data) + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  modemPost(payload, url);
}

// Posts a float to an adafruit.io feed with the location
void postFloatToFeed(float data, float lat, float lon, String feed) {
  String payload = "{\"value\": " + String(data)  +  ",\"lat\": " + String(lat) + ",\"lon\": " + String(lon) + "}";

 "{\"value\":\"1\",\"lat\":1351824120,\"lon\":1351824120}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  modemPost(payload, url);
}

// Posts an integer to an adafruit.io feed with the location
void postIntToFeed(int data, float lat, float lon, String feed) {
  String payload = "{\"value\": " + String(data)  + ",\"lat\": " + String(lat) + ",\"lon\": " + String(lon) + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  modemPost(payload, url);
}
