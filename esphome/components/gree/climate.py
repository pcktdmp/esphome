import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import climate
from esphome.const import CONF_ID, CONF_SUPPORTS_COOL,\
    CONF_SUPPORTS_HEAT, CONF_DATA_PIN, CONF_SWING_MODE,\
    CONF_MIN_TEMPERATURE, CONF_MAX_TEMPERATURE, CONF_TEMPERATURE_STEP,\
    CONF_SWING_MODE, CONF_LIGHT, CONF_MAX_POWER, CONF_DEFAULT_TARGET_TEMPERATURE

gree_ns = cg.esphome_ns.namespace('gree')
GreeClimate = gree_ns.class_('GreeClimate', climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.All(climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(GreeClimate),
    cv.Optional(CONF_SUPPORTS_COOL, default=True): cv.boolean,
    cv.Optional(CONF_SUPPORTS_HEAT, default=True): cv.boolean,
    cv.Optional(CONF_DEFAULT_TARGET_TEMPERATURE, default=20.0): cv.float_,
    cv.Optional(CONF_MIN_TEMPERATURE, default=16.0): cv.float_,
    cv.Optional(CONF_MAX_TEMPERATURE, default=30.0): cv.float_,
    cv.Optional(CONF_TEMPERATURE_STEP, default=1.0): cv.float_,
    cv.Optional(CONF_SWING_MODE, default=True): cv.boolean,
    cv.Optional(CONF_LIGHT, default=False): cv.boolean,
    cv.Optional('sleep', default=False): cv.boolean,
    cv.Optional('xfan', default=False): cv.boolean,
    cv.Optional(CONF_MAX_POWER, default=False): cv.boolean,
    cv.Optional('ir_pin', default=4): cv.int_,
}).extend(cv.COMPONENT_SCHEMA))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add_library('IRremoteESP8266', '2.7.9')
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
    cg.add(var.set_supports_cool(config[CONF_SUPPORTS_COOL]))
    cg.add(var.set_supports_heat(config[CONF_SUPPORTS_HEAT]))
    cg.add(var.set_visual_min_temperature(config[CONF_MIN_TEMPERATURE]))
    cg.add(var.set_visual_max_temperature(config[CONF_MAX_TEMPERATURE]))
    cg.add(var.set_visual_temperature_step(config[CONF_TEMPERATURE_STEP]))
    cg.add(var.set_default_target_temperature(config[CONF_DEFAULT_TARGET_TEMPERATURE]))
    cg.add(var.set_ac_swing(config[CONF_SWING_MODE]))
    cg.add(var.set_ac_light(config[CONF_LIGHT]))
    cg.add(var.set_ac_sleep(config['sleep']))
    cg.add(var.set_ac_xfan(config['xfan']))
    cg.add(var.set_ac_turbo(config[CONF_MAX_POWER]))
    cg.add(var.set_ir_pin(config['ir_pin']))
