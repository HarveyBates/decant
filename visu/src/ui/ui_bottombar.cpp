#include "ui/ui_bottombar.h"

void UIBottombar::display() {
  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
      ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_MenuBar;

  const ImGuiViewport* viewport = ImGui::GetMainViewport();

  // Set position to the bottom of the viewport
  ImGui::SetNextWindowPos(
      ImVec2(viewport->Pos.x,
             viewport->Pos.y + viewport->Size.y - ImGui::GetFrameHeight()));

  // Extend width to viewport width
  ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, ImGui::GetFrameHeight()));

  const char* can_rate = UIConfig::get_active_can_rate();

  if (ImGui::Begin("StatusBar", nullptr, flags)) {
    if (ImGui::BeginMenuBar()) {
      ImGui::Text("Device:");
      if (USBHandler::is_connected()) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");
      } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");
      }

      ImGui::Text("|");

      if (can_rate) {
        ImGui::Text("CAN Speed: %s kbps", can_rate);
      } else {
        ImGui::Text("CAN Speed: <Not Selected>");
      }

      ImGui::EndMenuBar();
    }
    ImGui::End();
  }
}