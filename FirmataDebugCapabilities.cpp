// 
// 
// 


#include "FirmataClient.h"

#ifdef DEBUG_CAPABILITIES
static String mode2string(int mode); // Convert int to pin mode text

/**
* Gets all pins capabilities as an array of pins. For debugging only
*
* Param[out] Pointer to pin array
*/
pin * FirmataClientClass::getStoredCapabilities()
{
	if (gotCapabilities) {
		return pins;
	}
	else return NULL;
}

/**
* Sends pin configuration to Debug serial port. For debugging only
*/
void FirmataClientClass::printCapabilities()
{
	DBG_PORT.println(__PRETTY_FUNCTION__);
	for (int i = 0; i < MAX_PINS; i++) {
		if (pins[i].available) {
			DBG_PORT.printf("Pin %d modes\r\n", i);
			for (int j = 0; j < NUMBER_OF_MODES; j++) {
				if (pins[i].capability[j].supported) {
					DBG_PORT.printf("  Mode: %d = %s, resolution: %d", j, mode2string(j).c_str(), pins[i].capability[j].resolution);
					if (j == ANALOG) {
						DBG_PORT.printf(", analog channel: %d", pins[i].analogChannel);
					}
					DBG_PORT.println();
				}
			}
		}
	}
}

/**
* Convert int to pin mode text. For debugging only
*
* Param[in] Integer corresponding to pin mode
* Param[out] String that corresponds to mode
*/
static String mode2string(int mode) {
	switch (mode) {
	case INPUT:
		return ("Input");
		break;
	case OUTPUT:
		return ("Output");
		break;
	case ANALOG:
		return ("Analog");
		break;
	case PWM:
		return ("PWM");
		break;
	case SERVO:
		return ("Servo");
		break;
	case SHIFT:
		return ("Shift");
		break;
	case I2C:
		return ("I2C");
		break;
	case ONEWIRE:
		return ("OneWire");
		break;
	case STEPPER:
		return ("Stepper");
		break;
	case ENCODER:
		return ("Encoder");
		break;
	case SERIAL:
		return ("Serial");
		break;
	case PULLUP:
		return ("PULLUP");
		break;
	default:
		return ("Mode not implemented");
	}
}
#endif // DEBUG_CAPABILITIES