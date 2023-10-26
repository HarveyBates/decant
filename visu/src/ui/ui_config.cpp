#include "ui/ui_config.h"

static int current_can_rate_idx = 11;
static int standard_frame       = true;

static const char* can_rates[] = {"5",   "10",  "20",  "33",  "40",
                                  "50",  "80",  "95",  "100", "125",
                                  "200", "250", "500", "1000"};

void UIConfig::display() {
  ImGui::BeginChild("Configuration", ImVec2(400, 200), true);

  ImGui::SeparatorText("CAN");

  ImGui::Combo("Speed (kbps)", &current_can_rate_idx, can_rates,
               IM_ARRAYSIZE(can_rates));

  ImGui::RadioButton("Standard", &standard_frame, 1);
  ImGui::SameLine();
  ImGui::RadioButton("Extended", &standard_frame, 0);

  ImGui::Button("Configure");
  ImGui::EndChild();
}

const char* UIConfig::get_active_can_rate() {
  return can_rates[current_can_rate_idx];
}

/// @return True if standard frame
bool UIConfig::get_frame_type() { return standard_frame; }
