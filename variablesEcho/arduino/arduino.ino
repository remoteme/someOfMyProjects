
#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 1001
#define DEVICE_NAME "arduino car"
#define TOKEN "~1_VLTq=uGG@M1Hp"


#include <Adafruit_NeoPixel.h>

#include "Arduino.h"
#include "SoftwareSerial.h"



#include <RemoteMe.h>
#include <Observers.h>
#include <ESP8266WiFi.h>



#include <ESP8266WiFiMulti.h>




RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

ESP8266WiFiMulti WiFiMulti;



void integerBooleanF(int32_t v,boolean b1 ) {
	Serial.println("integerBooleanF");
	Serial.println(v);
	Serial.println(b1);
	remoteMe.getObservers()->setIntegerBoolean("pam", v,b1,true);
}

void textF(String text) {
	Serial.println("textF");
	Serial.println(text);
  remoteMe.getObservers()->setText("pam2", text,true);
}

void doubleF(double d) {
	Serial.println("doubleF");
	Serial.println(d);
   remoteMe.getObservers()->setDouble("pam2", d,true);
}

void smallInteger2F(int16_t v1, int16_t v2) {
	Serial.println("smallInteger2F");
	Serial.println(v1);
	Serial.println(v2);
    remoteMe.getObservers()->setSmallInteger2("pam", v1,v2,true);
}

void smallInteger3F(int16_t v1, int16_t v2, int16_t v3) {
	Serial.println("smallInteger3F");
	Serial.println(v1);
	Serial.println(v2);
	Serial.println(v3);
   remoteMe.getObservers()->setSmallInteger3("pam", v1,v2,v3,true);
}

void booleanF(boolean v0) {
	Serial.println("booleanF");
	Serial.println(v0);
 remoteMe.getObservers()->setBoolean("pam", v0,true);
}


void setup()
{
  Serial.begin(9600);

  Serial.println("starting");

 

  

  WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }


  remoteMe.setupTwoWayCommunication();
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
  remoteMe.setupDirectConnections();
  remoteMe.getObservers()->observeIntegerBoolean("pam", integerBooleanF);
  remoteMe.getObservers()->observeText("pam2", textF);
  remoteMe.getObservers()->observeDouble("pam2", doubleF);
  remoteMe.getObservers()->observeSmallInteger2("pam", smallInteger2F);
  remoteMe.getObservers()->observeSmallInteger3("pam", smallInteger3F);
  remoteMe.getObservers()->observeBoolean("pam", booleanF);

 
 Serial.println("finish setup");
}




void loop()
{
  remoteMe.loop();
  

}

