#ifdef REED_SENSOR1
void checkReedState(unsigned int PIN, char* topic, String& last_reed_state, String& reed_state) {
#ifdef SERIAL_DEBUG
//  debugln("Running checkReedState()");
#endif

  last_reed_state = reed_state;                                      // Get previous state of door

  if (digitalRead(PIN) == HIGH) {
    reed_state = "CLOSED";
//    digitalWrite(BUILTIN_LED, HIGH);                                 // LED off
  } else {
      reed_state = "OPEN";
//      digitalWrite(BUILTIN_LED, LOW);                                // LED on
  }

  // If the state has changed then publish the change otherwise no need
  if (last_reed_state != reed_state) {
    MQTTclient.publish(topic, &reed_state[0]);
#ifdef SERIAL_DEBUG
    debug("..Reed state change: "); debug(reed_state);      debugln();
#endif
  }

  delay(100);
} // checkReedState()
#endif