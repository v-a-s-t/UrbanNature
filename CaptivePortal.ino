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
        if (request->url() == "/scan")   getScan(request);
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
        if (request->url() == "/wifi") {
          String json = "";
          for (int i = 0; i < len; i++)  json += char(data[i]);

          StaticJsonDocument<1024> credentialsJsonDoc;
          if (!deserializeJson(credentialsJsonDoc, json)) {
            // TODO save credentials
            request->send(200);
          }
        }
        else if (request->url() =="/feeds") {
          // TODO save feed
          request->send(200);
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

      // TODO response->print(getScanAsJsonString());
      request->send(response);
    }
};

void setupCaptivePortal() {
  dnsServer.start(DNS_PORT, "*", apIP);
}
