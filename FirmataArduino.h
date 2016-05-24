// Arduino.h

#ifndef _FIRMATA_ARDUINO_h
#define _FIRMATA_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Arduino {
	Arduino();
	Arduino(Stream* stream);
	Arduino(int serial, long bitrate = 57600);
	void imprimir() {
		Serial.begin(115200);
	}
};

#endif

