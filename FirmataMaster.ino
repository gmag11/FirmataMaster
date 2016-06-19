#include <Ticker.h>
#include "FirmataClient.h"
//#include <espsoftwareserial\SoftwareSerial.h>

//SoftwareSerial sserial(12,13); // SoftSerial port does not work reliablily

void setup()
{
	pinMode(14, OUTPUT);
	digitalWrite(14, HIGH);
	Serial.begin(57600);
	Serial.swap();
	Serial1.begin(115200);
	Serial1.println();
	//sserial.begin(57600);
	//sserial.flush();
		
	
	delay(2000);

	FirmataClient.begin(Serial); // Start Firmata communication

	FirmataClient.setSamplingInterval(1000);
	
	FirmataClient.pinMode(13, OUTPUT); // Enable builtin LED in Arduino UNO

	FirmataClient.pinMode(9, INPUT);

	DBG_PORT.printf("Protocol Version: %s\r\n", FirmataClient.getFirmataProtocolVersion().c_str());
	DBG_PORT.printf("Firmware: %s %s\r\n", FirmataClient.getFirmwareName().c_str(), FirmataClient.getFirmwareVersion().c_str());

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
	DBG_PORT.printf("A1: %d", FirmataClient.analogRead(1));
	delay(1000);
#endif // FIRMATA_DIGITAL_OUTPUT_SUPPORT
}
