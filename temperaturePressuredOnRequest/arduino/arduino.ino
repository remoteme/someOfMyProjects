#include <stdint.h>
#include "SparkFunBME280.h"

#include "Wire.h"
#include "SPI.h"

#define WIFI_NAME ""
#define WIFI_PASSWORD ""
#define DEVICE_ID 204
#define DEVICE_NAME "temperatureOnRequest"
#define TOKEN ""

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>



ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

BME280 mySensor;

// the setup function runs once when you press reset or power the board
void setup() {
	
	mySensor.settings.commInterface = I2C_MODE;
	mySensor.settings.I2CAddress = 0x76;


	//***Operation settings*****************************//

	//runMode can be:
	//  0, Sleep mode
	//  1 or 2, Forced mode
	//  3, Normal mode
	mySensor.settings.runMode = 3; //Forced mode

								   //tStandby can be:
								   //  0, 0.5ms
								   //  1, 62.5ms
								   //  2, 125ms
								   //  3, 250ms
								   //  4, 500ms
								   //  5, 1000ms
								   //  6, 10ms
								   //  7, 20ms
	mySensor.settings.tStandby = 0;

	//filter can be off or number of FIR coefficients to use:
	//  0, filter off
	//  1, coefficients = 2
	//  2, coefficients = 4
	//  3, coefficients = 8
	//  4, coefficients = 16
	mySensor.settings.filter = 0;

	//tempOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.tempOverSample = 1;

	//pressOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.pressOverSample = 1;

	//humidOverSample can be:
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.humidOverSample = 1;
	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.         Serial.begin(57600);

	mySensor.begin();

	//--------
	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}

	remoteMe.setUserSyncMessageListener(onUserSyncMessage);
	remoteMe.setupTwoWayCommunication();

	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

	
}


void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData)
{
	uint16_t pos = 0;

	returnDataSize = 8*4*3;
	returnData = (uint8_t*)malloc(returnDataSize);
	

	RemoteMeMessagesUtils::putFloat(returnData,pos, (mySensor.readTempC()));
	RemoteMeMessagesUtils::putFloat(returnData, pos, (mySensor.readFloatPressure() ));
	RemoteMeMessagesUtils::putFloat(returnData, pos, (mySensor.readFloatHumidity() ));
	

}


void loop()
{
	remoteMe.loop();
}