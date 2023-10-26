#include "CAN/can_interface.h"

static std::vector<CANInterface::CANPacket> packets;

static const auto interval_ms(500);
static std::chrono::time_point<std::chrono::high_resolution_clock> last_exe;

void CANInterface::init() {
  for (uint8_t i = 0; i < 20; i++) {
    CANInterface::CANPacket packet = get_packet();
    packet.id                      = i;
    packets.push_back(packet);
  }
}

CANInterface::CANPacket CANInterface::get_packet() {
  CANPacket packet{};

  // Create a random engine
  std::default_random_engine engine(std::random_device{}());

  // Create a uniform distribution for uint8_t
  std::uniform_int_distribution<uint8_t> u8_dist(0, UINT8_MAX);
  std::uniform_int_distribution<uint16_t> u16_dist(0x700, 0x710);
  std::uniform_int_distribution<uint32_t> u32_dist(0, UINT32_MAX);

  packet.error            = u8_dist(engine);
  packet.function_code    = u16_dist(engine);
  packet.can_id           = u32_dist(engine);
  packet.data_length_code = u8_dist(engine);

  for (auto& std_data : packet.standard_frame) {
    std_data = u8_dist(engine);
  }

  for (auto& ext_data : packet.extended_frame) {
    ext_data = u8_dist(engine);
  }

  return packet;
}

std::vector<CANInterface::CANPacket>* CANInterface::get_packets() {
  auto now = std::chrono::high_resolution_clock::now();
  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - last_exe)
          .count();

  if (elapsed_ms >= interval_ms) {
    CANInterface::CANPacket packet = get_packet();
    packets.push_back(packet);
    last_exe = now;
  }
  return &packets;
};
