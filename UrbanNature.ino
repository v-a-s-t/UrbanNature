#include "Config.h"
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

//  setupSensors();
//  enableSensors();

  setupModem();
  connect();
}

void loop() {
  delay(100);
}
