#ifndef CAN_VISUALISER_UI_MENUBAR_H
#define CAN_VISUALISER_UI_MENUBAR_H

#include "imgui.h"
#include "ui_config.h"

class UIMenubar {
  static void file();
  static void setup();
  static void help();

  static void show_settings_window();

 public:
  static void display();
};

#endif  // CAN_VISUALISER_UI_MENUBAR_H
