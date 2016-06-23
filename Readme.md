# Firmata Client for Arduino & ESP8266
##Introduction
ESP8266 is a great platform for IOT and, after the development of its [Arduino toolchain](https://github.com/esp8266/Arduino), making custom scketches beyond factory AT firmware has become as easy as it is with other Arduino compatible boards.
ESP8266 performs faster and has more RAM and flash. But Arduino wins in number of input and outputs, specially in analog inputs.

In projects where a high number of digital and analog pins are needed makers normally prefer using Arduino or use an input/output extension via [I2C](http://playground.arduino.cc/Code/I2CPortExpander8574) or [multiplexing](http://playground.arduino.cc/Code/MUX151).

[Firmata](https://github.com/firmata/protocol/blob/master/README.md) is a protocol that allows using Arduino pins from external systems like Raspberry Pi or computer using languages like C#, Python, Javascript or Processing, among others.

Why not use Firmata to drive an Arduino from ESP8266 to use its input/output capabilities as it was local ones?.

Firmata was first developed to use a serial link between master (client) and slave (server), but any Stream object can be used as transmission medium for this protocol. This includes a TCP or UDP connection over WiFi. In this case Firmata master and slave can be hundreds kilometers far appart.

Firmata is a well known and tested protocol. It is currently evolving to support more pin capabilities beyond digital and analog input/output, like I2C, 1-Wire, Serial, stepper motor, etc. You can read info on [https://github.com/firmata/protocol/blob/master/README.md](https://github.com/firmata/protocol/blob/master/README.md)

##Wiring ESP8266 and Arduino
TODO

##Using Firmata
### Setting up Arduino
### Using the library
### Enabling debugging

## Supported Firmata capabilities
FirmataMaster has been designed as a counterpart of [ConfigurableFirmata](https://github.com/firmata/ConfigurableFirmata) library. Therefore, there is the intetion to develop all capabilities that ConfigurableFirmata implements:
- **Analog input** Already implemented, not working yet
- **Analog output** Already implemented, not tested yet
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
I'd like, at some point, to migrate to the same plugin structure that ConfigurableFirmata uses, but current main focus is make digital an analog I/O working fine.

## Examples
TODO
