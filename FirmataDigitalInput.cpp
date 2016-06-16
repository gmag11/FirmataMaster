// 
// 
// 


#include "FirmataClient.h"

#ifdef FIRMATA_DIGITAL_INPUT_SUPPORT
/**
* Returns the last known value read from the digital pin: HIGH or LOW.
*
* @param[in] pin the digital pin whose value should be returned. Firmata slave should control if pin mode allows data to be read.
* @param[out] Value of read pin: HIGH or LOW
*/
int FirmataClientClass::digitalRead(int pin) {
	int value = (digitalInputData[pin >> 3] >> (pin & 0x07)) & 0x01; // Conver a pin value to port/pin
#ifdef DEBUG_DIGITAL
	DBG_PORT.printf("-DigitalRead: pin %d value %d\r\n", pin, value);
#endif // DEBUG_DIGITAL	
	return value;
}

/**
* Stores digital input data locally
*
* @param[in] Port that has been read.
* @param[in] Bitmask with info about pin state.
*/
void FirmataClientClass::setDigitalInputs(int portNumber, int portData) {
#ifdef DEBUG_DIGITAL
	DBG_PORT.printf("-Set digital inputs: port %d data %x\r\n", portNumber, portData);
#endif // DEBUG_DIGITAL
	digitalInputData[portNumber] = portData;
}
#endif // FIRMATA_DIGITAL_INPUT_SUPPORT