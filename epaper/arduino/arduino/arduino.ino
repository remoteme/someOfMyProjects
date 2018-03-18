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

#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

#include <SPI.h>
#include "epd4in2.h"
#include "epdpaint.h"
#include "images.h"

#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

#define COLORED     0
#define UNCOLORED   1

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	Epd epd;

	if (epd.Init() != 0) {
		Serial.print("e-Paper init failed");
		return;
	}

	/* This clears the SRAM of the e-paper display */
	epd.ClearFrame();

	/**
	* Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
	* In this case, a smaller image buffer is allocated and you have to
	* update a partial display several times.
	* 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
	*/
	unsigned char image[1500];
	Paint paint(image, 400, 28);    //width should be the multiple of 8 

	paint.Clear(UNCOLORED);
	paint.DrawStringAt(0, 0, "Glupi krolik", &Font24, COLORED);
	epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

	paint.Clear(UNCOLORED);

	
	//paint.DrawStringAt(0, 0, "Bardzo durny", &Font24, COLORED);
	//epd.SetPartialWindow(paint.GetImage(), 100, 100, paint.GetWidth(), paint.GetHeight());

	
	unsigned char image2[800];
	for (int i = 0; i < 800; i++) {
		image2[i]=(unsigned char)(pgm_read_byte(&(I_50[i])));
	}

	
	epd.SetPartialWindow(image2, 80, 100, 80, 80);

	epd.DisplayFrame();

	/* This displays an image */

	
	/* Deep sleep */
	epd.Sleep();
}

void loop() {
	// put your main code here, to run repeatedly:

}

