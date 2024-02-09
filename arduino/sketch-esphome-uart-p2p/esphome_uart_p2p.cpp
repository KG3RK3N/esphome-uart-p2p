#include "esphome_uart_p2p.h"

static const int headerSize = 3;

void EsphomeUartP2P::write(uint8_t address, const void *data, size_t dataLength, SensorType sensorType) {
  uint8_t uartData[headerSize + dataLength];
  uartData[0] = address;
  uartData[1] = static_cast<uint8_t>(sensorType);
  uartData[2] = dataLength;

  memcpy(uartData + headerSize, data, dataLength);

  uint8_t checksum = calculateChecksum(uartData, headerSize + dataLength);
  uartData[headerSize + dataLength] = checksum;

  this->port.write(uartData, headerSize + dataLength + sizeof(uint8_t));
}

uint8_t EsphomeUartP2P::calculateChecksum(const uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}