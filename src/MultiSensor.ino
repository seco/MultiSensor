#include <FS.h>                                                      // SPIFFS support
#include <ESP8266WiFi.h>                                             // ESP8266 Core WiFi Library
#include <ESP8266WebServer.h>                                        // Local WebServer used to serve the configuration portal
#include <ESP8266HTTPUpdateServer.h>                                 // OTA Updater: http://<ip>
#include <PubSubClient.h>                                            // MQTT Client Publish and Subscribe
#include <DNSServer.h>                                               // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266mDNS.h>                                             // Include the mDNS library
#include <WiFiManager.h>                                             // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ArduinoJson.h>                                             // Read, write JSON format
#include <DHT.h>                                                     // DHT sensor library by Adafruit
#include <Functions.h>                                               // Our functions
#include <User.h>                                                    // Custom settings

/*----------------------------------------- Global variables -----------------------------------------*/

WiFiClient WiFiClient;                                               // The WiFi client object
PubSubClient MQTTclient(WiFiClient);                                 // MQTT client object
MDNSResponder mDNS;                                                  // Multi-Cast DNS object

char chipID[25]           = "                        ";
char deviceID[30]         = "                             ";
int rebootAt;                                                        // How many mins to wait before auto-reboot
String freeHeap;

/* MQTT Settings */
char mqtt_server[40]      = "                                      ";
int  mqtt_port;
char mqtt_user[11]        = "          ";
char mqtt_password[11]    = "          ";
char mqtt_tempTopic[40]  = "                                      ";
char mqtt_motionTopic[40]= "                                      ";
char mqtt_reed1Topic[40]  = "                                      ";
char mqtt_reed2Topic[40]  = "                                      ";
char mqtt_reed3Topic[40]  = "                                      ";
char mqtt_reed4Topic[40]  = "                                      ";
char mqtt_inTopic[40]     = "                                      ";
long lastReconnectAttempt = 0;

/* Web Server */
ESP8266WebServer        httpServer(80);                              // WebServer on port 80
ESP8266HTTPUpdateServer httpUpdater;                                 // OTA updates
String                  INDEX_HTML;                                  // To hold web page

/* SIFFS Config file */
bool    spiffsActive = false;
String  getContentType(String filename);                             // convert the file extension to the MIME type
bool    handleFileRead(String path);                                 // send the right file to the client (if it exists)

/* Temperature variables */
int         tempPolling;                                             // Delay in secs for sleep timer (from config file)
#ifdef TEMP_SENSOR
char*       temp_state    = (char*) "UNDEFINED";
long        lastMsg       = 0;
float       temp_c;
float       humidity;
//static char celsiusTemp[7];
//static char fahrenheitTemp[7];
//static char humidityTemp[7];
DHT dht(DHT_PIN, DHTTYPE);                                           // Initialize DHT sensor
#endif

/* Motion sensor variables */
#ifdef MOTION_SENSOR
String motion_state = "UNDEFINED";
String last_motion_state = "";
#endif

/* Reed sensor variables */
#ifdef REED_SENSOR1
String reed_state1 = "UNDEFINED";
String last_reed_state1 = "";
#endif

#ifdef REED_SENSOR2
String reed_state2 = "UNDEFINED";
String last_reed_state2 = "";
#endif

#ifdef REED_SENSOR3
String reed_state3 = "UNDEFINED";
String last_reed_state3 = "";
#endif

#ifdef REED_SENSOR4
String reed_state4 = "UNDEFINED";
String last_reed_state4 = "";
#endif

long now;                                                            // Hold current time

/* ------------------------------------------ Start of code ------------------------------------------*/

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    delay(100);
  #endif

  #ifdef SERIAL_DEBUG
  debugln("\n\n************ Setup() started **************");
  #endif

  // Get chip's Unique ID to be used in various places
  uint32_t chipid = ESP.getChipId();
  snprintf(chipID, 25, "ESP-%08X", chipid);
#ifdef SERIAL_DEBUG
  debug("..Chip ID: "); debugln(chipID);
#endif
  if (SPIFFS.begin()) {                                              // Start filesystem
#ifdef SERIAL_DEBUG
    debugln("..File system mounted successfully");
#endif
    spiffsActive = true;
  } else {
#ifdef SERIAL_DEBUG
    debugln("..File system failed to mount");
#endif
    return;
  }

  char* result;                                                      // Result from read/write of config file

  result = loadConfig();                                             // Load configuration file
  if (strcmp(result, "OK") != 0) {
#ifdef SERIAL_DEBUG
  	debug("..");debugln(result);
#endif
  	return;
  }

  WiFi.hostname(deviceID);

#ifdef SERIAL_DEBUG
  debugln("..WiFiManager starting...");
#endif
  WiFiManager wifiManager;                                           // Initialise WiFiManager and it will do all the work of managing the WiFi
//  wifiManager.resetSettings();                                     // Wipes out previously saved WiFi settings
  wifiManager.setTimeout(300);                                       // Set timeout (secs) for completing WiFi configuration

