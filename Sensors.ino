#define MIC_SAMPLE 20

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

int getTemp() {

}

int getHumidity() {

}

int getPressure() {

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

int getAltitude() {

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
