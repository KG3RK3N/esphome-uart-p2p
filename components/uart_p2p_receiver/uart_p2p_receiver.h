#pragma once

#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include "sensor_type.h"

namespace esphome {
namespace uart_p2p_receiver {

class UartP2pReceiver : public Component, public uart::UARTDevice {
 public:
  struct SensorInfo {
    sensor::Sensor *sensor;
    binary_sensor::BinarySensor *binary_sensor;
    text_sensor::TextSensor *text_sensor;
    uint8_t address;
    SensorType sensor_type;
  };

  void add_sensor(sensor::Sensor *sensor, uint8_t address);
  void add_binary_sensor(binary_sensor::BinarySensor *binary_sensor, uint8_t address);
  void add_text_sensor(text_sensor::TextSensor *text_sensor, uint8_t address);


  // void setup() override;
  void loop() override;

 protected:
  std::vector<SensorInfo> sensor_infos_;

  void handle_uart_data(const uint8_t *data, size_t size);
  uint8_t calculateChecksum(const uint8_t *data, size_t length);
};
}
}