//  if(!wifiManager.autoConnect(chipID, WWW_PASSWD)) {                 // Fetch SSID and Password from EEPROM and try to connect
  if(!wifiManager.autoConnect(chipID)) {                             // Fetch SSID and Password from EEPROM and try to connect
#ifdef SERIAL_DEBUG
    debugln("....Timed out connecting to Access Point");             // If it doesn't connect start an access point and go into
#endif
    delay(5000);                                                     // a blocking loop waiting for configuration or timeout
    ESP.restart();                                                   // Restart and try again
  }

  if (!mDNS.begin(deviceID, WiFi.localIP())) {                       // Start the mDNS responder for <deviceID>.local
#ifdef SERIAL_DEBUG
    debugln("..Error setting up MDNS responder!");
#endif
    return;
  }

  delay(10);

  WiFiStatus();                                                      // Now connected to WiFi so print out info

  // Setup http firmware update page
  httpUpdater.setup(&httpServer, UPDATE_PATH, WWW_USER, WWW_PASSWD);

  httpServer.on("/", HTTP_GET, handleRoot);
  httpServer.on("/", HTTP_POST, handleRoot);
  httpServer.on("/saveChanges", HTTP_POST, saveChanges);
  httpServer.onNotFound(handleNotFound);

  httpServer.begin();                                                // Start Web Server
  delay(10);

  mDNS.addService("ESP8266", "tcp", 80);                              // Add service availability

  MQTTclient.setServer(mqtt_server, mqtt_port);                      // Initialse MQTT client
  MQTTclient.setCallback(callback);                                  // Callback service for receiving MQTT messages
  lastReconnectAttempt = 0;

  delay(10);

  // Initialise PIN's
  pinMode(DHT_PIN, INPUT);
  pinMode(MOTION_PIN, INPUT);
  pinMode(REED_PIN1, INPUT);
  pinMode(REED_PIN2, INPUT);
  pinMode(REED_PIN3, INPUT);
  pinMode(REED_PIN4, INPUT);
  //pinMode(BUILTIN_LED, OUTPUT);

#ifdef TEMP_SENSOR
  dht.begin();                                                       // Start the DHT sensor
#endif

#ifdef SERIAL_DEBUG
  debugln("************ Setup() finished *************\n\n");
#endif

} // setup()

void WiFiStatus() {
#ifdef SERIAL_DEBUG
  debugln("Running WiFiStatus()");
  debug("..Connected to WiFi: "); debugln(WiFi.SSID());
  debug("....mDNS started: "); debug(deviceID); debugln(".local");
  debug("....IP address:   "); debugln(WiFi.localIP().toString());
  debug("....MAC address:  "); debugln(WiFi.macAddress());
  debug("....Signal (dBm): "); debugln((String)WiFi.RSSI());
#endif
  delay(10);
} // WiFiStatus()

void reboot() {
  delay(1000);
//  ESP.reset();
  ESP.restart();
} //reboot()

void getFreeHeap() {
  long  fh = ESP.getFreeHeap();
  char  fhc[20];

  ltoa(fh, fhc, 10);
  freeHeap = String(fhc);
#ifdef SERIAL_DEBUG
  debug("Free Heap: "); debugln(freeHeap);
#endif
} // getFreeHeap()

void loop() {

  // Non-blocking reconnect to MQTT if WiFi is connected
  // This allows other parts of the loop to run whilst no MQTT connection
  // If theres no WiFi that will be handled by WiFiManager in blocking mode
  if ((WiFi.status() == WL_CONNECTED) && !MQTTclient.connected()) {
    now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect MQTT
      if (MQTTconnect()) {
        // Client connected
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // MQTT Client connected so check for MQTT activity
    MQTTclient.loop();
    delay(100);
  }

  httpServer.handleClient();                                         // Give time to Web Server

  if (MQTTclient.connected()) {                                      // Check sensors as long as we have a full connection (WiFi + MQTT)

#ifdef TEMP_SENSOR
    getDHTdata();
#endif

#ifdef MOTION_SENSOR
    checkMotionState(MOTION_PIN, mqtt_motionTopic, last_motion_state, motion_state);
#endif

#ifdef REED_SENSOR1
    checkReedState(REED_PIN1, mqtt_reed1Topic, last_reed_state1, reed_state1);
#endif

#ifdef REED_SENSOR2
    checkReedState(REED_PIN2, mqtt_reed2Topic, last_reed_state2, reed_state2);
#endif

#ifdef REED_SENSOR3
    checkReedState(REED_PIN3, mqtt_reed3Topic, last_reed_state3, reed_state3);
#endif

#ifdef REED_SENSOR4
    checkReedState(REED_PIN4, mqtt_reed4Topic, last_reed_state4, reed_state4);
#endif

  }

  long millisecs = millis();

  String systemUpTimeMn;
  String systemUpTimeHr;
//  String systemUpTimeDy;

  systemUpTimeMn = int((millisecs / (1000 * 60)) % 60);
  systemUpTimeHr = int((millisecs / (1000 * 60 * 60)) % 24 );
//  systemUpTimeDy = int((millisecs / (1000 * 60 * 60 * 24)) % 365);

#ifdef SERIAL_DEBUG
//  debug("rebootAt: "); debug((String)rebootAt); debug("............. Min: "); debug(systemUpTimeMn); debug(" Hr: "); debugln(systemUpTimeHr);
#endif
  if (systemUpTimeHr.toInt() == rebootAt) {
    MQTTclient.publish("SH/Status/Rebooted", deviceID);
    delay(2000);
    reboot();
  }

} // loop()

void debug(String message) {
//#ifdef SERIAL_DEBUG
    Serial.print(message);
    delay(SERIAL_DELAY);
//#endif
}

void debugln(String message) {
//#ifdef SERIAL_DEBUG
    Serial.println(message);
    delay(SERIAL_DELAY);
//#endif
}

void debugln() {
//  #ifdef SERIAL_DEBUG
    Serial.println();
//  #endif
}
