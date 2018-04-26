/*       ******************************************
         Comment the sensor's that are not required
         ******************************************
*/
#define TEMP_SENSOR                                                  // Temperature sensor
#define MOTION_SENSOR                                                // Motion sensor
#define REED_SENSOR1                                                 // Door or window sensor
//#define REED_SENSOR2                                                 // Door or window sensor
//#define REED_SENSOR3                                                 // Door or window sensor
//#define REED_SENSOR4                                                 // Door or window sensor

#define DHTTYPE               DHT22                                  // DHT 22 (AM2302), AM2321

// For Wemos D1 mini pro
#define DHT_PIN               D4                                     // Pin for Temperature sensor
#define MOTION_PIN            D2                                     // Pin for motion sensor
#define REED_PIN1             D1                                     // Pin for door/window sensor 1
#define REED_PIN2             D5                                     // Pin for door/window sensor 2
#define REED_PIN3             D6                                     // Pin for door/window sensor 3
#define REED_PIN4             D7                                     // Pin for door/window sensor 4

//#define SERIAL_DEBUG                                                 // Comment this to disable debugging
#define SERIAL_DELAY          000                                    // Delay for Serial.println to be able read

#define UPDATE_PATH           "/firmware"                            // Our update URL (http://<IP>/firmware
#define WWW_USER              "admin"                                // User for web page
#define WWW_PASSWD            "esp8266ex"                            // Password for web page
#define CONFIGFILE            "/config.json"                         // Configurtion file holding various values
