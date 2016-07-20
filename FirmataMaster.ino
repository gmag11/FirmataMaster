#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "FirmataClient.h"

#define DIGITAL_OUTPUT_TEST
//#define DIGITAL_INPUT_TEST
#define ANALOG_INPUT_TEST
//#define PWM_OUTPUT_TEST

//#include <espsoftwareserial\SoftwareSerial.h>

//SoftwareSerial sserial(12,13); // SoftSerial port does not work reliablily
WiFiClient firmataRemoteBoard;

void reconnect() {
	if (!WiFi.isConnected()) {
		//firmataRemoteBoard.stop();
		WiFi.begin("ESP_0D9D00", "12345678");
		for (int i = 0; i < 20; i++) {
			delay(1000);
			if (WiFi.status() == WL_CONNECTED)
				break;
			DBG_PORT.print(".");
		}
		DBG_PORT.println();
		DBG_PORT.printf("IP address: %s\r\n", WiFi.localIP().toString().c_str());
	}
	if (WiFi.isConnected()) {
		if (!firmataRemoteBoard.connected()) {
			DBG_PORT.printf("Connection status 1: %d\r\n", firmataRemoteBoard.status());
			DBG_PORT.println("Connecting board ...");
			if (!firmataRemoteBoard.connect("192.168.4.1", 23)) {
				DBG_PORT.println("connection failed");
				return;
			}
			else
				DBG_PORT.printf("Connection status 2: %d\r\n", firmataRemoteBoard.status());
			delay(200);
		}
	}
}

void initFirmata() {
	FirmataClient.begin(firmataRemoteBoard); // Start Firmata communication

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

void setup()
{
	Serial.begin(115200);
	Serial.println();
	WiFi.mode(WIFI_STA);
	reconnect();
	if (WiFi.isConnected())
		initFirmata();
}

void loop()
{
	// Flash LED every 2 seconds
	if (WiFi.status() == WL_CONNECTED) {
#ifdef FIRMATA_DIGITAL_OUTPUT_SUPPORT
#ifdef DIGITAL_OUTPUT_TEST
		FirmataClient.digitalWrite(13, HIGH);
		delay(1000);
		FirmataClient.digitalWrite(13, LOW);
		delay(1000);
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
	}
	if (WiFi.status() != WL_CONNECTED) {
		reconnect();
		if (WiFi.status() == WL_CONNECTED)
			initFirmata();
	}
	delay(100);

}
