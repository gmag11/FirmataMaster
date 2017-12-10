/*
Copyright (C) 2017 Juraj Andrássy
repository https://github.com/jandrassy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FiRMATASERIAL_H_
#define _FiRMATASERIAL_H_

#include "FirmataClient.h"

#ifdef FIRMATA_SERIAL_SUPPORT

#define FIRMATA_SERIAL_TX_BUFFER_LENGTH 28
#define FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH 128

class FirmataSerial : public Stream, public FirmataSerialReplyReader {

public:

  FirmataSerial(FirmataClientClass *firmataClient, int portId, int rxPin = 0, int txPin = 0);

  void begin(uint32_t baud);
  void end();
  void listen();

  // Stream implementation
  int read();
  int available();
  int peek();

  boolean overflow() {return rxOverflow;}

  // Print implementation
  size_t write(uint8_t val);
  size_t write(const uint8_t *buffer, size_t size);
  using Print::printf;
  void flush();

  //FirmataSerialReplayReader implementation
  void readReply(const byte buff[], int size);

private:

  FirmataClientClass* firmataClient;
  int portId;
  int rxPin;
  int txPin;

  byte rxBuffer[FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH];
  int rxBufferStart = 0;
  int rxBufferEnd = 0;
  boolean rxOverflow = false;

  byte txBuffer[FIRMATA_SERIAL_TX_BUFFER_LENGTH];
  int txBufferLength = 0;

public:
  // virtual destructor to get rid of compilation warning
  virtual ~FirmataSerial() {};

};

#endif
#endif
