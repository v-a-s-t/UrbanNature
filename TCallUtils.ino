#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

// setPowerBoostKeepOn
bool setupPMU()
{
  bool en = true;
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en) {
    Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    Wire.write(0x35); // 0x37 is default reg value
  }
  return Wire.endTransmission() == 0;
}

void goToSleep(int seconds) {
  Serial.print("Going to sleep for ");
  Serial.print(seconds);
  Serial.println(" seconds.");
  esp_sleep_enable_timer_wakeup(seconds * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}


void goToSleepMinutes(int minutes) {
  goToSleep(minutes * 60);
}

void turnOffPMU()
{
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  Wire.write(0x36); // Set bit1: 1 enable 0 disable boost keep on
  Wire.endTransmission();
}

#define IP5306_REG_READ_0   0x70

bool isPluggedIn() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_READ_0);
  Wire.endTransmission();
  Wire.requestFrom(IP5306_ADDR, 1);
  while (Wire.available() < 1);
  byte dataIn = Wire.read();
  if (bitRead(dataIn, 3) == 1) {
    Serial.println("Device powered by USB");
    return true;
  } else {
    Serial.println("Device powered by BAT");
    return false;
  }
}
