#define ALS_CONTR_REG 0x80
#define PS_CONTR_REG 0x81
#define PS_LED_REG 0x82
#define PS_MEAS_RATE_REG 0x84
#define ALS_MEAS_RATE_REG 0x85
#define ALS_DATA_CH1_L 0x88
#define ALS_DATA_CH1_H 0x89
#define ALS_DATA_CH0_L 0x8A
#define ALS_DATA_CH0_H 0x8B
#define ALS_PS_STATUS_REG 0x8C
#define PS_DATA_L 0x8D
#define PS_DATA_H 0x8E
#define LTR559_INT_REG 0x8F
#define ALS_Upp_Threshold_Reg_0 0x97
#define ALS_Upp_Threshold_Reg_1 0x98
#define ALS_Low_Threshold_Reg_0 0x99
#define ALS_Low_Threshold_Reg_1 0x9A
#define PS_Upp_Threshold_Reg_0 0x90
#define PS_Upp_Threshold_Reg_1 0x91
#define PS_Low_Threshold_Reg_0 0x92
#define PS_Low_Threshold_Reg_1 0x93
#define PART_ID_REG 0x86

bool LTR559_isALSReady = false;
bool LTR559_isPSReady = false;
bool LTR559_dataInvalid = false;
int ch_idx = 0;
int ratio = 100;
int gain = 4;
int integration_time = 50;
double lux;
int ch0_c[4] = {17743, 42785, 5926, 0};
int ch1_c[4] = { -11059, 19548, -1185, 0};


void LTR559_setControlRegs() {

  //Enable ALS

  //For Gain X1 Command = 0x01 1 to 64k lux
  //For Gain X2 Command = 0x05 0.5 to 32k lux
  //For Gain X4 Command = 0x09 0.25 to 16k lux
  //For Gain X8 Command = 0x0D 0.125 to 8k lux
  //For Gain X48 Command = 0x19 0.02 to 1.3k lux
  //For Gain X96 Command = 0x1D 0.01 to 600 lux
  LTR559_write8(ALS_CONTR_REG, 0x09);

  //Enable PS
  LTR559_write8(PS_CONTR_REG, 0x03);
  delay(100);

}

void LTR559_setPSLEDRegs() {
  //The PS LED Register define the LED pulse modulation frequency, duty cycle and peak current.
  //Default setting is 0x7F(60khz, 100%, 100mA)

  //Set LED Pulse Frequency 30kHz(duty cycle 100%, peak curr 100mA)
  //Pulse freq = 30kHz, (100%, 100mA) 0x1F
  //Pulse freq = 40kHz, (100%, 100mA) 0x3F
  //Pulse freq = 50kHz, (100%, 100mA) 0x5F
  //Pulse freq = 60kHz, (100%, 100mA) 0x7F
  //Pulse freq = 70kHz, (100%, 100mA) 0x9F
  //Pulse freq = 80kHz, (100%, 100mA) 0xBF
  //Pulse freq = 90kHz, (100%, 100mA) 0xDF
  //Pulse freq = 1000kHz, (100%, 100mA) 0xFF

  LTR559_write8(PS_LED_REG, 0x1F);
}

void LTR559_SetPSMeasurementRate() {
  //The PS_MEAS_Rate register controls the PS measurement rate
  //Default setting of the register is 0x02 (repeat rate 100ms)
  //Set PS Repeat Rate 50ms 0x00
  //Set PS Repeat Rate 500ms 0x04
  LTR559_write8(PS_MEAS_RATE_REG, 0x02);
}

void LTR559_setALSMeasurementRate() {
  //The ALS_MEAS_RATE register controls the ALS integration time and measurement rate
  //Default setting of the reg is 0x03 (integration time 100ms, repeat rate 500ms)
  //Set ALS integration Time 200ms, Repeat rate 200ms 0x12
  //Set ALS integration Time 400ms, Repeat rate 500ms 0x1B
  //Set ALS integration Time 50ms, Repeat rate 500ms 0x1B
  //Set ALS integration Time 50ms, Repeat rate 100ms 0x09

  LTR559_write8(ALS_MEAS_RATE_REG, 0x09);
}

