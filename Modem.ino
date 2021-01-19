#include <TinyGsmClient.h>
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define TINY_GSM_YIELD() { delay(2); }



// GPRS credentials (leave empty, if missing)
const char apn[]      = ""; // APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

void setupModem() {

  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

#ifdef MODEM_RST
  // Keep reset high
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
#endif

  // Turn on the Modem power first
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Pull down PWRKEY for more than 1 second according to manual requirements
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);

  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);
}

bool modemConnect() {
  Serial.println("Initializing modem...");
  //modem.init();
  modem.restart();

  // Turn off network status lights to reduce current consumption
  turnOffNetlight();

  // The status light cannot be turned off, only physically removed
  //turnOffStatuslight();

  // Or, use modem.init() if you don't need the complete restart
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);

  // Unlock SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  Serial.print("Sim status: ");
  Serial.println(modem.getSimStatus());

  if (modem.getSimStatus() == SIM_ERROR) {
    Serial.println("WARN - Sim error.");
    return false;
  }

  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork(240000L)) {
    Serial.println(" fail");
    delay(10000);
    return false;
  }
  Serial.println(" OK");

  // When the network connection is successful, turn on the indicator
  digitalWrite(LED, HIGH);

  if (modem.isNetworkConnected()) {
    Serial.println("Network connected");
  }

  Serial.print(F("Connecting to APN... "));
  Serial.println(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("ERROR - APN connection fail");
    return false;
  }
  Serial.println(" OK");
  return true;
}

void modemDisconnect() {
  modem.gprsDisconnect();
  Serial.println(F("GPRS disconnected"));
}

void modemPoweroff() {
  modem.poweroff();
  Serial.println(F("Modem power off"));
}

void turnOffNetlight() {
  Serial.println("Turning off SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=0");
}

void turnOnNetlight() {
  Serial.println("Turning on SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=1");
}

void setLatLong(float latitude, float longitude) {
  latGSM = latitude;
  lonGSM = longitude;
}

float getGSMLat() {
  return latGSM;
}

float getGSMLon() {
  return lonGSM;
}

void getGSMLocation() {
  float _lat      = 0;
  float _lon      = 0;
  float _accuracy = 0;
  int   _year     = 0;
  int   _month    = 0;
  int   _day      = 0;
  int   _hour     = 0;
  int   _min      = 0;
  int _sec = 0;

  if (modem.getGsmLocation(&_lon, &_lat, &_accuracy, &_year, &_month, &_day, &_hour, &_min, &_sec)) {
    setLatLong(_lat, _lon);
    Serial.println("Got GSM Location");
    Serial.print("Lat: ");
    Serial.println(getGSMLat());
    Serial.print("Lon: ");
    Serial.println(getGSMLon());
    Serial.println();
    /* TO DO
        Serial.println("Got time");
        Serial.print(_year);
        Serial.print(":");
        Serial.print(_month);
        Serial.print(":");
        Serial.print(_day);
        Serial.println();
        Serial.print(_hour);
        Serial.print(":");
        Serial.print(_min);
        Serial.print(":");
        Serial.print(_sec);
        Serial.println();
    */

  }
}
