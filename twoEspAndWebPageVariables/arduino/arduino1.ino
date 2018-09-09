#define WIFI_NAME "   "
#define WIFI_PASSWORD "   "
#define DEVICE_ID 2
#define DEVICE_NAME "ArduinoWithButtons"
#define TOKEN "   "


#include <RBD_Timer.h>
#include <RBD_Button.h>
#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <Variables.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>

boolean currentState=false;
uint8_t LEDpin = D5;
RBD::Button button1(D1);
RBD::Button button2(D2);

ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);



//*************** CODE FOR CONFORTABLE VARIABLE SET *********************

inline void setRgbLed(int16_t i1, int16_t i2, int16_t i3) {remoteMe.getVariables()->setSmallInteger3("rgbLed", i1, i2, i3); }
inline void setSingleLed(boolean b) {remoteMe.getVariables()->setBoolean("singleLed", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************

void onRgbLedChange(int16_t i1, int16_t i2, int16_t i3) {
  //your code here
}


void onSingleLedChange(boolean b) {
   currentState=b;
 digitalWrite(LEDpin, b ? HIGH : LOW);
}


void setup() {

  WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  remoteMe.setupTwoWayCommunication();

  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

 remoteMe.getVariables()->observeSmallInteger3("rgbLed" ,onRgbLedChange);
 remoteMe.getVariables()->observeBoolean("singleLed" ,onSingleLedChange);


 pinMode(LEDpin, OUTPUT);
}


void loop() {
  remoteMe.loop();

  if (button1.onPressed()) {
    setSingleLed(!currentState);
  }
  if (button2.onPressed()) {
   setRgbLed(random(1024),random(1024),random(1024));
  }
}