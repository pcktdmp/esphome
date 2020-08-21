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
  ac_ = new IRGreeAC(this->data_pin_);
  ac_->begin();
  delay(200);
  ac_->on();
  ac_->setFan(0);
  // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
  ac_->setMode(kGreeAuto);
  ac_->setSwingVertical(true, kGreeSwingAuto);
  ac_->setXFan(false);
  ac_->setLight(true);
  ac_->setFan(0);
  ac_->setSleep(false);
  ac_->setTurbo(false);
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
        ac_->off();
        break;
      case climate::CLIMATE_MODE_AUTO:
        ac_->setMode(kGreeAuto);
        ac_->on();
        break;
      case climate::CLIMATE_MODE_COOL:
        ac_->setMode(kGreeCool);
        ac_->on();
        break;
      case climate::CLIMATE_MODE_HEAT:
        ac_->setMode(kGreeHeat);
        ac_->on();
        break;
    }

    // Always set the target temperature
    // to make the switch between heat and
    // cool seamless.
    ac_->setTemp(this->target_temperature);

    // Send mode to hardware
    ac_->send();
    delay(200);

    // Publish updated state
    this->mode = mode;
    this->publish_state();
  }
  if (call.get_target_temperature().has_value()) {
    // User requested target temperature change
    float temp = *call.get_target_temperature();

    ac_->setTemp(temp);
    ac_->send();

    this->target_temperature = temp;
    this->publish_state();
  }
}

}  // namespace gree
}  // namespace esphome