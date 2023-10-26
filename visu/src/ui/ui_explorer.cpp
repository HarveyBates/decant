#include "ui/ui_explorer.h"

void UIExplorer::display() {
  ImGui::SeparatorText("Explorer");

  ImGui::Columns(2);  // Create two columns
  ImGui::SetColumnWidth(0, 150.0f);

  // Display labels in the first column
  ImGui::Text("COB-ID:");
  ImGui::Text("FC:");
  ImGui::Text("CAN-ID:");
  ImGui::Text("Command Byte:");
  ImGui::Text("CSS:");
  ImGui::Text("Number of Bytes:");
  ImGui::Text("Expedited:");
  ImGui::Text("Size:");
  ImGui::Text("Index:");
  ImGui::Text("Sub-Index:");

  // Display corresponding values in the second column
  ImGui::NextColumn();  // Move to the next column

  ImGui::Columns(1);
}