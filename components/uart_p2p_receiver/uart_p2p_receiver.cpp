#include "uart_p2p_receiver.h"

namespace esphome {
namespace uart_p2p_receiver {

static const char *const TAG = "uart_p2p_receiver";
static const int headerSize = 3;

void UartP2pReceiver::add_sensor(sensor::Sensor *sensor, uint8_t address) {
  this->sensor_infos_.push_back({sensor, nullptr, nullptr, address, SensorType::NUMERIC});
}

void UartP2pReceiver::add_binary_sensor(binary_sensor::BinarySensor *binary_sensor, uint8_t address) {
  this->sensor_infos_.push_back({nullptr, binary_sensor, nullptr, address, SensorType::BINARY});
}

void UartP2pReceiver::add_text_sensor(text_sensor::TextSensor *text_sensor, uint8_t address) {
  this->sensor_infos_.push_back({nullptr, nullptr, text_sensor, address, SensorType::TEXT});
}

void UartP2pReceiver::loop() {
  if (this->available() > headerSize) {
    uint8_t headerBuffer[headerSize];
    this->read_array(headerBuffer, headerSize);
    uint8_t messageSize = headerBuffer[2];

    if (this->available() >= messageSize + 1) {  // +1 for checksum
      this->read_array(headerBuffer + headerSize, messageSize + 1);
      this->handle_uart_data(headerBuffer, headerSize + messageSize + 1);
    }
  }
}

void UartP2pReceiver::handle_uart_data(const uint8_t *data, size_t size) {
  if (size < headerSize) {
    ESP_LOGW(TAG, "Received invalid UART data");
    return;
  }

  uint8_t address = data[0];
  SensorType sensorType = static_cast<SensorType>(data[1]);
  uint8_t sizeOfData = data[2];

  if (size != headerSize + sizeOfData + sizeof(uint8_t)) {
    ESP_LOGW(TAG, "Received UART data size does not match expected size. - Current: %d, Expected: %d", size, sizeOfData + sizeof(uint8_t) + 3);
    return;
  }

  uint8_t receivedChecksum = data[size - 1];
  uint8_t calculatedChecksum = calculateChecksum(data, size - 1);

  if (receivedChecksum != calculatedChecksum) {
    ESP_LOGW(TAG, "Received UART data with invalid checksum");
    return;
  }

  ESP_LOGD(TAG, "Received UART data - Address: 0x%02X, Type: %d, Size: %d", address, sensorType, sizeOfData);

  for (const SensorInfo &sensor_info : this->sensor_infos_) {
    if (sensor_info.address == address && sensor_info.sensor_type == sensorType) {
      // Match found, publish the received data to the corresponding sensor

      switch (sensorType) {
        case SensorType::NUMERIC: {
          float receivedValue;
          memcpy(&receivedValue, data + headerSize, sizeof(float));
          ESP_LOGI(TAG, "Received value for address 0x%02X: %f", address, receivedValue);
          sensor_info.sensor->publish_state(receivedValue);
          break;
        }
        case SensorType::BINARY: {
          uint8_t receivedValue;
          memcpy(&receivedValue, data + headerSize, sizeof(uint8_t));
          ESP_LOGI(TAG, "Received binary value for address 0x%02X: %d", address, receivedValue);
          sensor_info.binary_sensor->publish_state(static_cast<bool>(receivedValue));
          break;
        }
        case SensorType::TEXT: {
          std::string textValue = std::string((char *)data + headerSize, sizeOfData);
          ESP_LOGI(TAG, "Received text value for address 0x%02X: %s", address, textValue.c_str());
          sensor_info.text_sensor->publish_state(textValue);
          break;
        }
      }
      return;  // Exit loop after matching address and sensorType
    }
  }

  // No matching sensor found for the received address and sensorType
  ESP_LOGW(TAG, "Received UART data with unknown address: 0x%02X and sensorType: %d", address, sensorType);
}

uint8_t UartP2pReceiver::calculateChecksum(const uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}

}
}