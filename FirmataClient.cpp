// 
// 
// 

#include "FirmataClient.h"

FirmataClient::FirmataClient()
{
	
}

void FirmataClient::begin(Stream &stream)
{
	firmataStream = &stream;
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

/**
* Returns the last known value read from the digital pin: HIGH or LOW.
*
* @param pin the digital pin whose value should be returned (from 2 to 13,
* since pins 0 and 1 are used for serial communication)
*/
int FirmataClient::digitalRead(int pin) {
	return (digitalInputData[pin >> 3] >> (pin & 0x07)) & 0x01;
}

/**
* Write to a digital pin (the pin must have been put into output mode with
* pinMode()).
*
* @param pin the pin to write to (from 2 to 13)
* @param value the value to write: Arduino.LOW (0 volts) or Arduino.HIGH
* (5 volts)
*/
void FirmataClient::digitalWrite(int pin, int value) {
	int portNumber = (pin >> 3) & 0x0F;

	if (value == 0)
		digitalOutputData[portNumber] &= ~(1 << (pin & 0x07));
	else
		digitalOutputData[portNumber] |= (1 << (pin & 0x07));

	firmataStream->write(DIGITAL_MESSAGE | portNumber);
	firmataStream->write(digitalOutputData[portNumber] & 0x7F);
	firmataStream->write(digitalOutputData[portNumber] >> 7);
}

void FirmataClient::pinMode(int pin, int mode)
{
	firmataStream->write(SET_PIN_MODE);
	firmataStream->write(pin);
	firmataStream->write(mode);
}

void FirmataClient::setDigitalInputs(int portNumber, int portData) {
	//System.out.println("digital port " + portNumber + " is " + portData);
	digitalInputData[portNumber] = portData;
}

void FirmataClient::setAnalogInput(int pin, int value) {
	//System.out.println("analog pin " + pin + " is " + value);
	analogInputData[pin] = value;
}

void FirmataClient::setVersion(int majorVersion, int minorVersion) {
	//System.out.println("version is " + majorVersion + "." + minorVersion);
	this->majorVersion = majorVersion;
	this->minorVersion = minorVersion;
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

void FirmataClient::processInput(int inputData) {
	int command;

	//    System.out.print(">" + inputData + " ");

	if (parsingSysex) {
		if (inputData == END_SYSEX) {
			parsingSysex = false;
			processSysexMessage();
		}
		else {
			storedInputData[sysexBytesRead] = inputData;
			sysexBytesRead++;
		}
	}
	else if (waitForData > 0 && inputData < 128) {
		waitForData--;
		storedInputData[waitForData] = inputData;

		if (executeMultiByteCommand != 0 && waitForData == 0) {
			//we got everything
			switch (executeMultiByteCommand) {
			case DIGITAL_MESSAGE:
				setDigitalInputs(multiByteChannel, (storedInputData[0] << 7) + storedInputData[1]);
				break;
			case ANALOG_MESSAGE:
				setAnalogInput(multiByteChannel, (storedInputData[0] << 7) + storedInputData[1]);
				break;
			case REPORT_VERSION:
				setVersion(storedInputData[1], storedInputData[0]);
				break;
			}
		}
	}
	else {
		if (inputData < 0xF0) {
			command = inputData & 0xF0;
			multiByteChannel = inputData & 0x0F;
		}
		else {
			command = inputData;
			// commands in the 0xF* range don't use channel data
		}
		switch (command) {
			case DIGITAL_MESSAGE:
			case ANALOG_MESSAGE:
			case REPORT_VERSION:
				waitForData = 2;
				executeMultiByteCommand = command;
				break;
			case START_SYSEX:
				parsingSysex = true;
				sysexBytesRead = 0;
				break;
		}
	}
}

void FirmataClient::handleData()
{
	int data = -1;

	if (firmataStream->available() > 0) {
		data = firmataStream->read();
		if (data != -1) {
			processInput(data);
		}
	}

}

void FirmataClient::processSysexMessage() {
	//    System.out.print("[ ");
	//    for (int i = 0; i < storedInputData.length; i++) System.out.print(storedInputData[i] + " ");
	//    System.out.println("]");
	switch (storedInputData[0]) { //first byte in buffer is command
      case CAPABILITY_RESPONSE:
        for (int pin = 0; pin < MAX_PINS; pin++) {
          pinModes[pin] = 0;
        }
        for (int i = 1, pin = 0; pin < MAX_PINS; pin++) {
          for (;;) {
            int val = storedInputData[i++];
            if (val == 127) break;
            pinModes[pin] |= (1 << val);
            i++; // skip mode resolution for now
          }
          if (i == sysexBytesRead) break;
        }
        for (int port = 0; port < MAX_PINS; port++) {
          boolean used = false;
          for (int pin = 0; pin < 8; pin++) {
            if (pinModes[port * 8 + pin] & (1 << INPUT) != 0) used = true;
          }
          if (used) {
			  firmataStream->write(REPORT_DIGITAL | port);
			  firmataStream->write(1);
          }
        }
        break;
	case ANALOG_MAPPING_RESPONSE:
		for (int pin = 0; pin < MAX_ANALOG_PINS; pin++)
			analogChannel[pin] = 127;
		for (int i = 1; i < sysexBytesRead; i++)
			analogChannel[i - 1] = storedInputData[i];
		for (int pin = 0; pin < MAX_ANALOG_PINS; pin++) {
			if (analogChannel[pin] != 127) {
				firmataStream->write(REPORT_ANALOG | analogChannel[pin]);
				firmataStream->write(1);
			}
		}
		break;
	}
}


//FirmataClient FirmataClient;

