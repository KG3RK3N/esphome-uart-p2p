# esphome-uart-p2p

![GitHub stars](https://img.shields.io/github/stars/KG3RK3N/esphome-uart-p2p)
![GitHub forks](https://img.shields.io/github/forks/KG3RK3N/esphome-uart-p2p)
![GitHub watchers](https://img.shields.io/github/watchers/KG3RK3N/esphome-uart-p2p)

ESPHome components to send/receive sensor values between two ESPHome devices over UART without WIFI.

## Features

- [x] Sensor support
- [x] Binary sensor support
- [x] Text sensor support
- [x] CRC check
- [x] Value type check
- [x] Multi Transmitter/Receiver support for multiple UARTs (like ESP32)

## Requirements

* [ESPHome 2023.12.9 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32

## Schematics

```

┌──────────┐                ┌─────────┐
│          │<----- RX ----->│         │
│  ESP32   │<----- TX ----->│  ESP32  │
│          │<----- GND ---->│         │<-- 3.3V
│          │                │         │<-- GND
└──────────┘                └─────────┘

│            o GND ── GND                │
│            o TXD ── RXD (`rx_pin`)     │
│            o RXD ── TXD (`tx_pin`)     │
└─[oooooooo]─────────────────[oooooooo]──┘
```

For test purposes you can connect the RXD & TXD pins together of one esp and than use both components on one device.

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like the following examples:

### Transmitter
This component send the data to another devices.

```yaml
external_components:
  - source:
      url: github://KG3RK3N/esphome-uart-p2p
      type: git
    components: [uart_p2p_transmitter]
```

### Receiver
This component receive data from a device with a transmitter.
```yaml
external_components:
  - source:
      url: github://KG3RK3N/esphome-uart-p2p
      type: git
    components: [uart_p2p_receiver]
```

## Configuration
In the following samples you can see, that each sensor has a address to assign the values to each sensor. 
Its necessary to have unique addresses and the sensor type needs to be the same on both sides.

```
    Transmitter                 Receiver

Valid:
   0x01 (number)    ------>   0x01 (number)
   0x02 (binary)    ------>   0x02 (binary)

Invalid:
   0x02 (number)    ------>   0x02 (binary)
   0x02 (binary)    ------>   0x02 (number)
   0x02 (number)    ------>   0x03 (number)
```

### Transmitter
```yaml
uart_p2p_transmitter:
  uart: uart_1
  sensors:
    - sensor_id: random_sensor_1
      address: 0x01
  binary_sensors:
    - sensor_id: binary_sensor_1
      address: 0x02
  text_sensors:
    - sensor_id: text_sensor_1
      address: 0x03
```
[Here](esp32-transmitter-sample.yaml "ESP32 Transmitter") you can find a full sample for the transmitter.

### Receiver
```yaml
uart_p2p_receiver:
  uart_id: uart_1
  sensors:
    - id: random_sensor_1
      name: "Sensor address 0x01"
      unit_of_measurement: "%"
      address: 0x01
  binary_sensors:
    - id: binary_sensor_1
      name: "Binary sensor address 0x02"
      address: 0x02
  text_sensors:
    - id: text_sensor_1
      name: "Text sensor address 0x03"
      address: 0x03
```

[Here](esp32-receiver-sample.yaml "ESP32 Receiver") you can find a full sample for the receiver.

## More samples

- [Arduino -> Esphome](arduino/ESP32_ARDUINO.md)

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to enable the debug output of the UART component and increase the log level to the see outgoing and incoming serial traffic:

```yaml
logger:
  level: DEBUG

uart:
  id: uart_0
  baud_rate: 115200
  tx_pin: GPIO17
  rx_pin: GPIO16
  debug:
    direction: BOTH
```