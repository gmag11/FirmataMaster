// 
// 
// 


#include "FirmataClient.h"

#define DO_REPORT_ANALOG // Control if Analog report request is sent on start

static void checkStream(FirmataClientClass* firmata); // Callback to check input stream periodically

FirmataClientClass::FirmataClientClass()
{
}

/**
* Starts Firmata client attached to a Stream object.
* @param[in] Stream to use to connect to Firmata slave.
*/
void FirmataClientClass::begin(Stream &stream)
{
	firmataStream = &stream;

	// Chech stram for data every 50 miliseconds
	tk.attach_ms(50, checkStream, this);

	reset(); // Send reset to firmata board
	delay(1000);
	//queryFirmware(); // TODO: Check firmware to ensure a Firmata Board is connected
	delay(3000); // Wait for response

	
#ifdef FIRMATA_DIGITAL_INPUT_SUPPORT
	// enable all ports; firmware should ignore non-existent ones
	for (int i = 0; i < MAX_PORTS; i++) {
		firmataStream->write(REPORT_DIGITAL | i);
		firmataStream->write(1);
		//queryPinState(i); // TODO
	}
#endif // FIRMATA_DIGITAL_INPUT_SUPPORT

	delay(500);
	
	queryCapabilities(); //Query Firmata capabilities

	delay(1000);
#if defined(DO_REPORT_ANALOG) && defined(FIRMATA_ANALOG_INPUT_SUPPORT)
	queryAnalogMapping(); // Query analog inputs mapping
	delay(1000);

	// Enable analog reporting
	for (int i = 0; i < MAX_ANALOG_PINS; i++) {
		firmataStream->write(REPORT_ANALOG | i);
		firmataStream->write((uint8_t)1);
	}
#endif // DO_REPORT_ANALOG

}

/**
* Checks stream for input data
*
* @param[out] true if data is waiting ro be read
*/
bool FirmataClientClass::getStreamAvailable()
{
	return firmataStream->available();
}

/**
* Callback to check input stream periodically.
*
* @param[in] Firmata Client object to check.
*/
static void checkStream(FirmataClientClass* firmata) {
	FirmataClientClass *firmataInstance = firmata;
	if (!firmataInstance->checkingStream) {

		firmataInstance->checkingStream = true;
		while (firmataInstance->getStreamAvailable()) {
			firmataInstance->handleData();
		}
		firmataInstance->checkingStream = false;
	}
}

/**
* Send pin mode request. No answer is expected
*
* @param[in] pin to write to.
* @param[in] mode to configure pin
*/
void FirmataClientClass::pinMode(int pin, int mode)
{
#ifdef DEBUG_PINS
	DBG_PORT.printf("-Pin mode: pin %d mode %d\r\n", pin, mode);
#endif // DEBUG_PINS
	firmataStream->write(SET_PIN_MODE);
	firmataStream->write(pin);
	firmataStream->write(mode);
}

/**
* Configure analog and i2c data sampling interval
*
* @param[in] Interval in miliseconds.
*/
void FirmataClientClass::setSamplingInterval(int interval) {
#ifdef DEBUG_PINS
	DBG_PORT.printf("Set sampling interval at %d\r\n", interval);
#endif // DEBUG_PINS
	firmataStream->write(START_SYSEX);
	firmataStream->write(SAMPLING_INTERVAL);
	firmataStream->write(interval & 0xFF);
	firmataStream->write((interval >> 8) & 0xFF);
	firmataStream->write(END_SYSEX);
}

/**
* Stores version of Firmata board
*
* @param[in] Major version.
* @param[in] Minor version.
*/
void FirmataClientClass::setVersion(int major, int minor) {
//#ifdef DEBUG_FIRMATA
	DBG_PORT.printf("Firmata Version: %d %d\r\n", major, minor);
//#endif //DEBUG_FIRMATA
	majorVersion = major;
	minorVersion = minor;
}

/**
* Stores Firmware name of Firmata board.
* TODO: BUG: Firmware name assignment triggers an exception.
*
* @param[in] Firmware name.
*/
void FirmataClientClass::setFirmwareName (const char* name) {
#ifdef DEBUG_FIRMATA
	DBG_PORT.printf("Firmware name: %s\r\n", name);
#endif //DEBUG_FIRMATA
	memcpy(firmwareName, 0, 40); // Initializes name
	strcpy(firmwareName, name);
	//firmwareName = name; // <-- This triggers an exception (3) or (29)
#ifdef DEBUG_FIRMATA
	DBG_PORT.printf("Firmware name: %s\r\n", firmwareName);
#endif //DEBUG_FIRMATA
}

