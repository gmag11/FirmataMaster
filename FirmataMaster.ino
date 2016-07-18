#include <Ticker.h>
#include "FirmataClient.h"

//#define DIGITAL_OUTPUT_TEST
//#define DIGITAL_INPUT_TEST
//#define ANALOG_INPUT_TEST
#define PWM_OUTPUT_TEST

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
#ifdef DIGITAL_OUTPUT_TEST
	FirmataClient.pinMode(13, OUTPUT); // Enable builtin LED in Arduino UNO
#endif // DIGITAL_OUTPUT_TEST
#ifdef DIGITAL_INPUT_TEST
	FirmataClient.pinMode(9, INPUT);
#endif // DIGITAL_INPUT_TEST
#ifdef PWM_OUTPUT_TEST
	FirmataClient.pinMode(3, PWM);
#endif // PWM_OUTPUT_TEST
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
#ifdef DIGITAL_OUTPUT_TEST
	FirmataClient.digitalWrite(13, HIGH);
	delay(2000);
	FirmataClient.digitalWrite(13, LOW);
	delay(2000);
#endif // DIGITAL_OUTPUT_TEST
#endif // FIRMATA_DIGITAL_OUTPUT_SUPPORT

#ifdef FIRMATA_ANALOG_INPUT_SUPPORT
#ifdef ANALOG_INPUT_TEST
	float voltage0 = (float)FirmataClient.analogRead(0) * (5.0 / 1023);
	float voltage1 = (float)FirmataClient.analogRead(1) * (5.0 / 1023);
	float voltage2 = (float)FirmataClient.analogRead(2) * (5.0 / 1023);
	DBG_PORT.printf("A0: %4.3f V\r\n", voltage0);
	DBG_PORT.printf("A1: %4.3f V\r\n", voltage1);
	DBG_PORT.printf("A2: %4.3f V\r\n", voltage2);
#endif // ANALOG_INPUT_TEST
#endif // FIRMATA_ANALOG_INPUT_SUPPORT

#ifdef FIRMATA_PWM_OUTPUT_SUPPORT
#ifdef PWM_OUTPUT_TEST
	// Fade in LED
	for (int i = 0; i < 255; i++) {
		FirmataClient.analogWrite(3, i);
		delay(5);
	}
	// Fade out LED
	for (int i = 254; i >= 0; i--) {
		FirmataClient.analogWrite(3, i);
		delay(5);
	}
#endif // PWM_OUTPUT_TEST
#endif // FIRMATA_PWM_OUTPUT_SUPPORT
	delay(100);

}
