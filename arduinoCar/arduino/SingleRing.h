
#ifndef SingleRing_h
#define SingleRing_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class SingleRing {

private:

	int ledCount;
	int type = 0;
	int r = 0;
	int g = 0;
	int b = 30;
	int z = 4;
	int div = 1;

	int counter;

	int prevI = -1;
	Adafruit_NeoPixel pixels;

	int floorDiv(int x, int y);
	int floorMod(int x, int y);
  int ret(int i);
	int floor(int i);

	void turnOff();


	void police(int i);
	void cross(int i);
	void clock(int i);
	void rider(int i);
	void blink(int i);
	void solid();
	void off();


public:
	SingleRing(int ledCount, int pin, bool turnOffTimeout);
	void setConfiguration(int type, int r, int g, int b, int z, int div);
	void setMode(int i);
	void setup();
	void clear();
	void step(int i);
  void loop();

};
#endif
