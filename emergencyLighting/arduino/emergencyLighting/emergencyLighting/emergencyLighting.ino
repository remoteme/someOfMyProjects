#include "secret.h"
#include "SingleRing.h"

#include <Adafruit_NeoPixel.h>
#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);


#define DIODES_COUNT 16

SingleRing top = SingleRing(DIODES_COUNT, D5,false);
SingleRing bottom = SingleRing(DIODES_COUNT, D6,false);



void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {
	top.setMode(data[0]);
	bottom.setMode(data[1]);

}

void setup() {
	Serial.begin(9600);
	Serial.println("started");
	while (!Serial) {
	}

	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}


	remoteMe.setUserMessageListener(onUserMessage);
	remoteMe.setupTwoWayCommunication();

	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

	top.setup();
	bottom.setup();
	Serial.println("setup end");
}





void loop() {
	static int mode = 1;
	static int i = 0;

	remoteMe.loop();

	/*
	if (i % 300==0) {


	top.setMode(mode);
	bottom.setMode(22-mode);

	mode++;
	mode %= 23;
	Serial.println(mode);

	}

	*/


	i++;
	top.step(i);
	bottom.step(i);


	delay(10);

}
