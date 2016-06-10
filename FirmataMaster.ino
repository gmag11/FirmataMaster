//#include <TimeAlarms.h>
#include <Ticker.h>
#include <SoftwareSerial.h>
#include "FirmataClient.h"

//#include "FirmataArduino.h"


//SoftwareSerial sserial(12,13);

void setup()
{
	Serial.begin(57600);
	Serial1.begin(115200);
	//sserial.begin(57600);
	delay(2000);

	FirmataClient.begin(Serial);
	FirmataClient.pinMode(13, OUTPUT);
  /* add setup code here */

}

void loop()
{
#ifdef DEBUG_CAPABILITIES
	FirmataClient.printCapabilities();
#endif // DEBUG_CAPABILITIES
	FirmataClient.digitalWrite(13, HIGH);
	delay(2000);
	FirmataClient.digitalWrite(13, LOW);
	delay(2000);
  /* add main program code here */

}
