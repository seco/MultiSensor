char* loadConfig() {                                                 // Load JSON conig file from SPIFFS
#ifdef SERIAL_DEBUG
  debugln("Running loadConfig()");
#endif

  File configFile = SPIFFS.open(CONFIGFILE, "r");
  if (!configFile) {
    return (char*)"Failed to open config file";
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11) + 5500;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  std::unique_ptr<char[]> buf(new char[bufferSize]);                 // Allocate a buffer to store contents of the file

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), bufferSize);

  JsonObject& root = jsonBuffer.parseObject(buf.get());
  if (!root.success()) {
    return (char*)"Failed to parse config file correctly (root section)";
  }

  const char* t_deviceID       = root["deviceID"];     strcpy(deviceID, t_deviceID);
  int         t_rebootAt       = root["rebootAt"];     rebootAt    = t_rebootAt;
  int         t_Polling        = root["tempPolling"];  tempPolling = t_Polling;

  JsonObject& MQTT             = root["MQTT"];
  if (!MQTT.success()) {
    return (char*)"Failed to parse config file correctly (MQTT section)";
  }

  const char* t_MQTT_server      = MQTT["Server"];      strcpy(mqtt_server,      t_MQTT_server);
  int         t_MQTT_Port        = MQTT["Port"];        mqtt_port =              t_MQTT_Port;
  const char* t_MQTT_user        = MQTT["User"];        strcpy(mqtt_user,        t_MQTT_user);
  const char* t_MQTT_password    = MQTT["Password"];    strcpy(mqtt_password,    t_MQTT_password);
  const char* t_MQTT_tempTopic   = MQTT["tempTopic"];   strcpy(mqtt_tempTopic,   t_MQTT_tempTopic);
  const char* t_MQTT_motionTopic = MQTT["motionTopic"]; strcpy(mqtt_motionTopic, t_MQTT_motionTopic);
  const char* t_MQTT_reedTopic1  = MQTT["reed1Topic"];  strcpy(mqtt_reed1Topic,  t_MQTT_reedTopic1);
  const char* t_MQTT_reedTopic2  = MQTT["reed2Topic"];  strcpy(mqtt_reed2Topic,  t_MQTT_reedTopic2);
  const char* t_MQTT_reedTopic3  = MQTT["reed3Topic"];  strcpy(mqtt_reed3Topic,  t_MQTT_reedTopic3);
  const char* t_MQTT_reedTopic4  = MQTT["reed4Topic"];  strcpy(mqtt_reed4Topic,  t_MQTT_reedTopic4);
  const char* t_MQTT_inTopic     = MQTT["inTopic"];     strcpy(mqtt_inTopic,     t_MQTT_inTopic);

#ifdef SERIAL_DEBUG
  debugln("..Config file start");
  debug("....deviceID:         "); debugln(deviceID);
  debug("....Reboot At:        "); debugln((String)rebootAt);
  debug("....Temp Polling:     "); debugln((String)tempPolling);
  debug("....MQTT Server:      "); debugln(mqtt_server);
  debug("....MQTT Port:        "); debugln((String)mqtt_port);
  debug("....MQTT User:        "); debugln((String)mqtt_user);
  debug("....MQTT Password:    "); debugln((String)mqtt_password);
  debug("....MQTT tempTopic :  "); debugln(mqtt_tempTopic);
  debug("....MQTT motionTopic: "); debugln(mqtt_motionTopic);
  debug("....MQTT reed1Topic1: "); debugln(mqtt_reed1Topic);
  debug("....MQTT reed2Topic2: "); debugln(mqtt_reed2Topic);
  debug("....MQTT reed3Topic3: "); debugln(mqtt_reed3Topic);
  debug("....MQTT reed4Topic4: "); debugln(mqtt_reed4Topic);
  debug("....MQTT inTopic:     "); debugln(mqtt_inTopic);
  debugln("..Config file end");
#endif

  delay(10);
  return (char*)"OK";
} // loadConfig()

char* saveConfig() {                                                 // Save JSON config file to SPIFFS
#ifdef SERIAL_DEBUG
  debugln("Running saveConfig()");
#endif

  File configFile = SPIFFS.open(CONFIGFILE, "w");
  if (!configFile) {
    return (char*)"Failed to open config file for writing";
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root     = jsonBuffer.createObject();
  root["deviceID"]     = deviceID;
  root["rebootAt"]     = rebootAt;
  root["tempPolling"]  = tempPolling;

  JsonObject& MQTT     = root.createNestedObject("MQTT");
  MQTT["Server"]       = mqtt_server;
  MQTT["Port"]         = mqtt_port;
  MQTT["User"]         = mqtt_user;
  MQTT["Password"]     = mqtt_password;
  MQTT["tempTopic"]    = mqtt_tempTopic;
  MQTT["motionTopic"]  = mqtt_motionTopic;
  MQTT["reed1Topic"]   = mqtt_reed1Topic;
  MQTT["reed2Topic"]   = mqtt_reed2Topic;
  MQTT["reed3Topic"]   = mqtt_reed3Topic;
  MQTT["reed4Topic"]   = mqtt_reed4Topic;
  MQTT["inTopic"]      = mqtt_inTopic;

  root.printTo(configFile);
  delay(10);
  
#ifdef SERIAL_DEBUG
  debugln("..Changes saved");
#endif
  
  return (char*)"OK";
} // saveConfig()