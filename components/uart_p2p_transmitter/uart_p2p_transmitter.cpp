#include "uart_p2p_transmitter.h"

namespace esphome {
namespace uart_p2p_transmitter {

static const char *const TAG = "uart_p2p_transmitter";
static const int headerSize = 3;

void UartP2pTransmitter::add_sensor(sensor::Sensor *sensor, uint8_t address) {
  sensor->add_on_state_callback([this, address](float sensorValue) {
    // Callback function triggered when the sensor state changes
    ESP_LOGD(TAG, "Send UART data - Address: 0x%02X, Value: %f", address, sensorValue);
    this->sendUartData(address, &sensorValue, sizeof(float), SensorType::NUMERIC);
  });
}

void UartP2pTransmitter::add_binary_sensor(binary_sensor::BinarySensor *binary_sensor, uint8_t address) {
  binary_sensor->add_on_state_callback([this, address](bool state) {
    // Callback function triggered when the binary sensor state changes
    uint8_t binaryValue = static_cast<uint8_t>(state);
    this->sendUartData(address, &binaryValue, sizeof(uint8_t), SensorType::BINARY);
  });
}

void UartP2pTransmitter::add_text_sensor(text_sensor::TextSensor *text_sensor, uint8_t address) {
  text_sensor->add_on_state_callback([this, address](const std::string &textValue) {
    // Callback function triggered when the text sensor state changes
    this->sendUartData(address, textValue.c_str(), textValue.length(), SensorType::TEXT);
  });
}

void UartP2pTransmitter::sendUartData(uint8_t address, const void *data, size_t dataLength, SensorType sensorType) {
  uint8_t uartData[headerSize + dataLength];
  uartData[0] = address;
  uartData[1] = static_cast<uint8_t>(sensorType);
  uartData[2] = dataLength;

  memcpy(uartData + headerSize, data, dataLength);

  uint8_t checksum = calculateChecksum(uartData, headerSize + dataLength);
  uartData[headerSize + dataLength] = checksum;

  this->write_array(uartData, headerSize + dataLength + sizeof(uint8_t));
}

uint8_t UartP2pTransmitter::calculateChecksum(const uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}

}
}