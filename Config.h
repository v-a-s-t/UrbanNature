//#define DEBUG_OUTPUT
//#define DEBUG_INTERVAL 10
#define DEBUG_PMS
#define DEBUG_LTR559
#define DEBUG_MICS6814
//#define DEBUG_MICS6814_COMPENSATED
#define DEBUG_MICROPHONE
#define DEBUG_BME280
#define DEBUG_SIGNAL_STRENGTH
//#define DEBUG_BATTERY_PCT
//#define DEBUG_BATTERY_VOLTAGE
//#define DEBUG_POST

#define LED 13
#define BUTTON 15
#define PMS_EN 33
#define PMS_RST 15
#define PMS_RX 18
#define PMS_TX 19
#define SENSOR_EN 32
#define MIC_SENSE 34
#define OX_SENSE 25
#define RED_SENSE 14
#define NH3_SENSE 12
#define BATTERY_READ 35

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

// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800          // Modem is SIM800
#define TINY_GSM_RX_BUFFER      1024   // Set RX buffer to 1Kb

const char server[] = "io.adafruit.com";
const int port = 80;

#define WIFI_CONNECTION_TIMEOUT 30000
#define NUM_OF_SENSORS 18
#define SSID_MAX_LENGTH 31
#define POST_MAX_RETRIES 5
#define POST_RETRY_DELAY 1000

// time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
#define TIME_TOLERANCE_MINUTES 5
#define LIGHT_LOAD_ON_S 2

// location
float latGSM = 0;
float lonGSM = 0;
float latUser = 0;
float lonUser = 0;
bool hasInputtedLatLon = false;
