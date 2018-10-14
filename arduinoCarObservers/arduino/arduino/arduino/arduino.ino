#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 35
#define DEVICE_NAME "ard"
#define TOKEN "~1_1&2PfMc'wIS"


#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>




ESP8266WiFiMulti WiFiMulti;

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

//*************** CODE FOR CONFORTABLE VARIABLE SET *********************

inline void setRl1(boolean b) { remoteMe.getVariables()->setBoolean("Rl1", b); }

//*************** IMPLEMENT FUNCTIONS BELOW *********************


void onRl1Change(boolean b) {
	Serial.printf("onRl1Change: b: %d\n", b);
}




void setup() {
	Serial.begin(9600);
	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

	remoteMe.getVariables()->observeBoolean("Rl1", onRl1Change);


	remoteMe.setConnector(new RemoteMeSocketConnector());
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}


void loop() {
	remoteMe.loop();
}

