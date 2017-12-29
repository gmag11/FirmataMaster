/*
Copyright (C) 2017 Juraj Andrássy

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

#include "FirmataSerial.h"

#ifdef FIRMATA_SERIAL_SUPPORT

FirmataSerial::FirmataSerial(FirmataClientClass *_firmataClient, int _portId, int _rxPin, int _txPin) {
  firmataClient = _firmataClient;
  portId = _portId;
  rxPin = _rxPin;
  txPin = _txPin;
}

void FirmataSerial::begin(uint32_t baud) {
  firmataClient->serialConfig(portId, baud, rxPin, txPin);
  firmataClient->serialRead(portId, FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH / 2, this);
}

void FirmataSerial::end() {
  firmataClient->serialClose(portId);
}

void FirmataSerial::listen() {
  firmataClient->serialListen(portId);
}

void FirmataSerial::readReply(const byte buff[], int size) {
  for (int i = 0; i < size; i++) {
    int n = rxBufferEnd + 1;
    if (n == FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH) {
      n = 0;
    }
    if (n == rxBufferStart) {
      rxOverflow = true;
      break;
    }
    rxBuffer[rxBufferEnd] = buff[i];
    rxBufferEnd = n;
  }
}

int FirmataSerial::available() {
  int l = rxBufferEnd - rxBufferStart;
  if (l == 0) { // if buffer empty
    flush(); //complete tx
    return 0;
  }
  if (l < 0)
    return l + FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH;
  return l;
}

int FirmataSerial::read() {
  if (!available())
    return -1;
  int b = rxBuffer[rxBufferStart++];
  if (rxBufferStart == FIRMATA_SERIAL_RX_RING_BUFFER_LENGTH) {
    rxBufferStart = 0;
  }
  return b;
}

int FirmataSerial::peek() {
  if (!available())
    return -1;
  return rxBuffer[rxBufferStart];
}

size_t FirmataSerial::write(uint8_t val) {
  txBuffer[txBufferLength++] = val;
  if (txBufferLength == FIRMATA_SERIAL_TX_BUFFER_LENGTH) {
    flush();
  }
  return 1;
}

size_t FirmataSerial::write(const uint8_t *buffer, size_t size) {
  size_t l;
  l = Print::write(buffer, size);
  flush();
  return l;
}

void FirmataSerial::flush() {
  if (txBufferLength == 0)
    return;
  firmataClient->serialWrite(portId, txBuffer, txBufferLength);
  firmataClient->serialFlush(portId);
  txBufferLength = 0;
}

#endif
