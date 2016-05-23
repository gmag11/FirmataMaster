// 
// 
// 

#include "FirmataClient.h"

FirmataClient::FirmataClient(Stream &stream)
{
	firmataStream = &stream;
}

void FirmataClient::init()
{
	// enable all ports; firmware should ignore non-existent ones
	for (int i = 0; i < MAX_PORTS; i++) {
		firmataStream->write(REPORT_DIGITAL | i);
		firmataStream->write(1);
	}
	
	queryCapabilities();
	queryAnalogMapping();

	for (int i = 0; i < MAX_ANALOG_PINS; i++) {
		firmataStream->write(REPORT_ANALOG | i);
		firmataStream->write(1);
	}

}

void FirmataClient::queryCapabilities() {
	firmataStream->write(START_SYSEX);
	firmataStream->write(CAPABILITY_QUERY);
	firmataStream->write(END_SYSEX);
}

void FirmataClient::queryAnalogMapping() {
	firmataStream->write(START_SYSEX);
	firmataStream->write(ANALOG_MAPPING_QUERY);
	firmataStream->write(END_SYSEX);
}


//FirmataClient FirmataClient;

