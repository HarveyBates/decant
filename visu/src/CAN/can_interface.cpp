#include "can/can_interface.h"

static std::mutex mtx;

CANInterface::CANopenMessage CANInterface::format_message(
    CANInterface::CANopenMessageRAW& raw_msg) {
  CANopenMessage msg{};

  // Get the CAN-ID, either Extended or Standard
  switch (raw_msg.header.IDE) {
    case CAN_ID_STD:
      msg.cob_id     = raw_msg.header.StdId;
      msg.can_id     = (raw_msg.header.StdId & 0x7F);   // CAN-ID (Std frame)
      msg.fc         = (raw_msg.header.StdId & 0x780);  // FC
      msg.canopen_fc = function_code_to_canopen(msg.fc);
      break;
    case CAN_ID_EXT:
      msg.cob_id = raw_msg.header.ExtId;
      msg.can_id = (msg.cob_id & 0x7F);
      msg.fc     = (msg.cob_id & 0xFF00) >> 8;
      // TODO: Implement extended frame
      // msg.can_id = (raw_msg.header.ExtId & 0xFF00) >> 8;   // CAN-ID (Ext
      // frame) msg.function_code = (raw_msg.header.ExtId & 0x780);  // FC
      break;
    default:
      // Something has gone horribly wrong
      break;
  }

  // Get data length code
  msg.dlc = raw_msg.header.DLC;

  // Get actual message data
  memcpy(msg.data, raw_msg.data, sizeof(msg.data));

  // Get data as its ascii representation
  for (uint8_t i = 0; i < (uint8_t)sizeof(msg.data); i++) {
    if (msg.data[i] >= 32 && msg.data[i] <= 126) {
      msg.ascii[i] = static_cast<char>(msg.data[i]);
    } else {
      msg.ascii[i] = '.';
    }
  }

  // Calculate message received time (this feels super hacky)
  auto sys_time     = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          sys_time.time_since_epoch()) %
                      1000;

  // Calculate local time
  auto unix_timestamp  = std::time(nullptr);
  struct tm* localtime = std::localtime(&unix_timestamp);
  std::strftime(msg.timestamp, sizeof(msg.timestamp), "%Y/%m/%d %H:%M:%S",
                localtime);

  // Add milliseconds to local time
  snprintf(&msg.timestamp[19], sizeof(msg.timestamp) - 19, ".%lld",
           milliseconds.count());

  spdlog::trace("Message RX");

  return msg;
}

const char* CANInterface::function_code_to_canopen(uint16_t function_code) {
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