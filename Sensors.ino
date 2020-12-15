void setupSensors() {
  pinMode(SENSOR_EN, OUTPUT);
  digitalWrite(SENSOR_EN, LOW);
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
  return analogRead(MIC_SENSE);
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
  return(getLux());
}
