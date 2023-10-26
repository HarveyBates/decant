#include <thread>

#include "ui/ui.h"

int main() {
  if (UI::init() != UI::UI_OK) {
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
