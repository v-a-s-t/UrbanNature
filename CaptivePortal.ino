class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      //we can handle anything!
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
      Serial.print("handleRequest: ");
      Serial.println(request->url());

      if (request->method() == HTTP_GET) {
        if (request->url() == "/scan") getScan(request);
        if (request->url() == "/settings") getSettings(request);
        else if (SPIFFS.exists(request->url())) sendFile(request, request->url());
        else if (request->url().endsWith(".html") || request->url().endsWith("/") || request->url().endsWith("generate_204") || request->url().endsWith("redirect"))  {
          sendFile(request, "/index.html");
        }
        else if (request->url().endsWith("connecttest.txt") || request->url().endsWith("ncsi.txt")) {
          request->send(200, "text/plain", "Microsoft NCSI");
        } else if (strstr(request->url().c_str(), "generate_204_") != NULL) {
          Serial.println("you must be huawei!");
          sendFile(request, "/index.html");
        }
        else {
          request->send(304);
        }
      }
    }

    void handleBody(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      Serial.print("handleBody: ");
      Serial.println(request->url());

      if (request->method() == HTTP_POST) {
        if (request->url() == "/settings") {
          String json = "";
          for (int i = 0; i < len; i++) json += char(data[i]);
          // TODO save settings
          DynamicJsonDocument settings(1536);
          deserializeJson(settings, json);
          serializeJson(settings, Serial);
          Serial.println();
          if (settings.containsKey("ssid"))
            ssid = settings["ssid"].as<String>();
          if (settings.containsKey("pass"))
            pass = settings["pass"].as<String>();
          user = settings["user"].as<String>();
          aio_key = settings["aio_key"].as<String>();
          startHour = settings["startHour"];
          interval = settings["interval"];
          if (settings.containsKey("lat"))
            lat = settings["lat"].as<String>();
          if (settings.containsKey("lon"))
            lon = settings["lon"].as<String>();
          sensorFeeds.clear();
          sensorFeeds = settings["sensorFeeds"];
          serializeJson(sensorFeeds, Serial);
          Serial.println();
          savePreferences();
          request->send(200);
          reset();
        }
        else {
          request->send(404);
        }
      }
    }

    void sendFile(AsyncWebServerRequest * request, String path) {
      Serial.println("handleFileRead: " + path);

      if (SPIFFS.exists(path)) {
        request->send(SPIFFS, path, getContentType(path));
      }
      else {
        request->send(404);
        Serial.println("File requested not found.");
      }
    }

    String getContentType(String filename) {
      if (filename.endsWith(".htm")) return "text/html";
      else if (filename.endsWith(".html")) return "text/html";
      else if (filename.endsWith(".css")) return "text/css";
      else if (filename.endsWith(".js")) return "application/javascript";
      else if (filename.endsWith(".png")) return "image/png";
      else if (filename.endsWith(".gif")) return "image/gif";
      else if (filename.endsWith(".jpg")) return "image/jpeg";
      else if (filename.endsWith(".ico")) return "image/x-icon";
      else if (filename.endsWith(".xml")) return "text/xml";
      else if (filename.endsWith(".pdf")) return "application/x-pdf";
      else if (filename.endsWith(".zip")) return "application/x-zip";
      else if (filename.endsWith(".gz")) return "application/x-gzip";
      else if (filename.endsWith(".json")) return "application/json";
      return "text/plain";
    }

    void getScan(AsyncWebServerRequest * request) {
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      response->print(returnScan());
      request->send(response);
    }

    void getSettings(AsyncWebServerRequest * request) {
      AsyncResponseStream *response = request->beginResponseStream("application/json");

      DynamicJsonDocument settings(1024);
      settings["user"] = user;
      settings["aio_key"] = aio_key;
      settings["startHour"] = startHour;
      settings["interval"] = interval;
      settings["lat"] = lat;
      settings["lon"] = lon;
      settings["sensorFeeds"] = sensorFeeds;
      String settingsString;
      serializeJson(settings, settingsString);
      response->print(settingsString);
      request->send(response);
    }
};

void setupCaptivePortal() {
  createAP();
  dnsServer.start(DNS_PORT, "*", apIP);
  apServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  apServer.begin();
}

void captivePortalHandler() {
  dnsServer.processNextRequest();
}

void createAP() {
  ap_ssid = "UNP-" + generateID();
  Serial.print("Wifi name:");
  Serial.println(ap_ssid);

  WiFi.mode(WIFI_AP);
  delay(2000);

  WiFi.persistent(false);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
}
