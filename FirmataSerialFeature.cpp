// 
// 
// 


#include "FirmataClient.h"

#ifdef FIRMATA_SERIAL_SUPPORT

void FirmataClientClass::serialConfig(int portId, long baud, int rxPin, int txPin) {
#ifdef DEBUG_SERIAL_FEATURE
	DBG_PORT.printf("-Serial Config: portId %d baud %ld\r\n", portId, baud);
#endif

	// Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_CONFIG | portId);
  firmataStream->write(baud & 0x007F);
  firmataStream->write((baud >> 7) & 0x007F);
  firmataStream->write((baud >> 14) & 0x007F);
  if (rxPin != 0 && txPin != 0) {
    firmataStream->write(rxPin);
    firmataStream->write(txPin);
  }
  firmataStream->write(END_SYSEX);
}

void FirmataClientClass::serialWrite(int portId, const byte inBytes[], int size) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Write: portId %d size %d %s\r\n", portId, size, inBytes);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_WRITE | portId);
  for (int i = 0; i < size; i++) {
    firmataStream->write(inBytes[i] & 0x007F);
    firmataStream->write((inBytes[i] >> 7) & 0x007F);
  }
  firmataStream->write(END_SYSEX);
}

void FirmataClientClass::serialRead(int portId, int maxBytesToRead, FirmataSerialReplyReader *firmataSerial) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Read: portId %d\r\n", portId);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_READ | portId);
  firmataStream->write((byte) 0x00); // read continuously
  firmataStream->write(maxBytesToRead & 0x007F);
  firmataStream->write((maxBytesToRead >> 7) & 0x007F);
  firmataStream->write(END_SYSEX);

  replyReaders[portId] = firmataSerial;
}

void FirmataClientClass::serialStop(int portId) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Stop: portId %d\r\n", portId);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_READ | portId);
  firmataStream->write(0x01); // stop reading
  firmataStream->write(END_SYSEX);
}

void FirmataClientClass::serialClose(int portId) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Close: portId %d\r\n", portId);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_CLOSE | portId);
  firmataStream->write(END_SYSEX);

}

void FirmataClientClass::serialFlush(int portId) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Flush: portId %d\r\n", portId);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_FLUSH | portId);
  firmataStream->write(END_SYSEX);
}

void FirmataClientClass::serialListen(int portId) {
#ifdef DEBUG_SERIAL_FEATURE
  DBG_PORT.printf("-Serial Listen: portId %d\r\n", portId);
#endif

  // Send data
  firmataStream->write(START_SYSEX);
  firmataStream->write(SERIAL_MESSAGE);
  firmataStream->write(SERIAL_LISTEN | portId);
  firmataStream->write(END_SYSEX);
}


#endif // FIRMATA_SERIAL_SUPPORT
