#include "CAN/can_interface.h"

static std::mutex mtx;
static std::vector<CANInterface::CANopenMessage> packets;

void CANInterface::push(CANInterface::CANopenMessageRAW raw_msg) {
  std::lock_guard<std::mutex> lock(mtx);
  CANopenMessage msg = format_message(raw_msg);
  packets.emplace_back(msg);
}

CANInterface::CANopenMessage CANInterface::format_message(
    CANInterface::CANopenMessageRAW& raw_msg) {
  CANopenMessage msg{};

  // Get the CAN-ID, either Extended or Standard
  switch (raw_msg.header.IDE) {
    case CAN_ID_STD:
      msg.cob_id        = raw_msg.header.StdId;
      msg.can_id        = (raw_msg.header.StdId & 0x7F);   // CAN-ID (Std frame)
      msg.function_code = (raw_msg.header.StdId & 0x780);  // FC
      break;
    case CAN_ID_EXT:
      msg.cob_id = raw_msg.header.ExtId;
      // TODO: Implement extended frame
      // msg.can_id = (raw_msg.header.ExtId & 0xFF00) >> 8;   // CAN-ID (Ext
      // frame) msg.function_code = (raw_msg.header.ExtId & 0x780);  // FC
      break;
    default:
      // Something has gone horribly wrong
      break;
  }

  // Get data length code
  msg.data_length_code = raw_msg.header.DLC;

  // Get actual message data
  memcpy(msg.data, raw_msg.data, sizeof(msg.data));

  // Calculate message received time (this feels super hacky)
  auto sys_time     = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          sys_time.time_since_epoch()) %
                      1000;

  // Calculate local time
  msg.unix_timestamp   = std::time(nullptr);
  struct tm* localtime = std::localtime(&msg.unix_timestamp);
  std::strftime(msg.local_time, sizeof(msg.local_time), "%Y/%m/%d %H:%M:%S",
                localtime);

  // Add milliseconds to local time
  snprintf(&msg.local_time[19], sizeof(msg.local_time) - 19, ".%lld",
           milliseconds.count());

  spdlog::trace("Message RX");

  return msg;
}

std::vector<CANInterface::CANopenMessage>* CANInterface::get_packets() {
  std::lock_guard<std::mutex> lock(mtx);
  return &packets;
};

void CANInterface::clear_packets() {
  std::lock_guard<std::mutex> lock(mtx);
  spdlog::debug("Cleared packets.");
  packets.clear();
}
