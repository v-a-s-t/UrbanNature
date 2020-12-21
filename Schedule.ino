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
