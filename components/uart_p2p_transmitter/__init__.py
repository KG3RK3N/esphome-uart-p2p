import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import (
    binary_sensor,
    sensor,
    text_sensor,
    uart,
)
from esphome.const import (
    CONF_ID,
    CONF_BINARY_SENSORS,
    CONF_SENSOR_ID,
    CONF_SENSORS,
    CONF_TEXT_SENSORS,
    CONF_ADDRESS,
)

CODEOWNERS = ["@KG3RK3N"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "binary_sensor", "text_sensor"]
MULTI_CONF = True

uart_p2p_ns = cg.esphome_ns.namespace("uart_p2p_transmitter")
UartP2pTransmitterComponent = uart_p2p_ns.class_(
    "UartP2pTransmitter", cg.Component, uart.UARTDevice
)

UART_P2P_TRANSMITTER_BINARY_SENSOR_SCHEMA = cv.maybe_simple_value(
    {
        cv.Required(CONF_SENSOR_ID): cv.use_id(binary_sensor.BinarySensor),
        cv.Required(CONF_ADDRESS): cv.hex_uint8_t,
    },
    key=CONF_SENSOR_ID,
)

UART_P2P_TRANSMITTER_SENSOR_SCHEMA = cv.maybe_simple_value(
    {
        cv.Required(CONF_SENSOR_ID): cv.use_id(sensor.Sensor),
        cv.Required(CONF_ADDRESS): cv.hex_uint8_t,
    },
    key=CONF_SENSOR_ID,
)

UART_P2P_TRANSMITTER_TEXT_SENSOR_SCHEMA = cv.maybe_simple_value(
    {
        cv.Required(CONF_SENSOR_ID): cv.use_id(text_sensor.TextSensor),
        cv.Required(CONF_ADDRESS): cv.hex_uint8_t,
    },
    key=CONF_SENSOR_ID,
)

UART_P2P_TRANSMITTER_CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UartP2pTransmitterComponent),
            cv.Optional(CONF_BINARY_SENSORS): cv.ensure_list(
                UART_P2P_TRANSMITTER_BINARY_SENSOR_SCHEMA,
            ),
            cv.Optional(CONF_SENSORS): cv.ensure_list(
                UART_P2P_TRANSMITTER_SENSOR_SCHEMA,
            ),
            cv.Optional(CONF_TEXT_SENSORS): cv.ensure_list(
                UART_P2P_TRANSMITTER_TEXT_SENSOR_SCHEMA,
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

CONFIG_SCHEMA = cv.All(
    UART_P2P_TRANSMITTER_CONFIG_SCHEMA,
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "uart_p2p_transmitter", require_tx=True
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    for bsensor in config.get(CONF_BINARY_SENSORS, []):
        bs = await cg.get_variable(bsensor[CONF_SENSOR_ID])
        cg.add(var.add_binary_sensor(bs, bsensor[CONF_ADDRESS]))
    for s in config.get(CONF_SENSORS, []):
        bs = await cg.get_variable(s[CONF_SENSOR_ID])
        cg.add(var.add_sensor(bs, s[CONF_ADDRESS]))
    for tsensor in config.get(CONF_TEXT_SENSORS, []):
        bs = await cg.get_variable(tsensor[CONF_SENSOR_ID])
        cg.add(var.add_text_sensor(bs, tsensor[CONF_ADDRESS]))