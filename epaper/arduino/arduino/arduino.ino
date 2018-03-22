/**
*  @filename   :   epd4in2-demo.ino
*  @brief      :   4.2inch e-paper display demo
*  @author     :   Yehui from Waveshare
*
*  Copyright (C) Waveshare     August 4 2017
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#define RST_PIN         6
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

#include <SPI.h>
#include "epd4in2.h"
#include "epdpaint.h"
#include "images.h"
#include "RemoteMe.h"



#include "Wire.h"
#include "SPI.h"

#include "secret.h"

#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>


#define RST_PIN         6
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

#define COLORED     0
#define UNCOLORED   1



ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

Epd epd;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	delay(1000);
	Serial.println("started");


	//remoteme stuff
	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}
	Serial.println("connected");

	remoteMe.setupTwoWayCommunication();

	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
	//remoteme stuff end
	Serial.println("registeres send");



	if (epd.Init() != 0) {
		Serial.print("e-Paper init failed");
		return;
	}
}


const char* getDayOfWeek(uint8_t index) {
	switch (index)
	{
	case 0: return "Sun";
	case 1: return "Mon";
	case 2: return "Thu";
	case 3: return "Wed";
	case 4: return "Thr";
	case 5: return "Fri";
	case 6: return "Sat";
	default:
		break;
	}
}

const String getWithLeadingZero(uint8_t index) {
	if (index < 10) {
		return "0" + String(index);
	}
	else {
		return "" + index;
	}
}

void invert(unsigned char* paint,int size) {
	for (int i = 0; i < size / 8; i++) {
		paint[i] = ~paint[i];
	}

}
void displayWeather(uint8_t index, uint8_t today, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, int8_t temp, uint8_t icon) {


	unsigned char image[1500];
	Paint paint(image, 100, 53);    //width should be the multiple of 8 

	paint.Clear(UNCOLORED);

	uint8_t leftMargin = 12;

	paint.DrawStringAt(leftMargin+8, 0, getDayOfWeek(dayOfWeek), &Font24, COLORED);


	char buffer[6];

	
	sprintf(buffer, "%02d", dayOfMonth);
	paint.DrawStringAt(leftMargin, 25, buffer, &Font24, COLORED);
	paint.DrawFilledCircle(leftMargin + 35, 40, 2, COLORED);
	sprintf(buffer, "%02d", month);
	paint.DrawStringAt(leftMargin + 38, 25, buffer, &Font24, COLORED);

	if (today){
		invert(paint.GetImage(), 100 * 53);
	}
	epd.SetPartialWindow(paint.GetImage(), 100 * index, 0, paint.GetWidth(), paint.GetHeight());



	

	

	unsigned char image2[800];
	for (int i = 0; i < 800; i++) {
		if (icon == 1) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_01[i])));
		}else if (icon == 2) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_02[i])));
		}else if (icon == 3) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_03[i])));
		}else if (icon == 4) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_04[i])));
		}else if (icon == 9) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_09[i])));
		}else if (icon == 10) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_10[i])));
		}else if (icon == 11) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_11[i])));
		}else if (icon == 13) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_13[i])));
		}
		else if (icon == 50) {
			image2[i] = (unsigned char)(pgm_read_byte(&(I_50[i])));
		}
		
	}
	

	epd.SetPartialWindow(image2, index *100+10, 53, 80, 80);


	Paint paintTemp(image, 100, 53);    //width should be the multiple of 8 

	paintTemp.Clear(UNCOLORED);
	//int8_t temp2 = *(int8_t*)&temp;


	sprintf(buffer, "%dC", temp);
	paintTemp.DrawStringAt(leftMargin+10, 0, buffer, &Font24, COLORED);
	
	epd.SetPartialWindow(paintTemp.GetImage(), 100 * index, 53+80+10, paintTemp.GetWidth(), paintTemp.GetHeight());




}
void loop() {
	delay(1500);
	remoteMe.loop();



	/* This clears the SRAM of the e-paper display */
	epd.ClearFrame();

	uint8_t* returnData;
	
	uint16_t size = remoteMe.sendUserSyncMessage(1234, nullptr, 0, returnData);
	if (size != 0xFFFF) {

		Serial.println("got size:");
		Serial.println(size);

		uint16_t index = 0;
		for (uint8_t i = 0; i < 4; i++) {
			uint8_t today = RemoteMeMessagesUtils::getByte(returnData, index);
			uint8_t dayOfWeek = RemoteMeMessagesUtils::getByte(returnData, index);
			uint8_t day = RemoteMeMessagesUtils::getByte(returnData, index);
			uint8_t month = RemoteMeMessagesUtils::getByte(returnData, index);
			int8_t temp = RemoteMeMessagesUtils::getSignedByte(returnData, index);
			uint8_t icon = RemoteMeMessagesUtils::getByte(returnData, index);

			displayWeather(i,today,dayOfWeek,day,month,temp,icon);
			Serial.println(day);
		}
		epd.DisplayFrame();
		
		
	}
	else {
		Serial.println("ddint got proper answear");
	}
	epd.Sleep();
	// put your main code here, to run repeatedly:
	delay(100);
	remoteMe.disconnect();
	ESP.deepSleep(20e6);
}

