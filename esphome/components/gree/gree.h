#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>

namespace esphome {
namespace gree {

class GreeClimate : public climate::Climate, public Component {
 protected:
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;
  bool supports_cool_{true};
  bool supports_heat_{true};
  float visual_min_temperature_{16.0f};
  float visual_max_temperature_{30.0f};
  float visual_temperature_step_{1.0f};
  float default_target_temperature_{20.0f};
  int ac_fan_{0};
  bool ac_swing_{true};
  bool ac_xfan_{false};
  bool ac_light_{false};
  bool ac_sleep_{false};
  bool ac_turbo_{false};
  int ir_pin_{4};
  IRGreeAC *ac_{nullptr};

 public:
  void setup() override;
  void set_supports_cool(bool supports_cool) { this->supports_cool_ = supports_cool; }
  void set_supports_heat(bool supports_heat) { this->supports_heat_ = supports_heat; }
  void set_visual_min_temperature(float visual_min_temperature) { this->visual_min_temperature_ = visual_min_temperature; }
  void set_visual_max_temperature(float visual_max_temperature) { this->visual_max_temperature_ = visual_max_temperature; }
  void set_visual_temperature_step(float visual_temperature_step) { this->visual_temperature_step_ = visual_temperature_step; }
  void set_default_target_temperature(float default_target_temperature) { this->default_target_temperature_ = default_target_temperature; }
  void set_ac_fan(int ac_fan) { this->ac_fan_ = ac_fan; }
  void set_ac_swing(bool ac_swing) { this->ac_swing_ = ac_swing; }
  void set_ac_xfan(bool ac_xfan) { this->ac_xfan_ = ac_xfan; }
  void set_ac_light(bool ac_light) { this->ac_light_ = ac_light; }
  void set_ac_sleep(bool ac_sleep) { this->ac_sleep_ = ac_sleep; }
  void set_ac_turbo(bool ac_turbo) { this->ac_turbo_ = ac_turbo; }

  // Follow suggested circuit from:
  // https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/TurnOnGreeAC/TurnOnGreeAC.ino
  void set_ir_pin(int ir_pin) { this->ir_pin_ = ir_pin; }
};

}  // namespace gree
}  // namespace esphome