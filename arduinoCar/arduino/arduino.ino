#define WIFI_NAME ""
#define WIFI_PASSWORD ""
#define DEVICE_ID 1001
#define DEVICE_NAME "arduino car"
#define TOKEN ""

#include "SingleRing.h"
#include <Adafruit_NeoPixel.h>

#include "Arduino.h"
#include "SoftwareSerial.h"



#include <RemoteMe.h>
#include <ESP8266WiFi.h>



#include <ESP8266WiFiMulti.h>



SingleRing top = SingleRing(16, D8, false);
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

ESP8266WiFiMulti WiFiMulti;


#define AIN1 D7
#define AIN2 D6
#define PWMA D5

#define BIN1 D3
#define BIN2 D2
#define PWMB D1


void motorA(int speed) {
   uint8_t mode=0;
   if (speed<0){
    mode=1;
    speed=-speed;
  }else if (speed>0){
    mode=2;
  }else{
   speed=0;
    mode=0;
  }
  
  if (mode==0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }else if (mode==1){
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }else if (mode == 2) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }

  analogWrite(PWMA, speed);
}

void motorB( int speed) {
  uint8_t mode=0;
  
   if (speed<0){
    mode=1;
    speed=-speed;
  }else if (speed>0){
    mode=2;
  }else{
   speed=0;
    mode=0;
  }
  
  if (mode == 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
  else if (mode == 1) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  else if (mode == 2) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    
  }

  analogWrite(PWMB, speed);
}

void setupPins() {

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  analogWriteFreq(1000);
  analogWriteRange(1023);
}



void setup()
{
  Serial.begin(9600);

  Serial.println("starting");

  setupPins();

  

  

  WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }


  remoteMe.setUserMessageListener(onUserMessage);

  remoteMe.setupTwoWayCommunication();
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
  remoteMe.setupDirectConnections();

  top.setup();
 
 Serial.println("finish setup");
}



void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {
  uint16_t pos=0;
  uint8_t type=RemoteMeMessagesUtils::getUint8(data, pos);
  if (type==1){//motor
 
    
    motorA(RemoteMeMessagesUtils::getInt16(data, pos));
    motorB(RemoteMeMessagesUtils::getInt16(data, pos));
  }else if (type==2){//light
     top.setMode(RemoteMeMessagesUtils::getInt16(data, pos));
  }
}

void loop()
{
  remoteMe.loop();
  top.loop();
  

}

