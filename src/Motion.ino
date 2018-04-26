#ifdef MOTION_SENSOR
void checkMotionState(unsigned int PIN, char* topic, String& last_motion_state, String& motion_state) {
#ifdef SERIAL_DEBUG
//  debugln("Running checkMotionState()");
#endif

  last_motion_state = motion_state; //get previous state of motion

  if (digitalRead(PIN) == HIGH) {
    motion_state = (char*) "OPEN";
//    digitalWrite(BUILTIN_LED, LOW);                                  // LED on
  } else {
      motion_state = (char*) "CLOSED";
//      digitalWrite(BUILTIN_LED, HIGH);                               // LED off
  }

  // If the state has changed only then publish the change
  if (last_motion_state != motion_state) {
    MQTTclient.publish(topic, &motion_state[0]);
#ifdef SERIAL_DEBUG
    debug("..Motion state change: "); debug(motion_state); debugln();
#endif
  }

/*
  int val;
  val = digitalRead(PIN);
  //low = no motion, high = motion
  if (val == LOW) {
    Serial.println("No motion");
  }
  else {
    Serial.println("Motion detected::ALARM");
  }
*/

/*
  if (digitalRead(MOTION_PIN) == HIGH) {
    MQTTclient.publish(mqtt_motionTopic, &motion_state[0]);
#ifdef SERIAL_DEBUG
    debug("..motion state change: "); debug(motion_state); debugln();
#endif
  }
*/

  delay(100);
} // checkMotionState()
#endif
