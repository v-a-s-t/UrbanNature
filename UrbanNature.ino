//TODO Fix Lux Calculation

#include "Config.h"
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  //  setupSensors();
  //  enableSensors();
  LTR559_begin();
  // setupModem();
  // connect();
  delay(1000);

}

void loop() {
  Serial.println();
  delay(1000);
}
