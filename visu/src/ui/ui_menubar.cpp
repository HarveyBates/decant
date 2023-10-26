#include "ui/ui_menubar.h"

static bool display_settings = false;

void UIMenubar::display() {
  if (!ImGui::BeginMenuBar()) return;

  file();
  setup();
  help();

  if (display_settings) {
    show_settings_window();
  }

  ImGui::EndMenuBar();
}

void UIMenubar::file() {
  if (!ImGui::BeginMenu("File")) return;

  if (ImGui::MenuItem("Open", "CTRL+O")) {
  }
  if (ImGui::MenuItem("Save", "CTRL+S")) {
  }
  if (ImGui::MenuItem("Exit")) {
  }

  ImGui::EndMenu();
}

void UIMenubar::setup() {
  if (!ImGui::BeginMenu("Setup")) return;

  if (ImGui::MenuItem("Settings")) {
    display_settings = true;
  }

  ImGui::EndMenu();
}

void UIMenubar::help() {
  if (!ImGui::BeginMenu("Help")) return;

  if (ImGui::MenuItem("About")) {
  }

  ImGui::EndMenu();
}

void UIMenubar::show_settings_window() {
  // Centre the window
  ImVec2 displaySize = ImGui::GetIO().DisplaySize;
  ImVec2 centerPos   = ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f);
  ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

  // Display the settings
  ImGui::Begin("Config", &display_settings, ImGuiWindowFlags_AlwaysAutoResize);
  UIConfig::display();
  ImGui::End();
}
