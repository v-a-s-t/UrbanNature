void setupSensors() {
  pinMode(SENSOR_EN, OUTPUT);
  digitalWrite(SENSOR_EN, LOW);
  //LTR559 requires atleast 100ms before initialising
  delay(300);

  LTR559_begin();
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
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}

int getNoise() {
  return analogRead(MIC_SENSE);
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
