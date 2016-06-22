# Firmata Client for Arduino & ESP8266
##Introduction
ESP8266 is a great platform for IOT and, after the development of its Arduino toolchain, making custom scketches beyond factory AT firmware has become as easy as it is with other Arduino compatible boards.
ESP8266 performs faster and has more RAM and flash. But Arduino wins in number of input and outputs, specially in analog inputs.

In projects where a high number of digital and analog pins are needed makers normally prefer using Arduino or use an input/output extension via I2C or multiplexing.

Firmata is a protocol that allows using Arduino pins from external systems like a Raspberry Pi or a computer using languages like C#, Python, Javascript or Processing, among others.

Why not use Firmata to drive an Arduino from ESP8266 to use its input/output capabilities as it was local ones?.

Firmata was first developed to use a serial link between master (client) and slave (server), but any stream object can be used as transmission medium for this protocol. This includes a TCP connection over WiFi. In this case Firmata master and slave can be hundreds kilometers far appart.

Firmata is a well known and tested protocol. It is currently evolving to support more pin capabilities beyond digital and analog input/output, like I2C, 1-Wire, Serial, stepper motor, etc. You can read info on https://github.com/firmata/protocol
