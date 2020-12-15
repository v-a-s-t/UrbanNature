#define MIC_SAMPLE 20

void setupSensors() {
  pinMode(SENSOR_EN, OUTPUT);
  digitalWrite(SENSOR_EN, LOW);

  // LTR559 requires at least 100ms before initialising
  delay(300);
  LTR559_begin();
  BME280_begin();
}

void enableSensors() {
  digitalWrite(SENSOR_EN, HIGH);
}

void disableSensors() {
  digitalWrite(SENSOR_EN, LOW);
}

float getTemp() {
  return BME280_getTemp();
}

float getHumidity() {
  return BME280_getHumidity();
}

float getPressure() {
  return BME280_getPressure();
}

float getAltitude() {
  return BME280_getAltitude();
}

int getNoise() {
  double micAverage = 0;
  for (byte i = 0; i < MIC_SAMPLE; i++) {
    micAverage = micAverage + analogRead(MIC_SENSE);
    delay(1);
  }
  micAverage = micAverage / MIC_SAMPLE;
  return (int)micAverage;
}

int getOxidising() {

}

int getReducing() {

}

int getNh3() {

}

float getLight() {
  return (getLux());
}

void printAllSensors() {
  Serial.print("Temp: ");
  Serial.println(getTemp());

  Serial.print("Humidity: ");
  Serial.println(getHumidity());

  Serial.print("Pressure: ");
  Serial.println(getPressure());

  Serial.print("Altitude: ");
  Serial.println(getAltitude());
}
