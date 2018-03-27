#define WIFI_NAME ""
#define WIFI_PASSWORD ""
#define DEVICE_ID 203
#define DEVICE_NAME "diodeWithButtonArduino"
#define TOKEN ""


#define WEBPAGE_DEVICE_ID 1001

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


#include <RBD_Timer.h>  
#include <RBD_Button.h> 


uint8_t LEDpin = D5;
RBD::Button button(D2);


ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);
boolean currentState = false;


void setup() {
	Serial.begin(9600);

	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}


	remoteMe.setUserMessageListener(onUserMessage);
	remoteMe.setUserSyncMessageListener(onUserSyncMessage);


	remoteMe.setupTwoWayCommunication();
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
	pinMode(LEDpin, OUTPUT);
	digitalWrite(LEDpin, LOW);

}


void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData) {
	returnDataSize = 1;
	returnData = (uint8_t*)malloc(returnDataSize);
	Serial.println("received sync message");
	uint16_t pos = 0;
	RemoteMeMessagesUtils::putUint8(returnData, pos, currentState ? 1 : 0);
	

}

void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {
	changeDiodeState();
	
}



void changeDiodeState(   ) {
	
	currentState= !currentState;

	digitalWrite(LEDpin, currentState?HIGH:LOW);

	uint16_t returnDataSize = 1;
	uint16_t pos = 0;
	uint8_t *data = (uint8_t*)malloc(returnDataSize);
	RemoteMeMessagesUtils::putUint8(data, pos, currentState?1:0);


	remoteMe.sendUserMessage(WEBPAGE_DEVICE_ID, data, returnDataSize);


}




void loop() {
	remoteMe.loop();


	if (button.onPressed()) {
		changeDiodeState();
	}


}