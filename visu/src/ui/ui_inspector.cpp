#include "ui/ui_inspector.h"

static const ImGuiTableFlags table_flags =
    ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollX |
    ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable;

static constexpr uint8_t N_STANDARD_COLUMNS = 16;
static constexpr uint8_t N_EXTENDED_COLUMNS = 72;

static uint32_t inspector_row_idx = 0;
static uint32_t watchlist_row_idx = 0;

static bool running     = false;
static bool auto_filter = false;
static bool auto_scroll = false;

static std::vector<CANInterface::CANopenMessage> watchlist_frames;

const char* function_code_to_canopen(uint16_t function_code);

void UIInspector::display(const ImVec2* parent_size) {
  ImGui::SeparatorText("Inspector");
  uint8_t n_columns = N_STANDARD_COLUMNS;

  control_buttons();

  // Check if extended of standard CANopen frame
  if (!UIConfig::get_frame_type()) {
    n_columns = N_EXTENDED_COLUMNS;
  }

  ImGui::BeginTable("InspectorTable", n_columns, table_flags,
                    ImVec2(parent_size->x - 20, parent_size->y / 2.3));

  add_header();
  add_rows(n_columns);

  ImGui::EndTable();

  ImGui::SeparatorText("Watchlist");

  if (ImGui::Button("Watch Frame " ICON_FA_SEARCH)) {
    std::vector<CANInterface::CANopenMessage>* packets =
        CANInterface::get_packets();
    bool duplicate = false;
    for (const auto& packet : *packets) {
      if (packets->at(inspector_row_idx).id == packet.id) {
        duplicate = true;
        break;
      }
    }

    if (!duplicate) {
      watchlist_frames.push_back(packets->at(inspector_row_idx));
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Unwatch Frame " ICON_FA_TRASH)) {
    if (watchlist_row_idx >= 0 && watchlist_row_idx < watchlist_frames.size()) {
      watchlist_frames.erase(watchlist_frames.begin() + watchlist_row_idx);
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Auto-Filter " ICON_FA_FILTER)) {
    auto_filter = !auto_filter;
  }

  if (auto_filter) {
    std::vector<CANInterface::CANopenMessage>* packets =
        CANInterface::get_packets();

    // Automatically filter messages by their COB-ID
    watchlist_frames.clear();

    for (auto packet = packets->rbegin(); packet != packets->rend(); ++packet) {
      bool unique = true;
      for (const auto& watchlist_pkt : watchlist_frames) {
        if (packet->function_code == watchlist_pkt.function_code) {
          unique = false;
          break;
        }
      }
      if (unique) {
        watchlist_frames.push_back(*packet);
      }
    }
  }

  ImGui::BeginTable("WatchedFrames", n_columns, table_flags,
                    ImVec2(parent_size->x - 20, parent_size->y / 2.3));

  add_header();

  uint8_t row = 0;
  for (auto& packet : watchlist_frames) {
    add_row(&packet, n_columns, row++, watchlist_row_idx);
  }

  ImGui::EndTable();
}

void UIInspector::add_header() {
  static const ImGuiTableRowFlags header_flags =
      ImGuiTableColumnFlags_WidthFixed;

  // Setup headings
  ImGui::TableSetupColumn("MSG-ID", header_flags, 50);
  ImGui::TableSetupColumn("Timestamp", header_flags, 170);
  ImGui::TableSetupColumn("COB-ID", header_flags, 80);
  ImGui::TableSetupColumn("FC", header_flags, 50);
  ImGui::TableSetupColumn("FC-ID", header_flags, 80);
  ImGui::TableSetupColumn("CAN-ID", header_flags, 50);
  ImGui::TableSetupColumn("DLC", header_flags, 30);
  // Standard frame columns
  for (uint8_t i = 0; i < 8; i++) {
    char name[6];
    snprintf(name, sizeof(name), "%02X", i);
    ImGui::TableSetupColumn(name, header_flags, 30);
  }

  // Extended frame columns
  if (!UIConfig::get_frame_type()) {
    for (uint8_t i = 8; i < 64; i++) {
      char name[6];
      snprintf(name, sizeof(name), "%02X", i);
      ImGui::TableSetupColumn(name, header_flags, 30);
    }
    ImGui::TableSetupColumn("ASCII", header_flags, 500);
  } else {
    ImGui::TableSetupColumn("ASCII", header_flags, 100);
  }

  ImGui::TableHeadersRow();
}

void UIInspector::add_row(CANInterface::CANopenMessage* packet,
                          uint8_t n_columns, uint32_t row_idx,
                          uint32_t& selected_row_idx) {
  ImGui::TableNextRow();

  ImGui::TableSetColumnIndex(0);
  char label[8];
  snprintf(label, sizeof(label), "%d", packet->id);
  // Add the cell, making the row of the table selectable
  if (ImGui::Selectable(label, false, ImGuiSelectableFlags_SpanAllColumns)) {
    selected_row_idx = row_idx;
  }

  // ImGui::Text("%d", packet.id);
  ImGui::TableSetColumnIndex(1);
  ImGui::Text("%s", packet->local_time);
  ImGui::TableSetColumnIndex(2);
  ImGui::Text("0x%04X", packet->cob_id);
  ImGui::TableSetColumnIndex(3);
  ImGui::Text("0x%02X", packet->function_code);
  ImGui::TableSetColumnIndex(4);
  ImGui::Text("%s", function_code_to_canopen(packet->function_code));
  ImGui::TableSetColumnIndex(5);
  ImGui::Text("0x%02X", packet->can_id);
  ImGui::TableSetColumnIndex(6);
  ImGui::Text("%02X", packet->data_length_code);

  std::string ascii_rep;

  // Show standard frame bytes
  uint8_t idx = 0;
  uint8_t col = 7;
  for (; col < N_STANDARD_COLUMNS - 1; col++) {
    uint8_t b = packet->data[idx++];
    ImGui::TableSetColumnIndex(col);
    populate_cell(b);
    if (b >= 32 && b <= 126) {
      ascii_rep.push_back(static_cast<char>(b));
    } else {
      ascii_rep.push_back('.');
    }
  }

  // If extended show extended frame bytes
  // idx = 0;
  // if (!UIConfig::get_frame_type()) {
  //  for (; col < N_EXTENDED_COLUMNS - 1; col++) {
  //    uint8_t b = packet->standard_frame[idx++];
  //    ImGui::TableSetColumnIndex(col);
  //    populate_cell(b);
  //    if (b >= 32 && b <= 126) {
  //      ascii_rep.push_back(static_cast<char>(b));
  //    } else {
  //      ascii_rep.push_back('.');
  //    }
  //  }
  //}

  // ASCII representation
  ImGui::TableSetColumnIndex(n_columns - 1);
  ImGui::Text("%s", ascii_rep.c_str());
}

void UIInspector::add_rows(uint8_t n_columns) {
  std::vector<CANInterface::CANopenMessage>* packets =
      CANInterface::get_packets();

  uint32_t row = 0;
  for (auto& packet : *packets) {
    if (inspector_row_idx + 1 == row) {
      ImGui::TableSetBgColor(
          ImGuiTableBgTarget_RowBg0,
          ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.5f, 1.0f, 1.0f)));
    }

    add_row(&packet, n_columns, row, inspector_row_idx);
    row++;
  }

  if (auto_scroll) {
    if (row > 0) {
      ImGui::TableSetColumnIndex(0);
      ImGui::SetScrollHereY(1.0f);  // 1.0f will put it at the bottom
    }
  }
}

