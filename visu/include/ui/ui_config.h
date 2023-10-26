#ifndef CAN_VISUALISER_UI_CONFIG_H
#define CAN_VISUALISER_UI_CONFIG_H

#include "imgui.h"
#include "usb/usb_handler.h"

class UIConfig : private USBHandler {
 public:
  static void display();
  static const char* get_active_can_rate();
  static bool get_frame_type();
};

#endif  // CAN_VISUALISER_UI_CONFIG_H
