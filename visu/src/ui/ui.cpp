#include "ui/ui.h"

static SDL_Window* window;
static SDL_GLContext gl_context;
static const char* glsl_version = nullptr;

UI::UIStatus UI::init() {
  UIStatus status;

  status = setup_SDL();

  if (status != UI_OK) {
    return status;
  }

  UIColours::init();
  CANInterface::init();  // Debug only

  return create_window();
}

UI::UIStatus UI::setup_SDL() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    return UI_SDL_ERROR;
  }

// GL version
#if defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  glsl_version = "#version 150";
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  return UI_OK;
}

UI::UIStatus UI::create_window() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Create window with graphics context
  window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            window_flags);

  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  // Dark theme
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  if (glsl_version) {
    ImGui_ImplOpenGL3_Init(glsl_version);
  } else {
    return UI_ERROR;
  }

  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontDefault();

  // Default font size
  float baseFontSize = 13.0f;

  // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to
  // align correctly
  float iconFontSize = baseFontSize * 2.0f / 3.0f;

  // Merge icons from Font Awesome
  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
  ImFontConfig icons_config;
  icons_config.MergeMode        = true;
  icons_config.PixelSnapH       = true;
  icons_config.GlyphMinAdvanceX = iconFontSize;

  io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize,
                               &icons_config, icons_ranges);

  return UI_OK;
}

bool UI::exit() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
      return true;
    }

    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE &&
        event.window.windowID == SDL_GetWindowID(window)) {
      return true;
    }
  }

  return false;
}

UI::UIStatus UI::update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();

  ImGui::NewFrame();

  // Full screen window
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowSize(display_size);

  const float display_div_x = display_size.x / 2;
  const float display_div_y = display_size.y / 2;
  const ImVec2 INSPECTOR_SIZE =
      ImVec2(display_div_x - 30, (display_size.y - 60));
  const ImVec2 GRAPH_SIZE    = ImVec2(display_div_x, display_div_y);
  const ImVec2 ANALYSIS_SIZE = ImVec2(display_div_x, display_div_y - 68);

  ImGui::Begin("CAN-Visualiser", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoSavedSettings |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar);

  USBHandler::connect();

  ImGui::BeginChild("Inspector", INSPECTOR_SIZE, true,
                    ImGuiWindowFlags_HorizontalScrollbar);
  UIInspector::display(&INSPECTOR_SIZE);
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Hello", GRAPH_SIZE, true);
  // Generate some example data
  const int dataSize          = 100;
  static float data[dataSize] = {};
  for (int i = 0; i < dataSize; i++) {
    data[i] = 0.5f + 0.5f * sinf(i * 0.1f);  // Example data
  }
  ImGui::PushStyleColor(ImGuiCol_FrameBg,
                        IM_COL32(0, 0, 0, 0));  // Transparent background
  ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(255, 0, 0, 255));
  // In your render loop
  ImGui::PlotLines("Plot", data, IM_ARRAYSIZE(data), 0, nullptr, FLT_MAX,
                   FLT_MAX, ImVec2(GRAPH_SIZE.x - 80, GRAPH_SIZE.y - 50));
  ImGui::PopStyleColor(2);
  ImGui::EndChild();

  ImGui::SetCursorPos(ImVec2(display_div_x - 14, display_div_y + 35));
  ImGui::BeginChild("Analysis", ANALYSIS_SIZE, true);
  UIExplorer::display();
  ImGui::EndChild();

  UIMenubar::display();
  UIBottombar::display();

  ImGui::End();

  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)display_size.x, (int)display_size.y);
  glClearColor(bg_color.x * bg_color.w, bg_color.y * bg_color.w,
               bg_color.z * bg_color.w, bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);

  return UI::UI_OK;
}
void UI::destroy() {
  // ImGUI cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // Internal cleanup

  // SDL cleanup
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
