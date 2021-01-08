#define LED 13
#define BUTTON 2
#define PMS_EN 33
#define PMS_RST 2
#define PMS_RX 19
#define PMS_TX 18
#define SENSOR_EN 25
#define MIC_SENSE 34
#define OX_SENSE 35
#define RED_SENSE 14
#define NH3_SENSE 12

#define MODEM_RST             5
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define I2C_SDA              21
#define I2C_SCL              22

#define LTR559_ADDR         0x23
#define BME280_ADDR          0x76
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800          // Modem is SIM800
#define TINY_GSM_RX_BUFFER      1024   // Set RX buffer to 1Kb

const char server[] = "io.adafruit.com";
const int port = 80;

#define WIFI_CONNECTION_TIMEOUT 30000
#define NUM_OF_SENSORS 18

// time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// GSM location 
float latGSM = 0;
float lonGSM = 0;
