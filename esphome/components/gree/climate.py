import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import climate
from esphome.const import CONF_ID, CONF_SUPPORTS_COOL, CONF_SUPPORTS_HEAT, CONF_DATA_PIN

gree_ns = cg.esphome_ns.namespace('gree')
GreeClimate = gree_ns.class_('GreeClimate', climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.All(climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(GreeClimate),
    cv.Optional(CONF_SUPPORTS_COOL, default=True): cv.boolean,
    cv.Optional(CONF_SUPPORTS_HEAT, default=True): cv.boolean,
    cv.Optional(CONF_DATA_PIN, default=2): cv.int_,
}).extend(cv.COMPONENT_SCHEMA))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add_library('IRremoteESP8266', '2.7.9')
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
    cg.add(var.set_supports_cool(config[CONF_SUPPORTS_COOL]))
    cg.add(var.set_supports_heat(config[CONF_SUPPORTS_HEAT]))
    cg.add(var.set_data_pin(config[CONF_DATA_PIN]))
