#include <Ticker.h>
#include "FirmataClient.h"
//#include <espsoftwareserial\SoftwareSerial.h>

//SoftwareSerial sserial(12,13); // SoftSerial port does not work reliablily

void setup()
{
	Serial.begin(57600);
	Serial1.begin(115200);
	Serial1.println();
	//sserial.begin(57600);
	//sserial.flush();
		
	delay(2000);

	FirmataClient.begin(Serial); // Start Firmata communication
	
	FirmataClient.pinMode(13, OUTPUT); // Enable builtin LED in Arduino UNO

#ifdef DEBUG_CAPABILITIES
	FirmataClient.printCapabilities(); // Check board capabilities
#endif // DEBUG_CAPABILITIES
}

void loop()
{
	// Flash LED every 2 seconds
#ifdef FIRMATA_DIGITAL_OUTPUT_SUPPORT
	FirmataClient.digitalWrite(13, HIGH);
	delay(2000);
	FirmataClient.digitalWrite(13, LOW);
	delay(2000);
#endif // FIRMATA_DIGITAL_OUTPUT_SUPPORT
}
