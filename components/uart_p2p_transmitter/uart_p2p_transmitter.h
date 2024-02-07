#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include "sensor_type.h"

namespace esphome {
namespace uart_p2p_transmitter {

class UartP2pTransmitter : public Component, public uart::UARTDevice {
 public:
  void add_sensor(sensor::Sensor *sensor, uint8_t address);
  void add_binary_sensor(binary_sensor::BinarySensor *binary_sensor, uint8_t address);
  void add_text_sensor(text_sensor::TextSensor *text_sensor, uint8_t address);

  void sendUartData(uint8_t address, const void *data, size_t dataLength, SensorType sensor_type);
 protected:
  uint8_t calculateChecksum(const uint8_t *data, size_t length);
};
}
}