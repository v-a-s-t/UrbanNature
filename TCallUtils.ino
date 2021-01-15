#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

#define IP5306_REG_SYS_CTL0  0x00
#define IP5306_REG_SYS_0    0x00
#define IP5306_REG_SYS_1    0x01
#define IP5306_REG_SYS_2    0x02
#define IP5306_REG_CHG_0    0x20
#define IP5306_REG_CHG_1    0x21
#define IP5306_REG_CHG_2    0x22
#define IP5306_REG_CHG_3    0x23
#define IP5306_REG_CHG_4    0x24
#define IP5306_REG_READ_0   0x70
#define IP5306_REG_READ_1   0x71
#define IP5306_REG_READ_2   0x72
#define IP5306_REG_READ_3   0x77
#define IP5306_REG_READ_4   0x78

#define IP5306_GetKeyOffEnabled()               ip5306_get_bits(IP5306_REG_SYS_0, 0, 1)
#define IP5306_SetKeyOffEnabled(v)              ip5306_set_bits(IP5306_REG_SYS_0, 0, 1, v)//0:dis,*1:en

#define IP5306_GetBoostOutputEnabled()          ip5306_get_bits(IP5306_REG_SYS_0, 1, 1)
#define IP5306_SetBoostOutputEnabled(v)         ip5306_set_bits(IP5306_REG_SYS_0, 1, 1, v)//*0:dis,1:en

#define IP5306_GetPowerOnLoadEnabled()          ip5306_get_bits(IP5306_REG_SYS_0, 2, 1)
#define IP5306_SetPowerOnLoadEnabled(v)         ip5306_set_bits(IP5306_REG_SYS_0, 2, 1, v)//0:dis,*1:en

#define IP5306_GetChargerEnabled()              ip5306_get_bits(IP5306_REG_SYS_0, 4, 1)
#define IP5306_SetChargerEnabled(v)             ip5306_set_bits(IP5306_REG_SYS_0, 4, 1, v)//0:dis,*1:en

#define IP5306_GetBoostEnabled()                ip5306_get_bits(IP5306_REG_SYS_0, 5, 1)
#define IP5306_SetBoostEnabled(v)               ip5306_set_bits(IP5306_REG_SYS_0, 5, 1, v)//0:dis,*1:en

#define IP5306_GetLowBatShutdownEnable()        ip5306_get_bits(IP5306_REG_SYS_1, 0, 1)
#define IP5306_SetLowBatShutdownEnable(v)       ip5306_set_bits(IP5306_REG_SYS_1, 0, 1, v)//0:dis,*1:en

#define IP5306_GetBoostAfterVin()               ip5306_get_bits(IP5306_REG_SYS_1, 2, 1)
#define IP5306_SetBoostAfterVin(v)              ip5306_set_bits(IP5306_REG_SYS_1, 2, 1, v)//0:Closed, *1:Open

#define IP5306_GetShortPressBoostSwitchEnable() ip5306_get_bits(IP5306_REG_SYS_1, 5, 1)
#define IP5306_SetShortPressBoostSwitchEnable(v) ip5306_set_bits(IP5306_REG_SYS_1, 5, 1, v)//*0:disabled, 1:enabled

#define IP5306_GetFlashlightClicks()            ip5306_get_bits(IP5306_REG_SYS_1, 6, 1)
#define IP5306_SetFlashlightClicks(v)           ip5306_set_bits(IP5306_REG_SYS_1, 6, 1, v)//*0:short press twice, 1:long press

#define IP5306_GetBoostOffClicks()              ip5306_get_bits(IP5306_REG_SYS_1, 7, 1)
#define IP5306_SetBoostOffClicks(v)             ip5306_set_bits(IP5306_REG_SYS_1, 7, 1, v)//*0:long press, 1:short press twice

#define IP5306_GetLightLoadShutdownTime()       ip5306_get_bits(IP5306_REG_SYS_2, 2, 2)
#define IP5306_SetLightLoadShutdownTime(v)      ip5306_set_bits(IP5306_REG_SYS_2, 2, 2, v)//0:8s, *1:32s, 2:16s, 3:64s

#define IP5306_GetLongPressTime()               ip5306_get_bits(IP5306_REG_SYS_2, 4, 1)
#define IP5306_SetLongPressTime(v)              ip5306_set_bits(IP5306_REG_SYS_2, 4, 1, v)//*0:2s, 1:3s

#define IP5306_GetChargingFullStopVoltage()     ip5306_get_bits(IP5306_REG_CHG_0, 0, 2)
#define IP5306_SetChargingFullStopVoltage(v)    ip5306_set_bits(IP5306_REG_CHG_0, 0, 2, v)//0:4.14V, *1:4.17V, 2:4.185V, 3:4.2V (values are for charge cutoff voltage 4.2V, 0 or 1 is recommended)

