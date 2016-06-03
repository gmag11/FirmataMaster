#include <SoftwareSerial.h>
#include "FirmataClient.h"
//#include "FirmataArduino.h"


SoftwareSerial sserial(12,13);

void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);
	sserial.begin(57600);
	delay(2000);
	FirmataClient.begin(sserial);
	FirmataClient.pinMode(13, OUTPUT);
  /* add setup code here */

}

void loop()
{
	/*FirmataClient.digitalWrite(13, HIGH);
	delay(2000);
	FirmataClient.digitalWrite(13, LOW);
	delay(2000);*/
	FirmataClient.handleData();
	delay(0);
  /* add main program code here */

}
