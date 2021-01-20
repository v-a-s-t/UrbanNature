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
  pinMode(PMS_EN, OUTPUT);

  // LTR559 requires at least 100ms before initialising
  delay(300);
  LTR559_begin();
  BME280_begin();
  PMS5003_begin();
}

void enableSensors() {
  digitalWrite(SENSOR_EN, HIGH);
  digitalWrite(PMS_EN, HIGH);
  digitalWrite(PMS_RST, HIGH);
}

void disableSensors() {
  digitalWrite(SENSOR_EN, LOW);
  digitalWrite(PMS_EN, LOW);
  digitalWrite(PMS_RST, LOW);
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

  return (int)analogRead(MIC_SENSE);
}


float getOxidising() {
  return mics6814_readOx();
}

float getReducing() {
  return mics6814_readRed();
}

float getNh3() {
  return mics6814_readNH3();
}

float getLight() {
  return (getLux());
}

int getParticle(byte type) {
  switch (type) {
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
  switch (type) {
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
  switch (type) {
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
  else {
    Serial.println("No PMS data read.");
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

  Serial.print("Lux: ");
  Serial.println(getLight());

  Serial.print("Noise Level: ");
  Serial.println(getNoise());

  Serial.print("Oxidising: ");
  Serial.println(getOxidising());

  Serial.print("Reducing: ");
  Serial.println(getReducing());

  Serial.print("Nitrous Oxide: ");
  Serial.println(getNh3());

  readPMS5003();
  printPmsData();
}

float noiseSample = 0;
int noiseTick = 0;
int noiseSampleCount = 0;
bool isSamplingNoise = false;
bool noiseSampleReady = false;

#define SAMPLE_TIME 30
#define NOISE_THRESHOLD 100

//Return arbituary number 0 (completely silent) to 1 (extremely noisy)
//TODO: UPDATE TO MIN MAX PEAK DETECTION
void sampleMic(int noiseLevel) {
  if (noiseLevel > NOISE_THRESHOLD) {
    noiseTick++;
  }
  noiseSampleCount++;
  if (noiseSampleCount == SAMPLE_TIME) {
    noiseSample = (float)noiseTick / SAMPLE_TIME;
    noiseSampleReady = true;
  }
}

int lightSample = 0;
int lightSampleCount = 0;
int _lightLevel = 0;
int prevLightLevel = 0;
int sampleTime = SAMPLE_TIME;
bool lightSampleReady = false;

void sampleLight(int lightLevel) {
  _lightLevel = lightLevel;
  if (_lightLevel == 0) {
    _lightLevel = prevLightLevel;
    if (_lightLevel == 0) {
      if (sampleTime > 0) {
        sampleTime --;
      }
    }
  }
  lightSample = _lightLevel + lightSample;
  prevLightLevel = _lightLevel;
  lightSampleCount++;
  if (lightSampleCount == SAMPLE_TIME) {
    lightSample = (int)lightSample / sampleTime;
    lightSampleReady = true;
  }
}

int bmeSampleCount = 0;
float tempSample;
float presSample;
float altSample;
float humSample;
float _temp, _pres, _alt, _hum;
float prevTemp, prevPres, prevAlt, prevHum;
bool BME280Ready = false;

void sampleBME280(float temp, float pres, float alt, float hum) {
  _temp = temp;
  _pres = pres;
  _alt = alt;
  _hum = hum;
  if (_temp == 0) {
    _temp = prevTemp;
  }
  if (_pres == 0) {
    _pres = prevPres;
  }
  if (_alt == 0) {
    _alt = prevAlt;
  }
  if (_hum == 0) {
    _hum = prevHum;
  }
  tempSample = _temp + tempSample;
  presSample = _pres + presSample;
  altSample = _alt + altSample;
  humSample = _hum + humSample;
  prevTemp = _temp;
  prevPres = _pres;
  prevAlt = _alt;
  prevHum = _hum;
  bmeSampleCount++;
  if (bmeSampleCount == SAMPLE_TIME) {
    tempSample = tempSample / SAMPLE_TIME;
    presSample = presSample / SAMPLE_TIME;
    altSample = altSample / SAMPLE_TIME;
    humSample = humSample / SAMPLE_TIME;
    BME280Ready = true;
  }
}

void SampleCycle1() {
  if (BME280Ready && lightSampleReady && noiseSampleReady) {
    Serial.print("Temperature Sampling: ");
    Serial.println(tempSample);
    Serial.print("Pressure Sampling: ");
    Serial.println(presSample);
    Serial.print("Altitude Sampling: ");
    Serial.println(altSample);
    Serial.print("Humidity Sampling: ");
    Serial.println(humSample);
    Serial.print("Light Sampling: ");
    Serial.println(lightSample);
    Serial.print("Microphone Sampling: ");
    Serial.println(noiseSample);
  }
}

void testSampleAllSensors() {

  if ((sensorFeeds.containsKey("sensor_temp")) || (sensorFeeds.containsKey("sensor_humidity")) || (sensorFeeds.containsKey("sensor_pressure")) || (sensorFeeds.containsKey("sensor_altitude"))) {
    sampleBME280();
  }
  if (sensorFeeds.containsKey("sensor_light")) {
    sampleLux();
  }

  enableSensors();
  delay(2000);

  if (sensorFeeds.containsKey("sensor_noise")) {
    sampleMicPP();
  }
  if ((sensorFeeds.containsKey("sensor_p03um")) || (sensorFeeds.containsKey("sensor_p05um")) || (sensorFeeds.containsKey("sensor_p10um")) || (sensorFeeds.containsKey("sensor_p25um")) || (sensorFeeds.containsKey("sensor_p50um")) || (sensorFeeds.containsKey("sensor_p100um")) || (sensorFeeds.containsKey("sensor_pm10")) || (sensorFeeds.containsKey("sensor_pm25")) || (sensorFeeds.containsKey("sensor_pm100"))) {
    for (int i = 0; i < 100; i ++) {
      readPMS5003();
      delay(1000);
    }
  }
  if ((sensorFeeds.containsKey("sensor_oxidising")) || (sensorFeeds.containsKey("sensor_reducing")) || (sensorFeeds.containsKey("sensor_nh3"))) {
    sampleMICS6814();
  }
  disableSensors();

#ifdef DEBUG_OUTPUT
  Serial.println("Sensor Sampling Complete!");
  Serial.print("Temp: ");
  Serial.println(temperatureSample);
  Serial.print("Hum: ");
  Serial.println(humiditySample);
  Serial.print("Pressure: ");
  Serial.println(pressureSample);
  Serial.print("Altitude: ");
  Serial.println(altitudeSample);
  Serial.print("Mic Level: ");
  Serial.println(micPPSample);
  Serial.print("Lux: ");
  Serial.println(luxSample);
  Serial.print("Reducing Gases: ");
  Serial.println(redSample);
  Serial.print("Oxidising Gases: ");
  Serial.println(oxSample);
  Serial.print("NH3: ");
  Serial.println(nh3Sample);
  Serial.println("Concentration Units (standard)");
  Serial.print("PM 1.0: "); Serial.print(getConcentrationStandard(pm10));
  Serial.print("\t\tPM 2.5: "); Serial.print(getConcentrationStandard(pm25));
  Serial.print("\t\tPM 10: "); Serial.println(getConcentrationStandard(pm100));
  Serial.println("---------------------------------------");
  Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(getParticle(p03um));
  Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(getParticle(p05um));
  Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(getParticle(p10um));
  Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(getParticle(p25um));
  Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(getParticle(p50um));
  Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(getParticle(p100um));
  Serial.println("---------------------------------------");
#endif
}

void debugSensors() {
#ifdef DEBUG_PMS
  pinMode(PMS_RST, OUTPUT);
  setupSensors();
  enableSensors();
  Serial.println("The PMS will need to warm up before data is valid");
  delay(2000);
  while (1) {
    printPmsData();
    delay(1000);
  }
#endif
#ifdef DEBUG_LTR559
  setupSensors();
  enableSensors();
  while (1) {
    Serial.println(getLux());
    delay(1000);
  }
#endif
#ifdef DEBUG_MICS6814
  setupSensors();
  enableSensors();
  Serial.println("The Gas sensor will need to heat up at least 30s before data is valid");
  delay(2000);
  while (1) {
    Serial.print("RED: ");
    Serial.println(mics6814_readRed());
    Serial.print("OX: ");
    Serial.println(mics6814_readOx());
    Serial.print("NH3: ");
    Serial.println(mics6814_readNH3());
    Serial.println();
    delay(1000);
  }
#endif
#ifdef DEBUG_MICROPHONE
  setupSensors();
  enableSensors();
  while (1) {
    Serial.println(getMicPeak());
    delay(10);
  }
#endif
#ifdef DEBUG_BME280
  setupSensors();
  enableSensors();
  while (1) {
    Serial.print("Temp: ");
    Serial.println(getTemp());
    Serial.print("Humidity: ");
    Serial.println(getHumidity());
    Serial.print("Pressure: ");
    Serial.println(getPressure());
    Serial.print("Altitude: ");
    Serial.println(getAltitude());
    Serial.println();
    delay(1000);
  }
#endif
}
