#ifndef CAN_VISUALISER_CAN_INTERFACE_H
#define CAN_VISUALISER_CAN_INTERFACE_H

#include <chrono>
#include <cstdint>
#include <random>
#include <vector>

class CANInterface {
 public:
  struct CANPacket {
    uint16_t id;
    uint8_t error;
    uint16_t function_code;
    uint32_t can_id;
    uint8_t data_length_code;
    uint8_t standard_frame[8];
    uint8_t extended_frame[64 - 8];
  };

 private:
  static CANPacket get_packet();

 public:
  static void init();
  static std::vector<CANPacket>* get_packets();
};

#endif  // CAN_VISUALISER_CAN_INTERFACE_H
