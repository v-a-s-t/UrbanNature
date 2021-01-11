#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

void BME280_begin() {
  byte status = bme.begin(0x76, &Wire);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
  }
}

float BME280_getTemp() {
  return bme.readTemperature();
}

float BME280_getPressure() {
  return bme.readPressure() / 100.0F;
}

float BME280_getAltitude() {
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}

float BME280_getHumidity() {
  return bme.readHumidity();
}


void sampleBME280() {
  double tempOut, humOut, pressOut,altOut;
  for (int i = 0; i < 30; i ++) {
    tempOut = tempOut + BME280_getTemp();
    humOut = humOut + BME280_getHumidity();
    pressOut = pressOut + BME280_getPressure();
    altOut = altOut + BME280_getAltitude();
    delay(1000);
  }
  temperatureSample = tempOut / 30.0;
  humiditySample = humOut / 30.0;
  pressureSample = pressOut / 30.0;
  altitudeSample = altOut / 30;
}
