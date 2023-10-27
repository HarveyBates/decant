#ifndef CAN_VISUALISER_UI_INSPECTOR_H
#define CAN_VISUALISER_UI_INSPECTOR_H

#include <ranges>
#include <vector>

#include "CAN/can_interface.h"
#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "ui_colours.h"
#include "ui_config.h"

class UIInspector {
  static void add_row(CANInterface::CANopenMessage* packet, uint8_t n_columns,
                      uint32_t row_idx, uint32_t& selected_row_idx);
  static void add_rows(uint8_t n_columns);
  static void add_header();
  static void populate_cell(uint8_t value);
  static void control_buttons();

 public:
  static void display(const ImVec2* parent_size);
};

#endif  // CAN_VISUALISER_UI_INSPECTOR_H