uint16_t LTR559_READ_ALS_DATA_CH1() {
  uint8_t ch1_l, ch1_h;
  uint16_t ch1_out;
  ch1_l = LTR559_read8(ALS_DATA_CH1_L);
  ch1_h = LTR559_read8(ALS_DATA_CH1_H);
  ch1_out = (ch1_h << 8) | ch1_l;
  return ch1_out;
}

uint16_t LTR559_READ_ALS_DATA_CH0() {
  uint8_t ch0_l, ch0_h;
  uint16_t ch0_out;
  ch0_l = LTR559_read8(ALS_DATA_CH0_L);
  ch0_h = LTR559_read8(ALS_DATA_CH0_H);
  ch0_out = (ch0_h << 8) | ch0_l;
  return ch0_out;
}

void LTR559_SW_reset() {
  LTR559_write8(PS_CONTR_REG, 0x00);
  delay(50);
  LTR559_write8(PS_CONTR_REG, 0x02);
  delay(50);
}

double getLux() {

  LTR559_getSensorStatus();
  while (LTR559_isALSReady == false) {
    LTR559_getSensorStatus();
    delay(1000);
  }
  if (LTR559_isALSReady) {
    LTR559_isALSReady = false;
    int16_t als1 = LTR559_READ_ALS_DATA_CH1();
    int16_t als0 = LTR559_READ_ALS_DATA_CH0();
    ratio = als1 * 100 / (als1 + als0);
    if (als0 + als1 > 0) {} else {
      ratio = 101;
    }
    if (ratio < 45) {
      ch_idx = 0;
    } else if (ratio < 64) {
      ch_idx = 1;
    } else if (ratio < 85) {
      ch_idx = 2;
    } else {
      ch_idx = 3;
    }

    lux = (als0 * ch0_c[ch_idx]) - (als1 * ch1_c[ch_idx]);
    lux /= (integration_time / 100.0);
    lux /= gain;
    lux /= 10000.0;
    return lux;
  }
}

int getRawALS0() {
  LTR559_getSensorStatus();
  while (LTR559_isALSReady == false) {
    LTR559_getSensorStatus();
    delay(10);
  }
  LTR559_isALSReady = false;
  return LTR559_READ_ALS_DATA_CH0();
}
int getRawALS1() {
  LTR559_getSensorStatus();
  while (LTR559_isALSReady == false) {
    LTR559_getSensorStatus();
    delay(10);
  }
  LTR559_isALSReady = false;
  return LTR559_READ_ALS_DATA_CH1();
}

void LTR559_getSensorStatus() {
  //The ALS_PS_STATUS Register contains the information on interrupt, ALS and PS data availability status.
  //The reg is read only
  int8_t interruptData;
  int8_t LTR559_int_status = LTR559_read8(ALS_PS_STATUS_REG);

  interruptData = LTR559_int_status & 0x0A;
  //Interrupt
  switch (interruptData) {
    case 8:
      //ALS interrupt
      Serial.println("ALS interrupt");
      LTR559_isALSReady = true;
      break;
    case 2:
      //PS interrupt
      Serial.println("PS interrupt");
      LTR559_isPSReady = true;
      break;
    case 10:
      //Both interrupt
      Serial.println("Both interrupt");
      LTR559_isALSReady = true;
      LTR559_isPSReady = true;
      break;
  }
  interruptData = LTR559_int_status & 0x05;
  //NewData_status
  switch (interruptData) {
    case 4:
      //ALS interrupt
      Serial.println("ALS new data");
      LTR559_isALSReady = true;
      break;
    case 1:
      //PS interrupt
      Serial.println("PS new data");
      LTR559_isPSReady = true;
      break;
    case 5:
      //Both interrupt
      Serial.println("Both new data");
      LTR559_isALSReady = true;
      LTR559_isPSReady = true;
      break;
  }

  //ALS_Data_valid
  interruptData = LTR559_int_status & 0x80;
  switch (interruptData) {
    case 0x00:
      //data valid
      Serial.println("ALS New data valid!");
      LTR559_isALSReady = true;
      break;
    case 0x80:
      //data invalid
      Serial.println("Data invalid!");
      if (LTR559_isALSReady) {
        LTR559_isALSReady = false;
      }
      LTR559_READ_PS_DATA();
      LTR559_READ_ALS_DATA_CH1();
      LTR559_READ_ALS_DATA_CH0();
      break;
  }
}

