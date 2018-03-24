#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 202
#define DEVICE_NAME "switcher"
#define TOKEN "~1_VLTq=uGG@M1Hp"
#define WEBPAGE_DEVICE_ID 1000

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#include <ESP8266WiFiMulti.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


#include <Encoder.h>
#include <RBD_Timer.h>  
#include <RBD_Button.h> 

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


Encoder encoder(D5, D6);


uint8_t LEDpin = D3;
int analogPin = A0;





RBD::Button button(D7);


ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

Servo servo;

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


	servo.attach(15); //D8

	servo.write(20);

	//----- display
	Wire.begin();
	lcd.begin(16, 2); // Inicjalizacja LCD 2x16
	lcd.setCursor(0, 0);
	lcd.print("RemoteMe.org");
	lcd.setCursor(0, 1);
	lcd.print(":)   :)   :)");
}


void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData) {


}

//1 string to display
//2 servo
//3 diode
void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {

	char   buffer[25];
	sprintf(buffer, "on message  %d %d", data[0], data[1]);
	Serial.println(buffer);

	uint16_t pos = 0;
	uint8_t type = RemoteMeMessagesUtils::getByte(data, pos);

	if (type == 1) {
		String line1 = RemoteMeMessagesUtils::getString(data, pos);
		String line2 = RemoteMeMessagesUtils::getString(data, pos);
		lcd.setCursor(0, 0);
		lcd.print(line1);
		lcd.setCursor(0, 1);
		lcd.print(line2);
	}
	else if (type == 2) {
		servo.write(RemoteMeMessagesUtils::getByte(data, pos));
	}
	else if (type == 3) {
		analogWrite(LEDpin, ((int)4)*RemoteMeMessagesUtils::getByte(data, pos));
	}
}

//1 - Potentiometer + value
//2 - button click 
//3 - encoder + value - signed int
void sendChangeEvent(uint8_t type, uint8_t value) {
	uint16_t pos = 0;
	uint8_t *data = (uint8_t*)malloc(2);
	RemoteMeMessagesUtils::putByte(data, pos, type);
	RemoteMeMessagesUtils::putByte(data, pos, value);

	char   buffer[25];
	sprintf(buffer, "send message  %d %d", data[0], data[1]);
	Serial.println(buffer);
	remoteMe.sendUserMessage(WEBPAGE_DEVICE_ID, data, 2);
}


uint16_t lastVoltage = 0;
int32_t position = -999;

int i = 0;
void loop() {
	remoteMe.loop();

	int32_t newPosition;
	newPosition = encoder.read();
	if (newPosition != position) {
		if (newPosition % 4 == 0) {
			Serial.print("Left = ");
			Serial.println(newPosition / 4);

			sendChangeEvent(3, newPosition / 4);
			//encoder.write(0);
		}

		position = newPosition;

	}


	if (button.onPressed()) {
		Serial.println("Button1 Pressed");
		sendChangeEvent(2, 0);
	}

	
	if (i++ > 10000) {//when we read analog to often esp8266 is disconecting its known issue :( https://github.com/esp8266/Arduino/issues/1634 //its run the code every 200ms
		int currentPotentiometer = analogRead(analogPin);
		if (abs(currentPotentiometer - lastVoltage) > 20) {
			Serial.println(currentPotentiometer);
			lastVoltage = currentPotentiometer;
			sendChangeEvent(1, currentPotentiometer / 4);
		}
		i = 0;
		//Serial.println(millis());
	}
	
}