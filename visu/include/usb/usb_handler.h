#ifndef CAN_VISUALISER_USB_HANDLER_H
#define CAN_VISUALISER_USB_HANDLER_H

#include <libusb.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "CAN/can_interface.h"
#include "spdlog/spdlog.h"

class USBHandler {
 public:
  struct USBDevice {
    std::string name;
    libusb_device_handle* handler = nullptr;
  };

 protected:
 public:
  enum USBStatus { USB_OK, USB_NO_DEVICES_FOUND, USB_NOT_CONNECTED, USB_ERROR };

  static USBStatus connect();
  static void disconnect(bool bypass_wait = false);
  static void read(std::atomic<bool>& kill);
  static bool is_connected();
};

#endif  // CAN_VISUALISER_USB_HANDLER_H
