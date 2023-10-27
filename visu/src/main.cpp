#include <thread>

#include "spdlog/spdlog.h"
#include "ui/ui.h"

int main() {
  spdlog::set_level(spdlog::level::debug);

  if (UI::init() != UI::UI_OK) {
    spdlog::error("UI initialisation failed.");
    return -1;
  }

  static std::atomic<bool> kill_usb_rx_thread = false;

  std::thread usb_rx_thread(USBHandler::read, std::ref(kill_usb_rx_thread));

  while (!UI::exit()) {
    if (UI::update() != UI::UI_OK) break;
  }

  kill_usb_rx_thread.store(true);
  usb_rx_thread.join();

  UI::destroy();

  return 0;
}
