#define WIFI_NAME "ania24"
#define WIFI_PASSWORD ""
#define DEVICE_ID 2
#define DEVICE_NAME "arduino1"
#define TOKEN ""


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#include <RemoteMeDirectWebSocketConnector.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>




ESP8266WiFiMulti WiFiMulti;

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

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

int16_t maxSpeed=512; 

int16_t yAx;
int16_t xAx;

void move(){
  int16_t leftSide=yAx;
  int16_t rightSide=yAx;

  leftSide+=xAx;
  rightSide-=xAx;

  int16_t delta=(leftSide+rightSide)/2;

   
  leftSide+=delta;
  rightSide+=delta;

  Serial.printf(" %d %d \n",leftSide,rightSide);
  motorA(rightSide);
  motorB(leftSide );
}

void onGAMEPAD_DOWNChange(boolean b) {
    yAx=b?-maxSpeed:0;
    move();
}

void onGAMEPAD_LEFTChange(boolean b) {
    xAx=b?-maxSpeed:0;
      move();
}

void onGAMEPAD_RIGHTChange(boolean b) {
   xAx=b?maxSpeed:0;
      move();
}

void onGAMEPAD_UPChange(boolean b) {
     yAx=b?maxSpeed:0;
       move();
}

void onXy2Change(int16_t i1, int16_t i2) {
  yAx=i2;
  xAx=i1;
  move();
}



void setup() {
    Serial.begin(115200);
    setupPins();
    WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
    while (WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    remoteMe.getVariables()->observeBoolean("GAMEPAD_DOWN" ,onGAMEPAD_DOWNChange);
    remoteMe.getVariables()->observeBoolean("GAMEPAD_LEFT" ,onGAMEPAD_LEFTChange);
    remoteMe.getVariables()->observeBoolean("GAMEPAD_RIGHT" ,onGAMEPAD_RIGHTChange);
    remoteMe.getVariables()->observeBoolean("GAMEPAD_UP" ,onGAMEPAD_UPChange);
    remoteMe.getVariables()->observeSmallInteger2("xy2" ,onXy2Change);
 
    remoteMe.setConnector(new RemoteMeSocketConnector());
    remoteMe.setDirectConnector(new RemoteMeDirectWebSocketConnector());
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}


void loop() {
    remoteMe.loop();

   //  digitalWrite(BIN1, LOW);
   //  digitalWrite(BIN2, LOW);
   //  digitalWrite(AIN1, LOW);
  //   digitalWrite(AIN2, LOW);
  //   delay(500);
  //   digitalWrite(BIN1, HIGH);
  //   digitalWrite(BIN2, HIGH);
  //   digitalWrite(AIN1, HIGH);
  //   digitalWrite(AIN2, HIGH);
  //   delay(500);
}



