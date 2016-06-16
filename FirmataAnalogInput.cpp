// 
// 
// 


#include "FirmataClient.h"

#ifdef FIRMATA_ANALOG_INPUT_SUPPORT
int FirmataClientClass::analogRead(int pin)
{
	if (pin <= 0 && pin < 16)
		return analogInputData[pin];
	else
		return -1;
}

/**
* Stores analog input data locally
*
* @param[in] Pin that has been read.
* @param[in] Value to store.
*/
void FirmataClientClass::setAnalogInput(int pin, int value) {
#ifdef DEBUG_ANALOG
	//DBG_PORT.printf("-Set analog inputs: pin %d value %d\r\n", pin, value);
#endif // DEBUG_ANALOG	
	analogInputData[pin] = value;
}

/**
* Sends analog mapping query to Firmata board.
*/
void FirmataClientClass::queryAnalogMapping() {
#ifdef DEBUG_ANALOG
	DBG_PORT.print("Query analog mapping\r\n");
#endif // DEBUG_ANALOG	
	firmataStream->write(START_SYSEX);
	firmataStream->write(ANALOG_MAPPING_QUERY);
	firmataStream->write(END_SYSEX);
}
#endif // FIRMATA_ANALOG_INPUT_SUPPORT