



#include <stdint.h>


#include "Wire.h"
#include "SPI.h"







// the setup function runs once when you press reset or power the board
void setup() {
 Serial.begin(9600);
 
  delay(10000);
  Serial.println("going deep sleep");
    ESP.deepSleep(30e6);
Serial.println("goiXXXXXX");
  
  
}

void loop(){
  





  
}