#define IP5306_GetChargeUnderVoltageLoop()      ip5306_get_bits(IP5306_REG_CHG_1, 2, 3)   //Automatically adjust the charging current when the voltage of VOUT is greater than the set value
#define IP5306_SetChargeUnderVoltageLoop(v)     ip5306_set_bits(IP5306_REG_CHG_1, 2, 3, v)//Vout=4.45V + (v * 0.05V) (default 4.55V) //When charging at the maximum current, the charge is less than the set value. Slowly reducing the charging current to maintain this voltage

#define IP5306_GetEndChargeCurrentDetection()   ip5306_get_bits(IP5306_REG_CHG_1, 6, 2)
#define IP5306_SetEndChargeCurrentDetection(v)  ip5306_set_bits(IP5306_REG_CHG_1, 6, 2, v)//0:200mA, 1:400mA, *2:500mA, 3:600mA

#define IP5306_GetVoltagePressure()             ip5306_get_bits(IP5306_REG_CHG_2, 0, 2)
#define IP5306_SetVoltagePressure(v)            ip5306_set_bits(IP5306_REG_CHG_2, 0, 2, v)//0:none, 1:14mV, *2:28mV, 3:42mV (28mV recommended for 4.2V)

#define IP5306_GetChargeCutoffVoltage()         ip5306_get_bits(IP5306_REG_CHG_2, 2, 2)
#define IP5306_SetChargeCutoffVoltage(v)        ip5306_set_bits(IP5306_REG_CHG_2, 2, 2, v)//*0:4.2V, 1:4.3V, 2:4.35V, 3:4.4V

#define IP5306_GetChargeCCLoop()                ip5306_get_bits(IP5306_REG_CHG_3, 5, 1)
#define IP5306_SetChargeCCLoop(v)               ip5306_set_bits(IP5306_REG_CHG_3, 5, 1, v)//0:BAT, *1:VIN

#define IP5306_GetVinCurrent()                  ip5306_get_bits(IP5306_REG_CHG_4, 0, 5)
#define IP5306_SetVinCurrent(v)                 ip5306_set_bits(IP5306_REG_CHG_4, 0, 5, v)//ImA=(v*100)+50 (default 2250mA)

#define IP5306_GetShortPressDetected()          ip5306_get_bits(IP5306_REG_READ_3, 0, 1)
#define IP5306_ClearShortPressDetected()        ip5306_set_bits(IP5306_REG_READ_3, 0, 1, 1)

#define IP5306_GetLongPressDetected()           ip5306_get_bits(IP5306_REG_READ_3, 1, 1)
#define IP5306_ClearLongPressDetected()         ip5306_set_bits(IP5306_REG_READ_3, 1, 1, 1)

#define IP5306_GetDoubleClickDetected()         ip5306_get_bits(IP5306_REG_READ_3, 2, 1)
#define IP5306_ClearDoubleClickDetected()       ip5306_set_bits(IP5306_REG_READ_3, 2, 1, 1)

#define IP5306_GetPowerSource()                 ip5306_get_bits(IP5306_REG_READ_0, 3, 1)//0:BAT, 1:VIN
#define IP5306_GetBatteryFull()                 ip5306_get_bits(IP5306_REG_READ_1, 3, 1)//0:CHG/DIS, 1:FULL
#define IP5306_GetOutputLoad()                  ip5306_get_bits(IP5306_REG_READ_2, 2, 1)//0:heavy, 1:light
#define IP5306_GetLevelLeds()                ((~ip5306_get_bits(IP5306_REG_READ_4, 4, 4)) & 0x0F)//LED[0-4] State (inverted)

#define IP5306_LEDS2PCT(byte) ((byte & 0x01 ? 25 : 0) + (byte & 0x02 ? 25 : 0) + (byte & 0x04 ? 25 : 0) + (byte & 0x08 ? 25 : 0))

int ip5306_get_reg(uint8_t reg) {
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(IP5306_ADDR, 1)) {
    return Wire.read();
  }
  return -1;
}

int ip5306_set_reg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(reg);
  Wire.write(value);
  if (Wire.endTransmission(true) == 0) {
    return 0;
  }
  return -1;
}

uint8_t ip5306_get_bits(uint8_t reg, uint8_t index, uint8_t bits) {
  int value = ip5306_get_reg(reg);
  if (value < 0) {
    Serial.printf("ip5306_get_bits fail: 0x%02x\n", reg);
    return 0;
  }
  return (value >> index) & ((1 << bits) - 1);
}

void ip5306_set_bits(uint8_t reg, uint8_t index, uint8_t bits, uint8_t value) {
  uint8_t mask = (1 << bits) - 1;
  int v = ip5306_get_reg(reg);
  if (v < 0) {
    Serial.printf("ip5306_get_reg fail: 0x%02x\n", reg);
    return;
  }
  v &= ~(mask << index);
  v |= ((value & mask) << index);
  if (ip5306_set_reg(reg, v)) {
    Serial.printf("ip5306_set_bits fail: 0x%02x\n", reg);
  }
}

