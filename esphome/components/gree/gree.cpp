#include "gree.h"

namespace esphome {
namespace gree {

climate::ClimateTraits GreeClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supports_auto_mode(true);
  traits.set_supports_cool_mode(this->supports_cool_);
  traits.set_supports_heat_mode(this->supports_heat_);
  traits.set_visual_min_temperature(16.0f);
  traits.set_visual_max_temperature(30.0f);
  traits.set_visual_temperature_step(1.0f);
  traits.set_supports_two_point_target_temperature(false);
  return traits;
}

void GreeClimate::setup() {
  // This will be called by App.setup()
  m_ac = new IRGreeAC(this->data_pin_);
  m_ac->begin();
  delay(200);
  m_ac->on();
  m_ac->setFan(0);
  // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
  m_ac->setMode(kGreeAuto);
  m_ac->setSwingVertical(true, kGreeSwingAuto);
  m_ac->setXFan(false);
  m_ac->setLight(true);
  m_ac->setFan(0);
  m_ac->setSleep(false);
  m_ac->setTurbo(false);
  // sensible default
  this->target_temperature = 20.0f;
  this->publish_state();
}

void GreeClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    // User requested mode change
    this->mode = *call.get_mode();

    switch (mode) {
      case climate::CLIMATE_MODE_OFF:
        m_ac->off();
        break;
      case climate::CLIMATE_MODE_AUTO:
        m_ac->setMode(kGreeAuto);
        m_ac->on();
        break;
      case climate::CLIMATE_MODE_COOL:
        m_ac->setMode(kGreeCool);
        m_ac->on();
        break;
      case climate::CLIMATE_MODE_HEAT:
        m_ac->setMode(kGreeHeat);
        m_ac->on();
        break;
    }

    // Always set the target temperature
    // to make the switch between heat and
    // cool seamless.
    m_ac->setTemp(this->target_temperature);

    // Send mode to hardware
    m_ac->send();
    delay(200);

    // Publish updated state
    this->mode = mode;
    this->publish_state();
  }
  if (call.get_target_temperature().has_value()) {
    // User requested target temperature change
    float temp = *call.get_target_temperature();

    m_ac->setTemp(temp);
    m_ac->send();

    this->target_temperature = temp;
    this->publish_state();
  }
}

}  // namespace gree
}  // namespace esphome