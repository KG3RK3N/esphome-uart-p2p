#include "esphome_uart_p2p.h"

EsphomeUartP2P esphome(Serial); // <--- here define the serial to use (Serial, Serial1, etc.)

void setup() {
  Serial.begin(115200);
}

void loop() {
  sendMySensorValue();
  delay(5000);
}

// sample to send sensor value
void sendMySensorValue() {
  uint8_t address = 0x01;                                                       // thats the address from the esphome sensor
  float sensorValue = random(0,100);                                            // the value to send
  EsphomeUartP2P::SensorType sensorType = EsphomeUartP2P::SensorType::NUMERIC;  // the type of sensor (NUMERIC, BINARY, TEXT)

  esphome.write(address, &sensorValue, sizeof(sensorValue), sensorType);        // Write to serial
}

