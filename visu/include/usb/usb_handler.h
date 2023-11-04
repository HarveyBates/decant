#ifndef QT_VISU_USB_HANDLER_H
#define QT_VISU_USB_HANDLER_H

#include <libusb.h>

#include <QThread>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../widgets/rxcanfilteredtable.h"
#include "../widgets/rxcantable.h"
#include "can/can_interface.h"
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
  static void add_table(RXCANTable* _table);
  static void add_table(RXCANFilteredTable* _table);
  static void cleanup();
  static bool is_connected();
};

class USBEmitter : public QObject {
  Q_OBJECT
 public:
  static USBEmitter& instance();

 signals:
  void updateConnectionStatus(bool connected);
  void updateRXMessageCount();

 private:
  USBEmitter() {}
};

class USBHandlerThread : public QThread {
  Q_OBJECT
 public:
  USBHandlerThread() : terminate(false) {}

  void run() override {
    while (!terminate) {
      USBHandler::read(terminate);
    }
    USBHandler::cleanup();
  }

  void stop() { terminate = true; }

 private:
  std::atomic<bool> terminate;
};

#endif  // QT_VISU_USB_HANDLER_H
