#define BATTERY_SAMPLE 10

float readBatteryVoltage() {
  float voltageIn;
  float voltsPerStep = 3.3 / 4095.0;
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

void checkButtonOnStarUp(){
  if(digitalRead(BUTTON) == 0){
    Serial.println("Opening Captive portal");
  }
}
