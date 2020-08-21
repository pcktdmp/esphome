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
  int data_pin_{2};
  IRGreeAC *m_ac{nullptr};

 public:
  void setup() override;
  void set_supports_cool(bool supports_cool) { this->supports_cool_ = supports_cool; }
  void set_supports_heat(bool supports_heat) { this->supports_heat_ = supports_heat; }
  // Follow suggested circuit from:
  // https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/TurnOnGreeAC/TurnOnGreeAC.ino
  void set_data_pin(int data_pin) { this->data_pin_ = data_pin; }
};

}  // namespace gree
}  // namespace esphome