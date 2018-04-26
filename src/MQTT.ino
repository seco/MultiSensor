boolean MQTTconnect() {
#ifdef SERIAL_DEBUG
  debugln("Running MQTTconnect()");
  debug("..Attempting MQTT connection to "); debug(mqtt_server); debug("::"); debug((String)mqtt_port); debug("("); debug(mqtt_user); debug("/"); debug(mqtt_password); debugln(")");
#endif
  if (MQTTclient.connect(deviceID, mqtt_user, mqtt_password)) {

#ifdef TEMP_SENSOR
    MQTTclient.publish(mqtt_tempTopic, temp_state);
#endif

#ifdef MOTION_SENSOR
    MQTTclient.publish(mqtt_motionTopic, &motion_state[0]);
#endif

#ifdef REED_SENSOR1
    MQTTclient.publish(mqtt_reed1Topic, &reed_state1[0]);
#endif

#ifdef REED_SENSOR2
    MQTTclient.publish(mqtt_reed2Topic, &reed_state2[0]);
#endif

#ifdef REED_SENSOR3
    MQTTclient.publish(mqtt_reed3Topic, &reed_state3[0]);
#endif

#ifdef REED_SENSOR4
    MQTTclient.publish(mqtt_reed4Topic, &reed_state4[0]);
#endif

    MQTTclient.subscribe("mqtt_inTopic");
#ifdef SERIAL_DEBUG
    debugln("..MQTT connected");
#endif
  } else {
#ifdef SERIAL_DEBUG
    debugln("..MQTT connection failed");
#endif
  }

  delay(10);
  return MQTTclient.connected();
} // MQTTconnect()

void callback(char* topic, byte* payload, unsigned int length) {
#ifdef SERIAL_DEBUG
  debugln("Running MQTTconnect()");
  debug("..Message arrived ["); debug(topic); debug("] ");
#endif

  if (strcmp(topic,"SH/status")==0) {
    MQTTclient.publish("SH/statusReply", deviceID);
/*
    for (int i = 0; i < length; i++) {
      debug((String)(char)payload[i]);
    }
*/
  }
  delay(10);
} // callback()