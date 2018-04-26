void handleNotFound() {
#ifdef SERIAL_DEBUG
  debugln("Running handleNotFound()");
#endif
  httpServer.send(404, "text/plain", "404: Not found");
} // handleNotFound()

void handleRoot() {
#ifdef SERIAL_DEBUG
  debugln("Running handleRoot()");
#endif
  if(!httpServer.authenticate(WWW_USER, WWW_PASSWD)) {               // Check user + password
    return httpServer.requestAuthentication();                       // Request user + password
  }
#ifdef SERIAL_DEBUG
  debugln("..HTTP status 200: Good URL and successfully authenticated");
#endif
  sendHTMLPage();
  delay(10);                                                         // Build the HTML (array) and send to browser
} // handleRoot()

void sendHTMLPage() {
#ifdef SERIAL_DEBUG
  debugln("Running sendHTMLPage()");
#endif

  INDEX_HTML  = "<!DOCTYPE html>\n";
  INDEX_HTML += "<html lang=\"en\">\n";
	INDEX_HTML += "<head>\n";
	INDEX_HTML += "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\", charset=\"utf-8\">\n";
	INDEX_HTML += "<title>Configure Settings</title>\n";
	INDEX_HTML += "<style>\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\"</style>\n";
	INDEX_HTML += "</head>\n";
	INDEX_HTML += "<body> \n";

	INDEX_HTML += "<p><h1>Configure Settings</h1></p>\n";

  INDEX_HTML += "<form action='/saveChanges' method='POST' target=\"_self\">\n";

  INDEX_HTML += "<label>Device ID&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"deviceID\" size=\"30\" value=\"";
  INDEX_HTML += deviceID;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (Unique ID) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>Reboot Interval&nbsp&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"rebootAt\" size=\"30\" value=\"";
  INDEX_HTML += rebootAt;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (Reboot device after N Hours) </label>\n";
  INDEX_HTML += "<br>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>Temperature Polling&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"tempPolling\" size=\"30\" value=\"";
  INDEX_HTML += tempPolling;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (Temperature polling interval in secs) </label>\n";
  INDEX_HTML += "<br>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Server&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_server\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_server;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (MQTT static IP) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Port&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_port\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_port;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (MQTT port typically 1883) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT User&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_user\" value=\"";
  INDEX_HTML += mqtt_user;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. openhab)</label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Password&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"password\" name=\"mqtt_password\" value=\"";
  INDEX_HTML += mqtt_password;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label></label>\n";
  INDEX_HTML += "<br>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Temperature Msg</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic1\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_tempTopic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/LivingHeatIndex) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Motion Msg&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic2\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_motionTopic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/GarageMotion) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Door/Window Msg</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic3\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_reed1Topic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/WindowContact1) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Door/Window Msg</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic4\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_reed2Topic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/WindowContact2) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Door/Window Msg</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic5\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_reed3Topic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/WindowContact3) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT Door/Window Msg</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_outTopic6\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_reed4Topic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/sensor/WindowContact4) </label>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<label>MQTT inTopic&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp</label>\n";
  INDEX_HTML += "<input type=\"text\" name=\"mqtt_inTopic\" size=\"30\" value=\"";
  INDEX_HTML += mqtt_inTopic;
  INDEX_HTML += "\">\n";
  INDEX_HTML += "<label> (eg. SH/status) </label>\n";
  INDEX_HTML += "<br>\n";
  INDEX_HTML += "<br>\n";

  INDEX_HTML += "<input type=\"submit\" value=\"Save Changes\">\n";
  INDEX_HTML += "<input type=button onClick=\"parent.location='/firmware'\" value='Firmware Update'>\n";
	INDEX_HTML += "</form>\n";
	INDEX_HTML += "</body>\n";
  INDEX_HTML += "</html>\n";

  httpServer.send(200, "text/html", INDEX_HTML);                     // Send HTTP status 200 (Ok) and HTML to the browser
  delay(100);

#ifdef SERIAL_DEBUG
  debugln("..Web Page updated");
#endif

} // sendHTMLPage()

void saveChanges() {
#ifdef SERIAL_DEBUG
  debugln("Running saveChanges()");
#endif

  if (httpServer.args() !=  14) {
#ifdef SERIAL_DEBUG
    debugln("Wrong number args received from HTTP POST");
#endif
  	return;
  }
  debug("..No. of arguments from web page save: "); debugln((String)httpServer.args());
   
//  for ( uint8_t i = 0; i < httpServer.args(); i++ ) {
//    debugln(httpServer.arg(i));
//  }

  strcpy(deviceID, httpServer.arg(0).c_str());
  rebootAt    = httpServer.arg(1).toInt();
  tempPolling = httpServer.arg(2).toInt();
  strcpy(mqtt_server, httpServer.arg(3).c_str());
  mqtt_port = httpServer.arg(4).toInt();
  strcpy(mqtt_user, httpServer.arg(5).c_str());
  strcpy(mqtt_password, httpServer.arg(6).c_str());
  strcpy(mqtt_tempTopic, httpServer.arg(7).c_str());
  strcpy(mqtt_motionTopic, httpServer.arg(8).c_str());
  strcpy(mqtt_reed1Topic, httpServer.arg(9).c_str());
  strcpy(mqtt_reed2Topic, httpServer.arg(10).c_str());
  strcpy(mqtt_reed3Topic, httpServer.arg(11).c_str());
  strcpy(mqtt_reed4Topic, httpServer.arg(12).c_str());
  strcpy(mqtt_inTopic, httpServer.arg(13).c_str());

  saveConfig();
  httpServer.send(200,"text/html", "<META http-equiv=\"refresh\" content=\"15;URL=/\">Changes saved! Rebooting...");
  reboot();
  
} // saveChanges()