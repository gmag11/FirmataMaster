// FirmataClient.h

#ifndef _FIRMATACLIENT_h
#define _FIRMATACLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Ticker.h>

#define DEBUG_FIRMATA_MASTER
#ifdef DEBUG_FIRMATA_MASTER
//#define DEBUG_FIRMATA
//#define DEBUG_FIRMATA_PROTOCOL
//#define DEBUG_ANALOG
#define DEBUG_DIGITAL
#define DEBUG_PINS
#define DEBUG_SYSEX
//#define DEBUG_PROTOCOL_BYTES
//#define DEBUG_CAPABILITIES //Show pin capabilities over DBG_PORT
#endif
#define DBG_PORT Serial1


// pin modes definition
#define INPUT	0 //Constant to set a pin to input mode (in a call to pinMode())
#define OUTPUT	1 //Constant to set a pin to output mode (in a call to pinMode())
#define ANALOG	2 //Constant to set a pin to analog mode (in a call to pinMode())
#define PWM		3 //Constant to set a pin to PWM mode (in a call to pinMode())
#define SERVO	4 //Constant to set a pin to servo mode (in a call to pinMode())
#define SHIFT	5 //Constant to set a pin to shiftIn/shiftOut mode (in a call to pinMode())
#define I2C		6 //Constant to set a pin to I2C mode (in a call to pinMode())
#define ONEWIRE 7 //Constant to set a pin to OneWire mode (in a call to pinMode())
#define STEPPER 8 //Constant to set a pin to Stepper Motor mode (in a call to pinMode())
#define ENCODER 9 //Constant to set a pin to Rotary Encoder mode (in a call to pinMode())
#define SERIAL	10 //Constant to set a pin to Serial mode (in a call to pinMode())
#define PULLUP	11 //Constant to set a pin to pullup output mode (in a call to pinMode())
#define NUMBER_OF_MODES 12 //Number of defined modes to reserve memory in capability arrays.

//Digital mode states
#define HIGH	1 // Constant to write a high value to a pin (in a call to digitalWrite())
#define LOW		0 // Constant to write a low value to a pin (in a call to digitalWrite())

// Firmata basic definitions
#define MAX_DATA_BYTES 4096
#define MAX_PINS 128
#define MAX_PORTS 16
#define MAX_ANALOG_PINS 16

// Command codes (non sysex)
#define DIGITAL_MESSAGE	0x90 // send data for a digital port
#define ANALOG_MESSAGE	0xE0 // send data for an analog pin (or PWM)
#define REPORT_ANALOG	0xC0 // enable analog input by pin #
#define REPORT_DIGITAL	0xD0 // enable digital input by port
#define SET_PIN_MODE	0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
#define REPORT_VERSION	0xF9 // report firmware version
#define SYSTEM_RESET	0xFF // reset from MIDI
#define START_SYSEX		0xF0 // start a MIDI SysEx message
#define END_SYSEX		0xF7 // end a MIDI SysEx message

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */
#define RESERVED_COMMAND		0x00 // 2nd SysEx data byte is a chip-specific command (AVR, PIC, TI, etc)
#define TONE_DATA				0x5F // Call Arduino tone() and notone()
#define SERIAL_MESSAGE			0x60 // communicate with serial devices, including other boards
#define ENCODER_DATA			0x61 // Incremental encoder data
#define ANALOG_MAPPING_QUERY	0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE	0x6A // reply with mapping info
#define CAPABILITY_QUERY		0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE		0x6C // reply with supported modes and resolution
#define PIN_STATE_QUERY			0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE		0x6E // reply with pin's current mode and value
#define EXTENDED_ANALOG			0x6F // analog write (PWM, Servo, etc) to any pin
#define SERVO_CONFIG			0x70 // set max angle, minPulse, maxPulse, freq
#define STRING_DATA				0x71 // a string message with 14-bits per char
#define STEPPER_DATA			0x72 // Stepper motor command
#define ONEWIRE_DATA			0x73 // OneWire command
#define SHIFT_DATA				0x75 // a bitstream to/from a shift register
#define I2C_REQUEST				0x76 // send an I2C read/write request
#define I2C_REPLY				0x77 // a reply to an I2C read request
#define I2C_CONFIG				0x78 // config I2C settings such as delay times and power pins
#define REPORT_FIRMWARE			0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL		0x7A // set the poll rate of the main loop
#define SCHEDULER_COMMAND		0x7B // configures schedules of commands
#define SYSEX_NON_REALTIME		0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME			0x7F // MIDI Reserved for realtime messages

#ifdef DEBUG_CAPABILITIES

// Represents one pin single capability
typedef struct {
	bool supported = false; // Is this mode supported?
	int resolution = 0; // Numeric resolution
} pinCapability;

// Describes capabilities of a single pin 
typedef struct {
	bool available = false; // Is this pin available?
	pinCapability capability[NUMBER_OF_MODES]; // List of all possible capabilities
	int analogChannel = 127; // Analog channel. Only applicable to analog capable pins
} pin;
#endif // DEBUG_CAPABILITIES

class FirmataClientClass
{
 protected:
#ifdef DEBUG_CAPABILITIES
	 bool gotCapabilities = false; // Client has received info about Firmata board cababilities
	 pin pins[MAX_PINS]; // Pins description store
#endif // DEBUG_CAPABILITIES

	 int waitForData = 0; // Is client waiting for more data?
	 int executeMultiByteCommand = 0; // Number of current command remaining bytes to receive
	 int multiByteChannel = 0; 
	 int storedInputData[MAX_DATA_BYTES]; // Command buffer
	 boolean parsingSysex = false; // Command being received is sysex
	 int sysexBytesRead; // Number of bytes read on current sysex command

	 int digitalOutputData[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	 int digitalInputData[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	 int analogInputData[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	 int pinModes[MAX_PINS];
	 int analogChannel[MAX_ANALOG_PINS];
	 //int pinMode[MAX_PINS];

	 int majorVersion = 0;
	 int minorVersion = 0;
	 String firmware = "";
	 
	 Stream *firmataStream;

	 void setDigitalInputs(int portNumber, int portData);
	 void setAnalogInput(int pin, int value);
	 void setVersion(int majorVersion, int minorVersion);
	 void processSysexMessage();
	 void processInput(int inputData);

	 void queryCapabilities();
	 void queryAnalogMapping();
	 
	 Ticker tk;

 public:
	bool checkingStream = false;

	FirmataClientClass();
	void begin(Stream &stream);
	int digitalRead(int pin);
	bool getStreamAvailable();
	//static void checkStream(FirmataClientClass firmata);
	void digitalWrite(int pin, int value);
	void pinMode(int pin, int mode);
	void setSamplingInterval(int interval);
	
	void handleData();

#ifdef DEBUG_CAPABILITIES
	pin* getStoredCapabilities();
	void printCapabilities();
#endif // DEBUG_CAPABILITIES
};

extern FirmataClientClass FirmataClient;

#endif

