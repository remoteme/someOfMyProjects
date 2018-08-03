#define WIFI_NAME ""
#define WIFI_PASSWORD ""
#define DEVICE_ID 207
#define DEVICE_NAME "wemos"
#define TOKEN ""

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <Observers.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);


uint8_t LEDpin = D5;

void onChange(boolean b) {
	digitalWrite(LEDpin, b ? HIGH : LOW);
}


void setup() {
	pinMode(LEDpin, OUTPUT);
	
	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

	remoteMe.setupTwoWayCommunication();

	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

	remoteMe.getObservers()->observeBoolean("led",onChange);
	remoteMe.getObservers()->setBoolean("led", false);
}


void loop() {
	remoteMe.loop();
}
