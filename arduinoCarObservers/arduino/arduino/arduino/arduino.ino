
#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 1001
#define DEVICE_NAME "arduino car"
#define TOKEN "~5802_+z+5FjRR3W6nP"


#include <Adafruit_NeoPixel.h>

#include "Arduino.h"
#include "SoftwareSerial.h"



#include <RemoteMe.h>
#include <Observers.h>
#include <ESP8266WiFi.h>



#include <ESP8266WiFiMulti.h>




RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

ESP8266WiFiMulti WiFiMulti;


#define AIN1 D7
#define AIN2 D6
#define PWMA D5

#define BIN1 D3
#define BIN2 D2
#define PWMB D1

#define SPEED 800

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

void stop() {
	motorB(0);
	motorA(0);
}

void forward(boolean v0) {
	if (!v0) {
		stop();
	}
	else {
		motorA(SPEED);
		motorB(SPEED);
	}
	
}

void back(boolean v0) {
	if (!v0) {
		stop();
	}
	else {
		motorA(-SPEED);
		motorB(-SPEED);
	}
}
void left(boolean v0) {
	if (!v0) {
		stop();
	}
	else {
		motorA(SPEED);
		motorB(-SPEED);
	}
}
void right(boolean v0) {
	if (!v0) {
		stop();
	}
	else {
		motorA(-SPEED);
		motorB(SPEED);
	}
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



  remoteMe.setupTwoWayCommunication();
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
  remoteMe.setupDirectConnections();
  remoteMe.getObservers()->observeBoolean("forward", forward);
  remoteMe.getObservers()->observeBoolean("back", back);
  remoteMe.getObservers()->observeBoolean("left", left);
  remoteMe.getObservers()->observeBoolean("right", right);
 
 Serial.println("finish setup");
}



void loop()
{
  remoteMe.loop();
  

}

