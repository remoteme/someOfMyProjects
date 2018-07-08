#include "SingleRing.h"


SingleRing::SingleRing(int ledCount, int pin,bool turnOffTimeout) {
	pixels = Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
	this->ledCount = ledCount;
	if (turnOffTimeout) {
		counter = 0;
	}
	else {
		counter = -1;
	}
}

void SingleRing::setup() {
	pixels.begin();
	clear();
}

void SingleRing::clear() {
	prevI = -1;
	div = 1;
	turnOff();
	pixels.show();
}


void SingleRing::loop(){
  static unsigned long nextCall=0;
  static int i;
  if (millis()>nextCall){
     step(i++);
     nextCall=millis()+8;
  }
 
}

void SingleRing::setConfiguration(int type, int r, int g, int b, int z, int div) {
	this->type = type;
	this->r = r;
	this->g = g;
	this->b = b;
	this->z = z;
	this->div = div;
	if (counter!=-1){
		counter = 100 * 30;//60sek
	}
	
}


void SingleRing::setMode(int m) {
	switch (m) {
	case 0:setConfiguration(0, 0, 50, 0, 5, 1); break;//off =0
	case 1:setConfiguration(6, 0, 50, 0, 0, 20); break;//solid standard green
	case 2:setConfiguration(6, 0, 0, 50, 0, 20); break;//solid standard blue
	case 3:setConfiguration(6, 50, 0, 0, 0, 20); break;//solid standard red
	case 4:setConfiguration(6, 50, 10, 0, 0, 20); break;//solid standard orange
	case 5:setConfiguration(1, 0, 100, 0, 5, 2); break;//police clockwise green
	case 6:setConfiguration(1, 0, 100, 0, 5, -2); break;// police revert green
	case 7:setConfiguration(1, 0, 0, 100, 5, 2); break;//police clockwise blue
	case 8:setConfiguration(1, 0, 0, 100, 5, -2); break;// police revert blue
	case 9:setConfiguration(1, 100, 0, 0, 5, 2); break;//police standard red
	case 10:setConfiguration(1, 100, 0, 0, 5, -2); break;// police revert red
	case 11:setConfiguration(1, 100, 20, 0, 5, 2); break;//police standard orange
	case 12:setConfiguration(1, 100, 20, 0, 5, -2); break;// police revert orange
	case 13:setConfiguration(2, 0, 0, 50, 8, 10); break;//cross standard blue
	case 14:setConfiguration(2, 0, 0, 50, 8, -10); break;// cross revert blue
	case 15:setConfiguration(5, 0, 50, 0, 0, 20); break;//blink standard green
	case 16:setConfiguration(5, 0, 50, 0, 0, -20); break;// blink odwyrtka green
	case 17:setConfiguration(5, 0, 0, 50, 0, 20); break;//blink standard blue
	case 18:setConfiguration(5, 0, 0, 50, 0, -20); break;// blink revert blue
	case 19:setConfiguration(5, 50, 0, 0, 0, 20); break;//blink standard red
	case 20:setConfiguration(5, 50, 0, 0, 0, -20); break;// blink revert red
	case 21:setConfiguration(5, 50, 10, 0, 0, 20); break;//blink standard orange
	case 22:setConfiguration(5, 50, 10, 0, 0, -20); break;// blink revert orange
  case 23:setConfiguration(4, 200, 0, 0, 7, 7); break;//rider red
  case 24:setConfiguration(4, 0, 0, 200, 7, 7); break;//rider blue
  case 25:setConfiguration(4, 200, 0, 0, 7, 1); break;//rider red fast
	default: setConfiguration(0, 0, 100, 0, 8, 1); break;//off =0
	}
}

void SingleRing::step(int i) {


	turnOff();

	if (counter != -1) {
		if (counter == 0) {
			pixels.show();//commit off
			return;
		}

		counter--;
	}
	

	i = i / abs(div);

	if (i == prevI) {
		return;
	}




	prevI = i;

	if (type == 1) {
		police(i);
	}
	else if (type == 2) {
		cross(i);
	}
	else if (type == 3) {
		clock(i);
	}
	else if (type == 4) {
		rider(i);
	}
	else if (type == 5) {
		blink(i);
	}
	else if (type == 6) {
		solid();
	}
	else {
		off();
	}

	pixels.show(); // send
}
int SingleRing::floorDiv(int x, int y) {


	int r = x / y;
	// if the signs are different and modulo not zero, round down
	if ((x ^ y) < 0 && (r * y != x)) {
		r--;
	}
	return r;
}
int SingleRing::floorMod(int x, int y) {
	int r = x - floorDiv(x, y) * y;
	return r;
}

int SingleRing::floor(int i) {
	return floorMod(i, ledCount);
}

int SingleRing::ret(int i){
    i=i%(ledCount*2-2);
    if (i<ledCount){
      return i;
    }else{
      return  (ledCount*2-2)-i;
    }
  }

// r g b colors z-tail
void SingleRing::police(int i) {
	int n = div<0 ? -1 : 1;

	for (int x = 0; x<z; x++) {
		pixels.setPixelColor(floor((i + x)*n), r*x / z, g*x / z, b*x / z);
	}
}

// r g b colors z-space beetwen leds
void SingleRing::cross(int i) {
	int n = div<0 ? -1 : 1;

	int x = 0;
	while (x<ledCount) {
		if (x%z == 0) {
			pixels.setPixelColor(floor((i + x)*n), r, g, b);
		}
		else {
			pixels.setPixelColor(floor((i + x)*n), 0, 0, 0);
		}
		x++;

	}
}

void SingleRing::turnOff() {
	for (int x = 0; x<ledCount; x++) {
		pixels.setPixelColor(x, 0, 0, 0);
	}
}

// r g b colors z-nothing
void SingleRing::clock(int i) {
	int n = div<0 ? -1 : 1;



	for (int x = 0; x<floor(i*n); x++)
		pixels.setPixelColor(x, r, g, b);

}

//rgb color z - how many leds
void SingleRing::rider(int i) {

	turnOff();

	

	for (int x = z-1; x>=0; x--) {
		pixels.setPixelColor(ret(z+i-x), r/(2*x+1), g/(2*x+1), b/(2*x+1));
	}
}


// r g b colors ofcourse  z-do nohtning
void SingleRing::blink(int i) {
	int mn = i % 2 == 0 ? 0 : 1;

	if (div<0) {
		mn = abs(mn - 1);//jakbylo 0 to beidze 1 etc
	}

	for (int x = 0; x<ledCount; x++) {
		pixels.setPixelColor(x, r*mn, g*mn, b*mn);
	}
}


void SingleRing::solid() {

	for (int x = 0; x<ledCount; x++) {
		pixels.setPixelColor(x, r, g, b);
	}
}



  

void SingleRing::off() {

}
