#ifndef CAN_VISUALISER_CAN_INTERFACE_H
#define CAN_VISUALISER_CAN_INTERFACE_H

#include <chrono>
#include <cstdint>
#include <ctime>
#include <queue>
#include <random>
#include <thread>
#include <vector>

#include "can_definitions.h"
#include "spdlog/spdlog.h"

class CANInterface {
 public:
  struct CANopenMessageRAW {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
  };

  struct CANopenMessage {
    uint16_t id;
    uint8_t error;
    uint32_t cob_id;
    uint16_t function_code;
    uint32_t can_id;
    uint8_t data_length_code;
    uint8_t data[8];
    std::time_t unix_timestamp;
    char local_time[48];
  };

 public:
  static void push(CANopenMessageRAW msg);
  static CANopenMessage format_message(CANopenMessageRAW& raw_msg);
  static std::vector<CANopenMessage>* get_packets();
  static void clear_packets();
};

#endif  // CAN_VISUALISER_CAN_INTERFACE_H
