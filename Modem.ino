#include <TinyGsmClient.h>
#include "Config.h"

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// GPRS credentials (leave empty, if missing)
const char apn[]      = ""; // APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

void setupModem() {
#ifdef MODEM_RST
  // Keep reset high
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_RST, HIGH);
#endif

  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  // Turn on the Modem power first
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Pull down PWRKEY for more than 1 second according to manual requirements
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
}

void connect() {
  Serial.println("Initializing modem...");
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

  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork(240000L)) {
      Serial.println(" fail");
      delay(10000);
      return;
  }
  Serial.println(" OK");

  // When the network connection is successful, turn on the indicator
  digitalWrite(LED, HIGH);

  if (modem.isNetworkConnected()) {
      Serial.println("Network connected");
  }

  Serial.print(F("Connecting to APN... "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
      Serial.println(" fail");
      delay(10000);
      return;
  }
  Serial.println(" OK");
}

void turnOffNetlight()
{
  Serial.println("Turning off SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=0");
}

void turnOnNetlight()
{
  Serial.println("Turning on SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=1");
}