int16_t LTR559_READ_PS_DATA() {
  int8_t PS_L, PS_H;
  int16_t out;
  PS_L = LTR559_read8(PS_DATA_L);
  PS_H = LTR559_read8(PS_DATA_H);
  out = (PS_H << 8) | PS_L;
  return out;
}

void LTR559_setInterrupts() {
  //The interrupt register controls the operation of the interrupt pins and function.
  //The default value for the reg is 0x08 (inactive)
  // Note that when this reg is to be set with values other than its default values,
  // it should beset before device is in active mode.

  //Set interrupt polarity for active Low, both ALS and PS trigger
  //Command 0x03
  //Interrupt is Active Low and both ALS and PS can trigger
  //For Active High Interrupt, both trigger, Command 0x07
  //For Active High Interrupt, Only ALS trigger Command = 0x06
  //For Active High Interrupt, Only PS trigger, Command = 0x05
  LTR559_write8(LTR559_INT_REG, 0x08);
}

void LTR559_ALS_setThreshold() {
  //The ALPS_THRES_UP and ALS_THRES_LOW regs determines the upper and lower limite of the interrupt threshold value.
  //Following example illustrates the setting og the ALS threshold window of decimal values of 200 (lower thresh)
  // and 100 (upper thresh)

  uint8_t data0;
  uint8_t data1;

  //Upper Threshold setting (decimal 1000)
  data0 = 1000 >> 8;
  data1 = 1000 & 0xFF;
  LTR559_write8(ALS_Upp_Threshold_Reg_0, data0);
  LTR559_write8(ALS_Upp_Threshold_Reg_1, data1);


  //Lower Threshold setting (decimal 200)
  data0 = 200 >> 8;
  data1 = 200 & 0xFF;
  LTR559_write8(ALS_Low_Threshold_Reg_0, data0);
  LTR559_write8(ALS_Low_Threshold_Reg_1, data1);
}

void LTR559_PS_setThreshold() {
  //The PS_THRES_UP and PS_THRES_LOW registers determines the upper and lower limit of the interrupt threshold value.
  //Following example illustrates the setting of the PS dynamic threshold with hysteresis interruption for
  //decimal value 1000 (for NEAR detection) and 500 (for FAR detection)

  uint8_t data0;
  uint8_t data1;

  //For NEAR detection (decimal 1000)
  data0 = 1000 >> 8;
  data1 = 1000 & 0xFF;
  LTR559_write8(PS_Upp_Threshold_Reg_0, data0);
  LTR559_write8(PS_Upp_Threshold_Reg_1, data1);
  data0 = 0 >> 8;
  data1 = 0 & 0xFF;
  LTR559_write8(PS_Low_Threshold_Reg_0, data0);
  LTR559_write8(PS_Low_Threshold_Reg_1, data1);

  //For Far detection (decimal 500);
  data0 = 2047 >> 8;
  data1 = 2047 & 0xFF;
  LTR559_write8(PS_Upp_Threshold_Reg_0, data0);
  LTR559_write8(PS_Upp_Threshold_Reg_1, data1);
  data0 = 500 >> 8;
  data1 = 500 & 0xFF;
  LTR559_write8(PS_Low_Threshold_Reg_0, data0);
  LTR559_write8(PS_Low_Threshold_Reg_1, data1);
}

void LTR559_whoami() {
  byte in = LTR559_read8(PART_ID_REG);
  if (in == 146) {
    Serial.println("LTR559 Connected.");
  } else {
    Serial.println("LTR559 not found");
  }
}

void LTR559_write8(byte reg, byte value) {
  Wire.beginTransmission((uint8_t)LTR559_ADDR);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}

uint8_t LTR559_read8(byte reg) {
  uint8_t value;
  Wire.beginTransmission((uint8_t)LTR559_ADDR);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)LTR559_ADDR, (byte)1);
  value = Wire.read();
  return value;
}

void LTR559_begin() {
  LTR559_SW_reset();
  LTR559_whoami();
  LTR559_setInterrupts();
  LTR559_setALSMeasurementRate();
  LTR559_ALS_setThreshold();
  LTR559_PS_setThreshold();
  LTR559_setControlRegs();
}
