#include <Arduino.h>

class EsphomeUartP2P {
  public:
    enum SensorType {
        NUMERIC,
        BINARY,
        TEXT,
    };

    EsphomeUartP2P(Stream &p) : port(p) {}
    void write(uint8_t address, const void *data, size_t dataLength, SensorType sensorType);

  private:
    Stream &port;
    uint8_t EsphomeUartP2P::calculateChecksum(const uint8_t *data, size_t length);

};