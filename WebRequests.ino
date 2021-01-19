TinyGsmClient client(modem);

// Posts a payload to a url on adafruit.io
bool modemPost(String requestData, String url) {
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
  while (client.connected() && millis() - timeout < 10000L) {
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

// Posts an integer to an adafruit.io feed
void postIntToFeed(int data, String feed) {
  String payload = "{\"value\": " + String(data) + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  bool success = false;
  for (int i = 0; i < POST_MAX_RETRIES; i++) {
    success = modemPost(payload, url);
    if (success) break;
    else {
      setupModem();
      modemConnect();
      Serial.print("Retrying");
    }
  }
}

// Posts a float to an adafruit.io feed
void postFloatToFeed(float data, String feed) {
  String payload = "{\"value\": " + String(data) + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  bool success = false;
  for (int i = 0; i < POST_MAX_RETRIES; i++) {
    success = modemPost(payload, url);
    if (success) break;
    else {
      setupModem();
      modemConnect();
      Serial.print("Retrying");
    }
  }
}

// Posts a float to an adafruit.io feed with the location
void postFloatToFeed(float data, String lat, String lon, String feed) {
  String payload = "{\"value\": " + String(data)  +  ",\"lat\": " + lat + ",\"lon\": " + lon + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  bool success = false;
  for (int i = 0; i < POST_MAX_RETRIES; i++) {
    success = modemPost(payload, url);
    if (success) break;
    else {
      setupModem();
      modemConnect();
      Serial.print("Retrying");
    }
  }
}

// Posts an integer to an adafruit.io feed with the location
void postIntToFeed(int data, String lat, String lon, String feed) {
  String payload = "{\"value\": " + String(data)  + ",\"lat\": " + lat + ",\"lon\": " + lon + "}";
  String url = "/api/v2/" + user + "/feeds/" + feed + "/data";
  bool success = false;
  for (int i = 0; i < POST_MAX_RETRIES; i++) {
    success = modemPost(payload, url);
    if (success) break;
    else {
      setupModem();
      modemConnect();
      Serial.print("Retrying");
    }
  }
}

void postSensorsToAIO() {

  if (sensorFeeds.containsKey("sensor_temp")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(temperatureSample, lat, lon, sensorFeeds["sensor_temp"]);
    } else {
      postFloatToFeed(temperatureSample, sensorFeeds["sensor_temp"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_humidity")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(humiditySample, lat, lon, sensorFeeds["sensor_humidity"]);
    } else {
      postFloatToFeed(humiditySample, sensorFeeds["sensor_humidity"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_pressure")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(pressureSample, lat, lon, sensorFeeds["sensor_pressure"]);
    } else {
      postFloatToFeed(pressureSample, sensorFeeds["sensor_pressure"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_altitude")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(altitudeSample, lat, lon, sensorFeeds["sensor_altitude"]);
    } else {
      postFloatToFeed(altitudeSample, sensorFeeds["sensor_pressure"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_noise")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(micPPSample, lat, lon, sensorFeeds["sensor_noise"]);
    } else {
      postFloatToFeed(micPPSample, sensorFeeds["sensor_noise"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_light")) {
    if (lat != "" && lon != "") {
      postIntToFeed(luxSample, lat, lon, sensorFeeds["sensor_light"]);
    } else {
      postIntToFeed(luxSample, sensorFeeds["sensor_light"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_oxidising")) {
    if (lat != "" && lon != "") {
      postFloatToFeed((float)oxSample, lat, lon, sensorFeeds["sensor_oxidising"]);
    } else {
      postFloatToFeed((float)oxSample, sensorFeeds["sensor_oxidising"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_reducing")) {
    if (lat != "" && lon != "") {
      postFloatToFeed((float)redSample, lat, lon, sensorFeeds["sensor_reducing"]);
    } else {
      postFloatToFeed((float)redSample, sensorFeeds["sensor_reducing"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_nh3")) {
    if (lat != "" && lon != "") {
      postFloatToFeed((float)nh3Sample, lat, lon, sensorFeeds["sensor_nh3"]);
    } else {
      postFloatToFeed((float)nh3Sample, sensorFeeds["sensor_nh3"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p03um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p03um), lat, lon, sensorFeeds["sensor_p03um"]);
    } else {
      postFloatToFeed(getParticle(p03um), sensorFeeds["sensor_p03um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p05um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p05um), lat, lon, sensorFeeds["sensor_p05um"]);
    } else {
      postFloatToFeed(getParticle(p05um), sensorFeeds["sensor_p05um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p10um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p10um), lat, lon, sensorFeeds["sensor_p10um"]);
    } else {
      postFloatToFeed(getParticle(p10um), sensorFeeds["sensor_p10um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p25um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p25um), lat, lon, sensorFeeds["sensor_p25um"]);
    } else {
      postFloatToFeed(getParticle(p25um), sensorFeeds["sensor_p25um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p50um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p50um), lat, lon, sensorFeeds["sensor_p50um"]);
    } else {
      postFloatToFeed(getParticle(p50um), sensorFeeds["sensor_p50um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_p100um")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(p100um), lat, lon, sensorFeeds["sensor_p100um"]);
    } else {
      postFloatToFeed(getParticle(p100um), sensorFeeds["sensor_p100um"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_pm10")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(pm10), lat, lon, sensorFeeds["sensor_pm10"]);
    } else {
      postFloatToFeed(getParticle(pm10), sensorFeeds["sensor_pm10"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_pm25")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(pm25), lat, lon, sensorFeeds["sensor_pm25"]);
    } else {
      postFloatToFeed(getParticle(pm25), sensorFeeds["sensor_pm25"]);
    }
  }
  delay(10);
  if (sensorFeeds.containsKey("sensor_pm100")) {
    if (lat != "" && lon != "") {
      postFloatToFeed(getParticle(pm100), lat, lon, sensorFeeds["sensor_pm100"]);
    } else {
      postFloatToFeed(getParticle(pm100), sensorFeeds["sensor_pm100"]);
    }
  }
  delay(10);
}


void sendBatterylevel() {
  if (sensorFeeds.containsKey("battery_percentage")) {
    if (lat != "" && lon != "") {
      postIntToFeed(getBatteryPercentage(), lat, lon, sensorFeeds["battery_percentage"]);
    } else {
      postIntToFeed(getBatteryPercentage(), sensorFeeds["battery_percentage"]);
    }
  }
}
