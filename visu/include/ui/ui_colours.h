#ifndef CAN_VISUALISER_UI_COLOURS_H
#define CAN_VISUALISER_UI_COLOURS_H

#include <array>
#include <iostream>

#include "imgui.h"

class UIColours {
  static ImVec4 HSVtoRGB(float h);

 public:
  static void init();
  static ImVec4 get_hex_color(uint8_t value);
  static ImU32 get_text_color(ImVec4& background_color);
};

#endif  // CAN_VISUALISER_UI_COLOURS_H
