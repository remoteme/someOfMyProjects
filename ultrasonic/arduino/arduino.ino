#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "xxxxxx"
#define DEVICE_ID 11
#define DEVICE_NAME "esp"
#define TOKEN "~155_D49LDj@aBFhdffK."


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#include <ESP8266WiFi.h>

#define TRIGGER D3
#define ECHO    D2

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR CONFORTABLE VARIABLE SET *********************

inline void setDistance(int32_t i) {remoteMe.getVariables()->setInteger("distance", i); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************






void setup() {

    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    
    remoteMe.setConnector(new RemoteMeSocketConnector());
    remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
     pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
}


void loop() {
    remoteMe.loop();
    static long time=millis();
    if (time+700<millis()){//cannot send more frwquent since it will be block
      time=millis();
      setDistance(getDistance());
    }
 
}

int32_t getDistance(){
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return (duration/2) / 29.09;
  
  
}