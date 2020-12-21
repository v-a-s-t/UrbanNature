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

// Must be called after wifi connect!
void setupWifiTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void getTime() {
  if (isSimPresent) {
    getWifiTime();
  }
}

void getWifiTime() {
  getLocalTime(&timeData);
}

void getModemTime() {
  // TODO modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3, &timezone);
  // TODO after getting time from modem, set esp32 time.
}

void setTime() {
  /* TODO
  struct timeval tv;
  tv.tv_sec =   1601216683;  // enter UTC UNIX time (get it from https://www.unixtimestamp.com )
  settimeofday(&tv, NULL);
  // Set timezone to France (Europe/Paris) obvs change this to GB
  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/ 3", 1); // https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
  tzset();
  */
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
