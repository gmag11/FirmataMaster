// 
// 
// 


#include "FirmataClient.h"

#ifdef FIRMATA_DIGITAL_OUTPUT_SUPPORT
/**
* Write to a digital pin (the pin must have been put into output mode with
* pinMode()).
*
* @param[in] pin to write to.
* @param[in] value to write: Arduino.LOW or Arduino.HIGH
*/
void FirmataClientClass::digitalWrite(int pin, int value) {
#ifdef DEBUG_DIGITAL_OUTPUT
	DBG_PORT.printf("-Digital Write: pin %d value %d\r\n", pin, value);
#endif // DEBUG_DIGITAL_OUTPUT	
	int portNumber = (pin >> 3) & 0x0F; // Get port corresponding to pin

										// Set bit corresponding to pin to 0 or 1
	if (value == 0)
		digitalOutputData[portNumber] &= ~(1 << (pin & 0x07));
	else
		digitalOutputData[portNumber] |= (1 << (pin & 0x07));

	// Send data
	firmataStream->write(DIGITAL_MESSAGE | portNumber);
	firmataStream->write(digitalOutputData[portNumber] & 0x7F);
	firmataStream->write(digitalOutputData[portNumber] >> 7);
}
#endif // FIRMATA_DIGITAL_OUTPUT_SUPPORT