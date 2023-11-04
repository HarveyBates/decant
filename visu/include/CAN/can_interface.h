#ifndef QT_VISU_CAN_INTERFACE_H
#define QT_VISU_CAN_INTERFACE_H

#include <QQueue>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <queue>
#include <random>
#include <thread>

#include "can_definitions.h"
#include "spdlog/spdlog.h"

class CANInterface {
 public:
  struct CANopenMessageRAW {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
  };

  struct CANopenMessage {
    char timestamp[48];
    uint16_t cob_id;
    uint16_t fc;
    const char* canopen_fc;
    uint32_t can_id;
    uint8_t dlc;
    uint8_t data[8];
    char ascii[8];
  };

  static const char* function_code_to_canopen(uint16_t function_code);

 public:
  static CANopenMessage format_message(CANopenMessageRAW& raw_msg);
};

#endif  // QT_VISU_CAN_INTERFACE_H
