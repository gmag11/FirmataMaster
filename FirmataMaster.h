// FirmataMaster.h

#ifndef _FIRMATAMASTER_h
#define _FIRMATAMASTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Arduino {
	Arduino();
	Arduino(Stream* stream);
	Arduino(int serial, long bitrate = 9600);
	void imprimir() {
		Serial.begin(115200);
	}
};

#endif

