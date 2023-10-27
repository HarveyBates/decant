#ifndef CAN_VISUALISER_UI_H
#define CAN_VISUALISER_UI_H

#include <SDL.h>
#include <SDL_opengl.h>

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "spdlog/spdlog.h"
#include "ui_bottombar.h"
#include "ui_colours.h"
#include "ui_config.h"
#include "ui_explorer.h"
#include "ui_inspector.h"
#include "ui_menubar.h"

#define WINDOW_NAME "Decant-Visualiser"

class UI {
  static const auto window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  static constexpr int WINDOW_WIDTH  = 1920;
  static constexpr int WINDOW_HEIGHT = 1080;
  static constexpr ImVec4 bg_color   = ImVec4(0.40f, 0.44f, 0.52f, 0.3f);

 public:
  enum UIStatus { UI_OK, UI_ERROR, UI_SDL_ERROR };
  static UIStatus init();
  static bool exit();
  static UIStatus update();
  static void destroy();

 private:
  static UIStatus setup_SDL();
  static UIStatus create_window();
};

#endif  // CAN_VISUALISER_UI_H
