#define BATTERY_SAMPLE 10

float readBatteryVoltage() {
  double voltageIn;
  for (int i = 0; i < BATTERY_SAMPLE; i++) {
    voltageIn = voltageIn + analogRead(BATTERY_READ);
    delay(10);
  }
  voltageIn = voltageIn / 10.0;

  voltageIn = (voltageIn * 2) / 4095.0;
  voltageIn = voltageIn * 3.3;

  return (float)voltageIn;
}

void setupPins() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

bool checkButtonOnStartUp() {
  if (digitalRead(BUTTON) == 0) {
    Serial.println("Opening Captive portal");
    usingCaptivePortal =  true;
  } else {
    usingCaptivePortal = false;
  }
}


unsigned long resetMillis;
#define RESET_TIMEOUT 2000

void reset() {
  resetMillis = millis();

  while (millis() - resetMillis < RESET_TIMEOUT) {
    yield();
  }
  ESP.restart();

}

String generateID() {
  //https://github.com/espressif/arduino-esp32/issues/3859#issuecomment-689171490
  uint64_t chipID = ESP.getEfuseMac();
  uint32_t low = chipID % 0xFFFFFFFF;
  uint32_t high = (chipID >> 32) % 0xFFFFFFFF;
  String out = String(low);
  return  out;
}