void printIP5306Stats() {
  bool usb = IP5306_GetPowerSource();
  bool full = IP5306_GetBatteryFull();
  uint8_t leds = IP5306_GetLevelLeds();
  Serial.printf("IP5306: Power Source: %s, Battery State: %s, Battery Available: %u%%\n", usb ? "USB" : "BATTERY", full ? "CHARGED" : (usb ? "CHARGING" : "DISCHARGING"), IP5306_LEDS2PCT(leds));
}

// setPowerBoostKeepOn
bool setupPMU()
{
//  bool en = true;
//  Wire.begin(I2C_SDA, I2C_SCL);
//  Wire.beginTransmission(IP5306_ADDR);
//  Wire.write(IP5306_REG_SYS_CTL0);
//  if (en) {
//    Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
//  } else {
//    Wire.write(0x35); // 0x37 is default reg value
//  }
//  return Wire.endTransmission() == 0;
  IP5306_SetLightLoadShutdownTime(3);
  IP5306_SetBoostOutputEnabled(1);
}

void printIP5306Settings(){
    Serial.println("IP5306 Settings:");
    Serial.printf("  KeyOff: %s\n", IP5306_GetKeyOffEnabled()?"Enabled":"Disabled");
    Serial.printf("  BoostOutput: %s\n", IP5306_GetBoostOutputEnabled()?"Enabled":"Disabled");
    Serial.printf("  PowerOnLoad: %s\n", IP5306_GetPowerOnLoadEnabled()?"Enabled":"Disabled");
    Serial.printf("  Charger: %s\n", IP5306_GetChargerEnabled()?"Enabled":"Disabled");
    Serial.printf("  Boost: %s\n", IP5306_GetBoostEnabled()?"Enabled":"Disabled");
    Serial.printf("  LowBatShutdown: %s\n", IP5306_GetLowBatShutdownEnable()?"Enabled":"Disabled");
    Serial.printf("  ShortPressBoostSwitch: %s\n", IP5306_GetShortPressBoostSwitchEnable()?"Enabled":"Disabled");
    Serial.printf("  FlashlightClicks: %s\n", IP5306_GetFlashlightClicks()?"Long Press":"Double Press");
    Serial.printf("  BoostOffClicks: %s\n", IP5306_GetBoostOffClicks()?"Double Press":"Long Press");
    Serial.printf("  BoostAfterVin: %s\n", IP5306_GetBoostAfterVin()?"Open":"Not Open");
    Serial.printf("  LongPressTime: %s\n", IP5306_GetLongPressTime()?"3s":"2s");
    Serial.printf("  ChargeUnderVoltageLoop: %.2fV\n", 4.45 + (IP5306_GetChargeUnderVoltageLoop() * 0.05));
    Serial.printf("  ChargeCCLoop: %s\n", IP5306_GetChargeCCLoop()?"Vin":"Bat");
    Serial.printf("  VinCurrent: %dmA\n", (IP5306_GetVinCurrent() * 100) + 50);
    Serial.printf("  VoltagePressure: %dmV\n", IP5306_GetVoltagePressure()*14);
    Serial.printf("  ChargingFullStopVoltage: %u\n", IP5306_GetChargingFullStopVoltage());
    Serial.printf("  LightLoadShutdownTime: %u\n", IP5306_GetLightLoadShutdownTime());
    Serial.printf("  EndChargeCurrentDetection: %u\n", IP5306_GetEndChargeCurrentDetection());
    Serial.printf("  ChargeCutoffVoltage: %u\n", IP5306_GetChargeCutoffVoltage());
    Serial.println();
}

void goToSleep(unsigned long seconds) {
  turnOffPMU();
  disableSensors();
  Serial.print("Going to sleep for ");
  Serial.print(seconds);
  Serial.println(" seconds.");
  esp_sleep_enable_timer_wakeup(seconds * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void goToSleepMinutes(unsigned long minutes) {
  goToSleep(minutes * 60);
}

void turnOffPMU()
{
  IP5306_SetBoostOutputEnabled(0);
}

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

bool isBatteryFull() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_READ_1);
  Wire.endTransmission();
  Wire.requestFrom(IP5306_ADDR, 1);
  while (Wire.available() < 1);
  byte dataIn = Wire.read();
  if (bitRead(dataIn, 3) == 1) {
    Serial.println("Battery is fully charged");
    return true;
  } else {
    Serial.println("Battery is charging or discharged");
    return false;
  }
}

unsigned long prevChargeMillis;
#define CHARGECHECKINTERVAL 1000

void chargeCheck() {
  if (millis() - prevChargeMillis > CHARGECHECKINTERVAL) {
    Serial.print("Battery Voltage: ");
    Serial.println(readBatteryVoltage());
    prevChargeMillis = millis();
    isCharging = isPluggedIn();
    if (isCharging) {
      if (!DEBUG) {
        if (isBatteryFull()) {
          digitalWrite(LED, 1);
        } else {
          digitalWrite(LED, 1);
          delay(10);
          digitalWrite(LED, 0);
        }
      }
    } else {
      ESP.restart();
    }
  }


}
