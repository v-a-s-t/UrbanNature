/*
Routine:
1. Connect to internet
  if collecting sensor data,
    if sim is present,
      choose modem.
    else,
      if wifi credentials,
        proceed.
      else,
        captive portal
2. Get time
3. Take measurement from each sensor
4. Send sensor measurement to each feed
5. Turn off peripherals
6. Schedule sleep
7. Sleep!
*/

struct tm timeData;
float timezone = 0.0;

// Must be called after wifi connect!
void setupWifiTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void connectAndCheckTime() {
  usingWifi = wifiConnect();
  if (!usingWifi) {
    setupModem();
    if (!modemConnect()) {
      Serial.println("ERROR: Modem could not connect.");
    }
  } else {
    Serial.println("Using wifi.");
  }
  getTime();
  int waitTime = calculateWaitTime(getHour(), getMinute(), startHour, interval);
  if (waitTime >= interval - 1) {
    waitTime = 0; // If we're late by a minute, just collect data! 
    Serial.println("Late by 1m. Collecting anyway.");
  }
  Serial.print("Minutes until data collection: ");
  Serial.println(waitTime);
  if (waitTime > 0) {
    goToSleepMinutes(waitTime);
  }
}

int getSleepMinutes() {
  getTime();
  int waitTime = calculateWaitTime(getHour(), getMinute(), startHour, interval);
  if (waitTime == 0) waitTime = interval;
  Serial.print("Minutes until next data collection: ");
  Serial.println(waitTime);
  return waitTime;
}

void getTime() {
  if (usingWifi) {
    setupWifiTime();
    getWifiTime();
    printTime();
  } else {
    getModemTime();
  }
}

void getWifiTime() {
  getLocalTime(&timeData);
}

void getModemTime() {
  modem.getNetworkTime(&timeData.tm_year, &timeData.tm_mon, &timeData.tm_mday, &timeData.tm_hour, &timeData.tm_min, &timeData.tm_sec, &timezone);
}

void printTime() {
  Serial.println(&timeData, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeData, "%A");
  Serial.print("Month: ");
  Serial.println(&timeData, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeData, "%d");
  Serial.print("Year: ");
  Serial.println(&timeData, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeData, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeData, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeData, "%M");
  Serial.print("Second: ");
  Serial.println(&timeData, "%S");
}

int getMinute() {
  return timeData.tm_min;
}

int getHour() {
  return timeData.tm_hour;
}

// Calculate how long we should wait until we record data.
int calculateWaitTime(int h, int m, int _startHour, int _interval) {
  int waitHours = 0;
  int waitMinutes = 0;
  if (_interval >= 60)
    waitHours = (h - _startHour) % (_interval / 60);
  else
    waitHours = 0;

  if (m % _interval > 0)
    waitMinutes = _interval - (m % _interval);
  else
    waitMinutes = 0;
  
  waitMinutes += waitHours * 60;
  return waitMinutes;
}

void scheduleHandler() {
  // TODO Read sensors and post to aio here
  goToSleep(getSleepMinutes());
}