/**
* Sends firmware version query to Firmata board.
*/
void FirmataClientClass::queryFirmware() {
#ifdef DEBUG_FIRMATA_PINS
	DBG_PORT.print("Query capabilities\r\n");
#endif // DEBUG_FIRMATA_PINS
	firmataStream->write(START_SYSEX);
	firmataStream->write(REPORT_FIRMWARE);
	firmataStream->write(END_SYSEX);
}

/**
* Sends capabilities query to Firmata board.
*/
void FirmataClientClass::queryCapabilities() {
#ifdef DEBUG_FIRMATA_PINS
	DBG_PORT.print("Query capabilities\r\n");
#endif // DEBUG_FIRMATA_PINS
	firmataStream->write(START_SYSEX);
	firmataStream->write(CAPABILITY_QUERY);
	firmataStream->write(END_SYSEX);
}

/**
* Sends reset query to Firmata board.
*/
void FirmataClientClass::reset(){
#ifdef DEBUG_FIRMATA
	DBG_PORT.print("Reset Firmata board\r\n");
#endif // DEBUG_ANALOG	
	firmataStream->write(SYSTEM_RESET);
}

/**
* Process a byte received from Firmata board
*
* Param[in] Received byte
*/
void FirmataClientClass::processInput(int inputData) {
	int command;

	if (parsingSysex) {
		if (inputData == END_SYSEX) { // Sysex message end received
			parsingSysex = false;
			//DBG_PORT.print("sysex end ");
			//DBG_PORT.println(sysexBytesRead);
			processSysexMessage(); // Process complete message
		}
		else { // Still receiving SysEx message
			storedInputData[sysexBytesRead] = inputData;
			sysexBytesRead++; // Count read bytes
		}
	}
	else if (waitForData > 0 && inputData < 128) { // Receiving a multy byte message
		waitForData--;
		storedInputData[waitForData] = inputData;

		if (executeMultiByteCommand != 0 && waitForData == 0) { // If message has been received completely
			//we got everything
			switch (executeMultiByteCommand) {
#ifdef FIRMATA_DIGITAL_INPUT_SUPPORT
			case DIGITAL_MESSAGE: // Store digital input data
#ifdef DEBUG_DIGITAL
				DBG_PORT.println("DIGITAL MESSAGE");
#endif // DEBUG_DIGITAL	
				setDigitalInputs(multiByteChannel, (storedInputData[0] << 7) + storedInputData[1]); 
				break;
#endif // FIRMATA_DIGITAL_INPUT_SUPPORT
#ifdef FIRMATA_ANALOG_INPUT_SUPPORT
			case ANALOG_MESSAGE: // Store analog input data
#ifdef DEBUG_ANALOG
				DBG_PORT.println("ANALOG MESSAGE");
#endif // DEBUG_ANALOG
				setAnalogInput(multiByteChannel, (storedInputData[0] << 7) + storedInputData[1]);
				break;
#endif // FIRMATA_ANALOG_INPUT_SUPPORT
			case REPORT_VERSION: // Store firmware version
#ifdef DEBUG_FIRMATA
				DBG_PORT.println("REPORT VERSION");
#endif // DEBUG_FIRMATA
				setVersion(storedInputData[1], storedInputData[0]);
				break;
			} // Ignore unknown messages
		}
	}
	else { 
		if (inputData < 0xF0) { 
			// Messages starting with < 0xF0 include channel in second nibble
			// Separate command and channel
			command = inputData & 0xF0; 
			multiByteChannel = inputData & 0x0F;
		}
		else {
			command = inputData;
			// commands in the 0xF* range don't use channel data
		}
#ifdef DEBUG_FIRMATA_PROTOCOL
		//DBG_PORT.printf("Firmata command: %X %X\r\n", command, multiByteChannel);
#endif // DEBUG_FIRMATA_PROTOCOL
		switch (command) {
			case DIGITAL_MESSAGE:
			case ANALOG_MESSAGE:
			case REPORT_VERSION:
				// All these are 3 bytes commands
				waitForData = 2;
				executeMultiByteCommand = command;
				break;
			case START_SYSEX:
				// Start SysEx message
				parsingSysex = true;
				sysexBytesRead = 0;
				//DBG_PORT.println("Start SysEx");
				break;
		}
	}
}

