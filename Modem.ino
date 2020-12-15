#include <TinyGsmClient.h>
#include "Config.h"

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

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

  // Unlock your SIM card with a PIN if needed
  Serial.print("Sim status: ");
  Serial.println(modem.getSimStatus());
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
