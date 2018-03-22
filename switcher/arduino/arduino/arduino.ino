#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 202
#define DEVICE_NAME "switcher"
#define TOKEN "~1_VLTq=uGG@M1Hp"

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



// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(D5, D6);



uint8_t LEDpin = D3;
int analogPin = A0;

long positionLeft = -999;
long positionRight = -999;


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


	servo.attach(2); //D4

	servo.write(0);

	//----- display
	Wire.begin();
	lcd.begin(16, 2); // Inicjalizacja LCD 2x16
	lcd.setCursor(0, 0);
	lcd.print("maciek");
}


void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData) {

}

void onUserMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t *data) {


}

int i = 0;
void loop() {
	remoteMe.loop();

	long newLeft, newRight;
	newLeft = knobLeft.read();
	if (newLeft != positionLeft) {
		if (newLeft % 4 == 0) {
			Serial.print("Left = ");
			Serial.print(newLeft / 4);

			Serial.println();
		}

		positionLeft = newLeft;
		positionRight = newRight;
	}


	if (button.onPressed()) {
		Serial.println("Button Pressed");
	}

	i++;
	analogWrite(LEDpin, i);
	delay(4);
	if (i > 1024) {
		i = 0;
		Serial.println("reset");
	}

	if (i < 512) {
		servo.write(10);
	}
	else {
		servo.write(200);
	}

	if (i % 256 == 0) {
		Serial.println(analogRead(analogPin));
	}
}