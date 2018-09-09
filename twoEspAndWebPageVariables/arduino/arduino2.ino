#define WIFI_NAME "  "
#define WIFI_PASSWORD "  "
#define DEVICE_ID 3
#define DEVICE_NAME "Arduino LED"
#define TOKEN "~  "

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <Variables.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

uint8_t R = D5;
uint8_t G = D6;
uint8_t B = D7;
uint8_t singleLed = D1;

//*************** CODE FOR CONFORTABLE VARIABLE SET *********************

inline void setRgbLed(int16_t i1, int16_t i2, int16_t i3) {remoteMe.getVariables()->setSmallInteger3("rgbLed", i1, i2, i3); }
inline void setSingleLed(boolean b) {remoteMe.getVariables()->setBoolean("singleLed", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************


void onSingleLedChange(boolean b) {
  digitalWrite(singleLed, b ? HIGH : LOW);
}

void onRgbLedChange(int16_t i1, int16_t i2, int16_t i3) {
   analogWrite(R,1023-i1);
   analogWrite(G, 1023-i2);
   analogWrite(B,1023- i3);
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


 pinMode(R, OUTPUT);
 pinMode(G, OUTPUT);
 pinMode(B, OUTPUT);
 pinMode(singleLed, OUTPUT);
}


void loop() {
  remoteMe.loop();
}
