#define BATTERY_SAMPLE 10

double readBatteryVoltage() {
  double voltageIn;
  double voltsPerStep = 3.3 / 4095.0;
  for (int i = 0; i < BATTERY_SAMPLE; i++) {
    voltageIn = voltageIn + analogRead(BATTERY_READ);
    delay(10);
  }
  voltageIn = voltageIn / BATTERY_SAMPLE;

  voltageIn = (voltageIn * voltsPerStep) * 2;

  return voltageIn;
}

bool batteryDetected = false;

void batteryCheck() {
  if (readBatteryVoltage() < 3.65 && batteryDetected) {
    Serial.println("Battery Low");
  }
}

void setupPins() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void checkButtonOnStartUp() {
  if (digitalRead(BUTTON) == 0) {
    Serial.println("Opening Captive portal");
    usingCaptivePortal =  true;
  } else {
    usingCaptivePortal = false;
  }
  pinMode(PMS_RST, OUTPUT);
  digitalWrite(PMS_RST, LOW);
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


#define CHARGECHECKMILLIS 1000
unsigned long chargeCheckMillis = 10000;
bool chargeStatus = false;

bool isCharging() {
   if (millis() - chargeCheckMillis > CHARGECHECKMILLIS) {
      chargeCheckMillis = millis();
      chargeStatus = IP5306_GetPowerSource();
   }
  return (chargeStatus);
}

#define CHARGEBLINKTIME 1000
unsigned long chargeBlinkMillis;
bool blinkState = false;
#define CHARGEMINIBLINKTIME 800

void chargingBlink() {
  if (millis() - chargeBlinkMillis > CHARGEBLINKTIME) {
    if (!blinkState) {
      blinkState = true;
      digitalWrite(LED, blinkState);
    }
    if (millis() - chargeBlinkMillis > (CHARGEMINIBLINKTIME + CHARGEMINIBLINKTIME)) {
      blinkState = false;
      chargeBlinkMillis = millis();
      digitalWrite(LED, blinkState);
    }
  }

}
