float red_temp_comp_factor = -0.015;
float red_hum_comp_factor = 0.0125;
float red_bar_comp_factor = -0.0053;
float oxi_temp_comp_factor = -0.017;
float oxi_hum_comp_factor = 0.0115;
float oxi_bar_comp_factor = -0.0072;
float nh3_temp_comp_factor = -0.02695;
float nh3_hum_comp_factor = 0.0094;
float nh3_bar_comp_factor = 0.003254;

float comp_oxi_RS, comp_red_RS, comp_nh3_RS;

double mics6814_readOx() {
  double oxIn = analogRead(OX_SENSE);
  oxIn = 56000 / ((4095 / oxIn) - 1);
  return oxIn;
}

double mics6814_readRed() {
  double redIn = analogRead(RED_SENSE);
  redIn = 56000 / ((4095 / redIn) - 1);
  return redIn;
}

double mics6814_readNH3() {
  double nh3In = analogRead(NH3_SENSE);
  nh3In = 56000 / ((4095 / nh3In) - 1);
  return nh3In;
}


#define R0_SAMPLE_AMOUNT 20
void setR0() {
  float red_r0, oxi_r0, nh3_r0, gas_temp, gas_press, gas_hum;
  for (int i = 0; i < R0_SAMPLE_AMOUNT; i++) {
    red_r0 = mics6814_readRed() + red_r0;
    oxi_r0 = mics6814_readOx() + oxi_r0;
    nh3_r0 = mics6814_readNH3() + nh3_r0;
    gas_temp = BME280_getTemp() + gas_temp;
    gas_press = BME280_getPressure() + gas_press;
    gas_hum = BME280_getHumidity() + gas_hum;
    delay(100);
  }
  red_r0 = red_r0 / R0_SAMPLE_AMOUNT;
  oxi_r0 = oxi_r0 / R0_SAMPLE_AMOUNT;
  nh3_r0 = nh3_r0 / R0_SAMPLE_AMOUNT;
  gas_temp = gas_temp / R0_SAMPLE_AMOUNT;
  gas_press = gas_press / R0_SAMPLE_AMOUNT;
  gas_hum = gas_hum / R0_SAMPLE_AMOUNT;


  float red_r0_av = prefsGetFloat("redR0");
  float oxi_r0_av = prefsGetFloat("oxiR0");
  float nh3_r0_av = prefsGetFloat("nh3R0");
  float gas_temp_av = prefsGetFloat("tempR0");
  float gas_press_av = prefsGetFloat("pressR0");
  float gas_hum_av = prefsGetFloat("humR0");
  unsigned int counter = prefsGetInt("R0Counter");
  counter++;

  red_r0_av = (red_r0_av + red_r0) / counter;
  oxi_r0_av = (oxi_r0_av + oxi_r0) / counter;
  nh3_r0_av = (nh3_r0_av + nh3_r0) / counter;
  gas_temp_av = ( gas_temp_av + gas_temp) / counter;
  gas_press_av = ( gas_press_av + gas_press) / counter;
  gas_hum_av = ( gas_hum_av + gas_hum) / counter;
  prefsSetInt("R0Counter", counter);
  prefsSetFloat("redR0", red_r0_av);
  prefsSetFloat("oxiR0", oxi_r0_av);
  prefsSetFloat("nh3R0", nh3_r0_av);
  prefsSetFloat("tempR0", gas_temp_av);
  prefsSetFloat("pressR0", gas_press_av);
  prefsSetFloat("humR0", gas_hum_av);

  Serial.println("R0 values set!");
  Serial.print("RED: ");
  Serial.println(red_r0_av);
  Serial.print("OXI: ");
  Serial.println(oxi_r0_av);
  Serial.print("NH3: ");
  Serial.println(nh3_r0_av);
  Serial.print("Temp: ");
  Serial.println(gas_temp_av);
  Serial.print("Pressure: ");
  Serial.println(gas_press_av);
  Serial.print("Humidtiy: ");
  Serial.println(gas_hum_av);
}

float getRatio(float RS, float R0) {
  if (RS / R0 > 0) {
    return RS / R0;
  } else {
    return 0.0001;
  }
}


float getOxR0() {
  float ox = prefsGetFloat("oxiR0");
  return ox;
}

float getRedR0() {
  float red = prefsGetFloat("redR0");
  return red;
}

float getNH3R0() {
  float nh3 = prefsGetFloat("nh3R0");
  return nh3;
}

void compensateGasReadings() {
  float ox = mics6814_readOx();
  float red = mics6814_readRed();
  float nh3 = mics6814_readNH3();
  float gas_temp_av = prefsGetFloat("tempR0");
  float gas_press_av = prefsGetFloat("pressR0");
  float gas_hum_av = prefsGetFloat("humR0");
  float gas_temp_diff, gas_press_diff, gas_hum_diff;
  gas_temp_diff = BME280_getTemp();
  gas_temp_diff = gas_temp_diff - gas_temp_av;
  gas_hum_diff = BME280_getHumidity();
  gas_hum_diff = gas_hum_diff - gas_hum_av;
  gas_press_diff = BME280_getPressure();
  gas_press_diff = gas_press_diff - gas_press_av;
  comp_oxi_RS = ox - (red_temp_comp_factor * ox * gas_temp_diff +
                      red_hum_comp_factor * ox * gas_hum_diff +
                      red_bar_comp_factor * ox * gas_press_diff);
  comp_red_RS = red - (red_temp_comp_factor * red * gas_temp_diff +
                       red_hum_comp_factor * red * gas_hum_diff +
                       red_bar_comp_factor * red * gas_press_diff);
  comp_nh3_RS = nh3 - (red_temp_comp_factor * nh3 * gas_temp_diff +
                       red_hum_comp_factor * nh3 * gas_hum_diff +
                       red_bar_comp_factor * nh3 * gas_press_diff);

}

/* Order for more reliable compensated data from the gas senor:
   - Turn on heater for 6 seconds.
   - Take initial start up readings of each gas sensor along with temperature, humidity and pressure.
   - add to rolling average
   - Leave heater on for 2 minutes
   - Take secondary start up readings of each gas sensor along with temperature, humidity and pressure.
   - calculate compensated Gas reasings
   - work out ratio of gas reading (RS/RO)
*/

void mics6814_setup() {
  delay(6000);
  setR0();
  delay(120000);
  compensateGasReadings();
  Serial.print("OX Ratio: ");
  Serial.println(getRatio(comp_oxi_RS, getOxR0()));
  Serial.print("RED Ratio: ");
  Serial.println(getRatio(comp_red_RS, getRedR0()));
  Serial.print("NH3 Ratio: ");
  Serial.println(getRatio(comp_nh3_RS, getNH3R0()));
}
