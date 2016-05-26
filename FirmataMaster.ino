#include <SoftwareSerial.h>
#include "FirmataClient.h"
//#include "FirmataArduino.h"

FirmataClient firmata;
SoftwareSerial sserial(12,13);

void setup()
{
	Serial.begin(115200);
	sserial.begin(57600);
	firmata.begin(sserial);
	firmata.pinMode(13, OUTPUT);
  /* add setup code here */

}

void loop()
{
	firmata.digitalWrite(13, HIGH);
	delay(500);
	firmata.digitalWrite(13, LOW);
	delay(500);
	firmata.handleData();
  /* add main program code here */

}
