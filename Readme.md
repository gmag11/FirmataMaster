# Firmata Client for Arduino & ESP8266
##Introduction
ESP8266 is a great platform for IOT and, after the development of its [Arduino toolchain](https://github.com/esp8266/Arduino), making custom scketches beyond factory AT firmware has become as easy as it is with other Arduino compatible boards.
ESP8266 performs faster and has more RAM and flash. But Arduino wins in number of input and outputs, specially in analog inputs.

In projects where a high number of digital and analog pins are needed makers normally prefer using Arduino or use an input/output extension via [I2C](http://playground.arduino.cc/Code/I2CPortExpander8574) or [multiplexing](http://playground.arduino.cc/Code/MUX151).

[Firmata](https://github.com/firmata/protocol/blob/master/README.md) is a protocol that allows using Arduino pins from external systems like Raspberry Pi or computer using languages like C#, Python, Javascript or Processing, among others.

Why not use Firmata to drive an Arduino from ESP8266 to use its input/output capabilities as it was local ones?.

Firmata was first developed to use a serial link between master (client) and slave (server), but any Stream object can be used as transmission medium for this protocol. This includes a TCP or UDP connection over WiFi. In this case Firmata master and slave can be hundreds kilometers far appart. There is some limitation on using Software Serial ports. See more details in "Using library" down below.

Firmata is a well known and tested protocol. It is currently evolving to support more pin capabilities beyond digital and analog input/output, like I2C, 1-Wire, Serial, stepper motor, etc. You can read info on [https://github.com/firmata/protocol/blob/master/README.md](https://github.com/firmata/protocol/blob/master/README.md)

##Wiring ESP8266 and Arduino
TODO

##Using Firmata
### Setting up Arduino
You need to flash a Firmata firmware on Arduino. Up to now, to develop FirmataMaster, I'm had [ConfigurableFirmata example](https://github.com/firmata/ConfigurableFirmata/tree/master/examples/ConfigurableFirmata) loaded on my Arduino Uno. If you plan to use Firmata using an IP TCP or UDP stream, you may try [another example](https://github.com/firmata/ConfigurableFirmata/tree/master/examples/ConfigurableFirmataWiFi) in Configurable Firmata GitHub repository. Although this has not been tested yet. I will be so pleased if you do that test and give me some feedback.

### Using the library
Library has been designed to be as similar to programming directly in arduino as possible. Main methods are named the same way that they are in Arduino SDK.
Library needs to be copied to your Arduino `library` folder
You need to add `#include <FirmataMaster.h>` in your scketch first.
During setup stage, your scketch should create a Stream object like Serial, TCP client, UDP client or any other that inherits Stream class so it implements methods like `available()`, `write()` and `read()`.
All Stream initialization is responsability of your scketch as it is the task to do reconnections in case of a disconnection as it may happen using IP transport. I plan to add some example code. Any help is wellcome :-).
After that you can use any pin on Arduino remotely.

### Enabling debugging
There are some debug directives that are commented by default:
```Arduino
// DEBUG options
#define DEBUG_FIRMATA_MASTER // Global DEBUG control
#ifdef DEBUG_FIRMATA_MASTER
//#define DEBUG_FIRMATA
//#define DEBUG_FIRMATA_PROTOCOL
//#define DEBUG_ANALOG_INPUT
//#define DEBUG_PWM_OUTPUT
//#define DEBUG_DIGITAL_INPUT
//#define DEBUG_DIGITAL_OUTPUT
//#define DEBUG_PINS
//#define DEBUG_SYSEX
//#define DEBUG_PROTOCOL_BYTES
//#define DEBUG_CAPABILITIES //Show pin capabilit over DBG_PORT. Enable only if necessary as it consumes a lot of RAM space
#endif
```
You are free to enable any debug capability to dump debug info to any stream (Serial1 by default).
**Important:** Notice that some of them may throw a considerable amount of data that may affect interrupts and cause watchdog to reboot ESP board.


## Supported Firmata capabilities
FirmataMaster has been designed as a counterpart of [ConfigurableFirmata](https://github.com/firmata/ConfigurableFirmata) library. Therefore, there is the intetion to develop all capabilities that ConfigurableFirmata implements:
- **Analog input** Already implemented
- **Analog (PWM) output** Already implemented
- **Digital input** Already implemented
- **Digital output** Already implemented
- **~~Serial ports~~** TODO
- **~~Rotary encoders~~** TODO
- **~~Servos~~** TODO
- **~~Stepper motors~~** TODO
- **~~Onewire~~** TODO
- **~~I2C~~** TODO
- **~~Scheduling tasks~~** TODO
- **~~Tone~~** TODO

Development of capabilities will be done as I need them for my projects. If you need an specific capability that is not already implemented you may ask for it in issue tracker or make a pull request with your development.
I'd like, at some point, to migrate to the same plugin structure that ConfigurableFirmata uses, but current main focus is make digital and analog I/O working fine.

## Examples
TODO