void UIInspector::populate_cell(uint8_t value) {
  // Set colours from lookup table
  ImVec4 bg_colour  = UIColours::get_hex_color(value);
  ImU32 text_colour = UIColours::get_text_color(bg_colour);
  ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                         ImGui::ColorConvertFloat4ToU32(bg_colour));
  ImGui::PushStyleColor(ImGuiCol_Text, text_colour);

  // Create label for cell
  ImGui::Text("%02X", value);

  // Clear colour constrains
  ImGui::PopStyleColor();
}

void UIInspector::control_buttons() {
  // Green Start Button
  if (!running) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
    if (ImGui::Button("Start " ICON_FA_PLAY)) {
      // Handle the "Green Start" button click
      running = true;
    }
  } else {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Stop " ICON_FA_STOP)) {
      running = false;
      // Handle the "Red Stop" button click
    }
  }
  ImGui::PopStyleColor();

  ImGui::SameLine();

  if (ImGui::Button("Clear " ICON_FA_SYNC)) {
    // Clear graph
    CANInterface::clear_packets();
  }

  ImGui::SameLine();

  if (ImGui::Button("Auto-Scroll " ICON_FA_LEVEL_DOWN_ALT)) {
    // Auto-scroll graph
    auto_scroll = !auto_scroll;
  }
}

const char* function_code_to_canopen(uint16_t function_code) {
  switch (function_code) {
    case CANopenFC::FC_NMT:
      return "NMT";
    case CANopenFC::FC_SYNC:
      return "Sync/Emergency";
    case CANopenFC::FC_TIMESTAMP:
      return "Timestamp";
    case CANopenFC::FC_PDO1_TRANSMIT:
      return "PDO1/TX";
    case CANopenFC::FC_PDO1_RECEIVE:
      return "PDO1/RX";
    case CANopenFC::FC_PDO2_TRANSMIT:
      return "PDO2/TX";
    case CANopenFC::FC_PDO2_RECEIVE:
      return "PDO2/RX";
    case CANopenFC::FC_PDO3_TRANSMIT:
      return "PDO3/TX";
    case CANopenFC::FC_PDO3_RECEIVE:
      return "PDO3/RX";
    case CANopenFC::FC_PDO4_TRANSMIT:
      return "PDO4/TX";
    case CANopenFC::FC_PDO4_RECEIVE:
      return "PDO4/RX";
    case CANopenFC::FC_SDO_TRANSMIT:
      return "SDO/TX";
    case CANopenFC::FC_SDO_RECEIVE:
      return "SDO/RX";
    default:
      break;
  }

  return "Unknown";
}