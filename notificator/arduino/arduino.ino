#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>
#include "SingleRing.h"


#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>

#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 205
#define DEVICE_NAME "siren"
#define TOKEN "~267_ZxoWtJ)0ph&2c"

#define DIODES_COUNT 16


SingleRing top = SingleRing(DIODES_COUNT, D5);
SingleRing bottom = SingleRing(DIODES_COUNT, D6);

SoftwareSerial mySoftwareSerial(D4, D3); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

ESP8266WiFiMulti WiFiMulti;
void setup()
{
	mySoftwareSerial.begin(9600);
	Serial.begin(115200);


	if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
		Serial.println(F("Unable to begin:"));
		Serial.println(F("1.Please recheck the connection!"));
		Serial.println(F("2.Please insert the SD card!"));
		while (true);
	}
	Serial.println(F("DFPlayer Mini online."));

	myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

								
	myDFPlayer.volume(30); 

	myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);



	myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
	

	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}
	


	remoteMe.setUserMessageListener(onUserMessage);


	remoteMe.setupTwoWayCommunication();
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

	top.setup();
	bottom.setup();
	top.clear();
	bottom.clear();
}



boolean turnedOff = true;
unsigned long turnOffMillis = 0;

void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {
	uint16_t pos = 0;
	uint8_t bottomMode = RemoteMeMessagesUtils::getUint8(data, pos);
	uint8_t topMode = RemoteMeMessagesUtils::getUint8(data, pos);
	uint8_t trackNumber = RemoteMeMessagesUtils::getUint8(data, pos);
	uint8_t time = RemoteMeMessagesUtils::getUint8(data, pos);
	uint8_t mode = RemoteMeMessagesUtils::getUint8(data, pos);

	bottom.setMode(bottomMode);
	top.setMode(topMode);
	if (mode == 1) {
		myDFPlayer.loop(trackNumber);
	}
	else {
		myDFPlayer.play(trackNumber);
	}
	
	turnedOff = false;
	turnOffMillis = millis() + 1000 * time;
	
}

void loop()
{
	remoteMe.loop();
	top.loop();
	bottom.loop();
	if (turnOffMillis<millis()) {
		if (!turnedOff) {
			top.clear();
			bottom.clear();
			myDFPlayer.stop();
			turnedOff = true;
			
		}
	}

}

