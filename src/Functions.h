char*   loadConfig();
char*   saveConfig();
void    handleNotFound();
void    handleRoot();
void    sendHTMLPage();
void    saveChanges();
boolean MQTTconnect();
void    callback(char* topic, byte* payload, unsigned int length);
void    getDHTdata();
void    checkReedState(unsigned int PIN, char* topic, String& last_reed_state, String& reed_state);
void    checkMotionState(unsigned int PIN, char* topic, String& last_motion_state, String& motion_state);