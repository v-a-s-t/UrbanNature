/*TODO
  - Fix Lux Calculation
  - Convert Sound level to Decibel
*/

#include "Config.h"
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  setupSensors();
  enableSensors();

  // setupModem();
  // modemConnect();
  // postIntToFeed(2, "test");
  // modemDisconnect();
  // modemPoweroff();

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}

void loop() {
  //printAllSensors();
  printPmsData();
  delay(1000);
  Serial.println(getNoise());
}