/**
* Read input stream and process data
*/
void FirmataClientClass::handleData()
{
	int data = -1;
	if (firmataStream->available() > 0) {
		data = firmataStream->read(); // Read a byte
#ifdef DEBUG_PROTOCOL_BYTES
		String temp = (data < 0x10 ? " 0x0" : " 0x");
		DBG_PORT.printf("%s%X", temp.c_str(), data);
#endif // DEBUG_PROTOCOL_BYTES	
		if (data != -1) {
			processInput(data);
		}
	}

}

/**
* Processes stored SysEx received message
*/
void FirmataClientClass::processSysexMessage() {
  int SyXCommand = storedInputData[0];
#ifdef DEBUG_SYSEX
	DBG_PORT.printf("Process SysEx: SE Command: %X\r\n", SyXCommand);
#endif // DEBUG_SYSEX
	switch (SyXCommand) { //first byte in buffer is command
      case CAPABILITY_RESPONSE:
#ifdef DEBUG_SYSEX
		  DBG_PORT.print("Capability response\r\n");
#endif // DEBUG_SYSEX
#ifdef DEBUG_CAPABILITIES
		  gotCapabilities = true;
#endif // DEBUG_CAPABILITIES
		  for (int pin = 0; pin < MAX_PINS; pin++) {
          pinModes[pin] = 0;
#ifdef DEBUG_CAPABILITIES
		  pins[pin].available = false;
#endif //DEBUG_CAPABILITIES
        }
        for (int i = 1, pin = 0; pin < MAX_PINS; pin++) {
          for (;;) { // Continue until all bytes are read
            int val = storedInputData[i++];
            if (val == 127) break;
            pinModes[pin] |= (1 << val);
#ifdef DEBUG_CAPABILITIES
			pins[pin].available = true;
			if (val >= 0 && val < NUMBER_OF_MODES) {
				pins[pin].capability[val].supported = true;
			}
			int res = storedInputData[i++];
			pins[pin].capability[val].resolution = res;
#else
			i++; // skip mode resolution for now
#endif // DEBUG_CAPABILITIES
			//i++; 
          }
          if (i == sysexBytesRead) break; // End of message
        }
        for (int port = 0; port < MAX_PORTS; port++) { // For all ports
          boolean used = false;
          for (int pin = 0; pin < 8; pin++) { // For every port pin check if it is configured as input
            if (pinModes[port * 8 + pin] & (1 << INPUT) != 0)
				used = true;
          }
          if (used) { // Enable report for used input pins
			  firmataStream->write(REPORT_DIGITAL | port);
			  firmataStream->write(1);
          }
        }
        break;
#ifdef FIRMATA_ANALOG_INPUT_SUPPORT
	case ANALOG_MAPPING_RESPONSE:
#ifdef DEBUG_SYSEX
		DBG_PORT.print("Analog mapping response\r\n");
#endif // DEBUG_SYSEX
		for (int pin = 0; pin < MAX_ANALOG_PINS; pin++) {
			analogChannel[pin] = 127;
#ifdef DEBUG_CAPABILITIES
			pins[pin].analogChannel = 127; 
#endif //DEBUG_CAPABILITIES
		}
		for (int i = 1; i < sysexBytesRead; i++) {
			int val = storedInputData[i];
			analogChannel[i - 1] = val;
#ifdef DEBUG_CAPABILITIES
			if (val < 127)
				pins[i-1].analogChannel = val;
#endif // DEBUG_CAPABILITIES
		}
#ifdef DO_REPORT_ANALOG
		for (int pin = 0; pin < MAX_ANALOG_PINS; pin++) { // Enable analog report for enabled channels
			if (analogChannel[pin] != 127) {
				firmataStream->write(REPORT_ANALOG | analogChannel[pin]);
				firmataStream->write(1);
			}
		}
#endif // DO_REPORT_ANALOG
		break;
#endif // FIRMATA_ANALOG_INPUT_SUPPORT
	case REPORT_FIRMWARE:
		//DBG_PORT.print("Report Firmware ");
		//DBG_PORT.println(sysexBytesRead);
		String temp = "";
		setVersion(storedInputData[1], storedInputData[2]); // Store Firmware version
		for (int i = 3; i < sysexBytesRead; i++) { // Read firmware name
			if (storedInputData[i] != 0)
				temp += (char)storedInputData[i];
		}
		
#ifdef DEBUG_SYSEX
		DBG_PORT.printf("SysEx bytes read: %d\r\n", sysexBytesRead);
		DBG_PORT.printf("Firmware: %s\r\n", temp.c_str());
#endif //DEBUG_SYSEX

		setFirmwareName(temp.c_str()); // Store firmware name
		break;
	}
}


FirmataClientClass FirmataClient;

