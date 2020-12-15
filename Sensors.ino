#define MIC_SAMPLE 20

enum particleTypes {
  p03um,
  p05um,
  p10um,
  p25um,
  p50um,
  p100um
};

enum concentrationTypes {
  pm10,
  pm25,
  pm100
};

void setupSensors() {
  pinMode(SENSOR_EN, OUTPUT);
  digitalWrite(SENSOR_EN, LOW);

  // LTR559 requires at least 100ms before initialising
  delay(300);
  LTR559_begin();
  BME280_begin();
  PMS5003_begin();
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

int getParticle(byte type) {
  switch(type) {
    case p03um:
      return particulateData.particles_03um;
      break;
    case p05um:
      return particulateData.particles_05um;
      break;
    case p10um:
      return particulateData.particles_10um;
      break;
    case p25um:
      return particulateData.particles_25um;
      break;
    case p50um:
      return particulateData.particles_50um;
      break;
    case p100um:
      return particulateData.particles_100um;
      break;
  }
}

int getConcentrationStandard(byte type) {
  switch(type) {
    case pm10:
      return particulateData.pm10_standard;
      break;
    case pm25:
      return particulateData.pm25_standard;
      break;
    case pm100:
      return particulateData.pm100_standard;
      break;
  }
}

int getConcentrationEnviro(byte type) {
  switch(type) {
    case pm10:
      return particulateData.pm10_env;
      break;
    case pm25:
      return particulateData.pm25_env;
      break;
    case pm100:
      return particulateData.pm100_env;
      break;
  }
}

boolean readPMS5003() {
  return readPMSdata(&Serial2);
}

void printPmsData() {
  if (readPMS5003()) {
    // reading data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(getConcentrationStandard(pm10));
    Serial.print("\t\tPM 2.5: "); Serial.print(getConcentrationStandard(pm25));
    Serial.print("\t\tPM 10: "); Serial.println(getConcentrationStandard(pm100));
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(getConcentrationEnviro(pm10));
    Serial.print("\t\tPM 2.5: "); Serial.print(getConcentrationEnviro(pm25));
    Serial.print("\t\tPM 10: "); Serial.println(getConcentrationEnviro(pm100));
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(getParticle(p03um));
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(getParticle(p05um));
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(getParticle(p10um));
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(getParticle(p25um));
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(getParticle(p50um));
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(getParticle(p100um));
    Serial.println("---------------------------------------");
  }
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

  readPMS5003();
  printPmsData();
}
