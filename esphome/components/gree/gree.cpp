#include "gree.h"

namespace esphome {
namespace gree {

climate::ClimateTraits GreeClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supports_auto_mode(true);
  traits.set_supports_cool_mode(this->supports_cool_);
  traits.set_supports_heat_mode(this->supports_heat_);
  traits.set_visual_min_temperature(this->visual_min_temperature_);
  traits.set_visual_max_temperature(this->visual_max_temperature_);
  traits.set_visual_temperature_step(this->visual_temperature_step_);
  traits.set_supports_two_point_target_temperature(false);
  return traits;
}

void GreeClimate::setup() {
  // https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRsend.h
  ac_ = new IRGreeAC(this->ir_pin_);
  ac_->begin();
  delay(200);
  ac_->on();
  ac_->setFan(this->ac_fan_);
  // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
  ac_->setMode(kGreeAuto);
  ac_->setSwingVertical(this->ac_swing_, kGreeSwingAuto);
  ac_->setXFan(this->ac_xfan_);
  ac_->setLight(this->ac_light_);
  ac_->setSleep(this->ac_sleep_);
  ac_->setTurbo(this->ac_turbo_);
  this->target_temperature = this->default_target_temperature_;
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
      // For future use with home assistant.
      case climate::CLIMATE_MODE_FAN_ONLY:
        ac_->setMode(kGreeFan);
        ac_->on();
        break;
      // For future use with home assistant.
      case climate::CLIMATE_MODE_DRY:
        ac_->setMode(kGreeDry);
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