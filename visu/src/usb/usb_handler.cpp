#include "usb/usb_handler.h"

/// CAN-Scope Vendor ID
static constexpr uint16_t VENDOR_ID = 0x0483;

/// CAN-Scope Product ID
static constexpr uint16_t PRODUCT_ID = 0x5740;

/// USB context (initialise with `libusb_init(&ctx)`)
static libusb_context* ctx = nullptr;

/// Active USB device
static USBHandler::USBDevice active_device;

/// Device connected
static std::atomic<bool> device_connected = false;

/// USB interface ID
static constexpr uint8_t INTERFACE_NUM = 1;

/// Handler for thread access to shared resources
std::mutex mtx;

USBHandler::USBStatus USBHandler::connect() {
  // Once connected stop enumeration
  if (device_connected.load()) return USB_OK;

  ssize_t n_devices;
  libusb_device** devices;

  // USB context, on thread kill this is freed
  if (!ctx) {
    if (libusb_init(&ctx) < 0) {
      spdlog::error("USB initialisation failed.");
      return USB_ERROR;
    }
  }

  // List USB devices
  n_devices = libusb_get_device_list(ctx, &devices);
  if (n_devices <= 0) {
    return USB_NO_DEVICES_FOUND;
  }

  spdlog::debug("{} USB devices found.", n_devices);

  for (ssize_t i = 0; i < n_devices; i++) {
    USBDevice usb_device;
    libusb_device_descriptor description{};

    // Get device descriptors (Vendor ID and Product ID)
    if (libusb_get_device_descriptor(devices[i], &description) < 0) {
      // Failed to get description, goto next device
      continue;
    }

    // Check if device matches the Vendor ID and Product ID of CAN-Scope
    if (description.idVendor != VENDOR_ID &&
        description.idProduct != PRODUCT_ID) {
      continue;
    }

    // Attach handler
    usb_device.handler =
        libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
    if (!usb_device.handler) {
      continue;
    }

    // Claim interface with ID
    if (libusb_claim_interface(usb_device.handler, INTERFACE_NUM) !=
        LIBUSB_SUCCESS) {
      libusb_close(usb_device.handler);
      continue;
    }

    std::unique_lock<std::mutex> lock(mtx);
    active_device = usb_device;
    device_connected.store(true);

    spdlog::debug("Device connected.");

    libusb_free_device_list(devices, 1);
    return USB_OK;
  }

  libusb_free_device_list(devices, 1);

  return USB_NOT_CONNECTED;
}

void USBHandler::disconnect(bool bypass_wait) {
  std::unique_lock<std::mutex> lock(mtx);
  if (!device_connected.load()) {
    lock.unlock();
    return;
  }

  if (active_device.handler) {
    libusb_release_interface(active_device.handler, INTERFACE_NUM);
    libusb_close(active_device.handler);
  }

  // This delay is required as the device handler thinks the device is still
  // connected even though the pipe failed (disconnected). The wait can be
  // bypassed when exiting the program.
  if (!bypass_wait) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  } else {
    spdlog::debug("Bypassing USB disconnect sleep.");
  }

  device_connected.store(false);

  lock.unlock();
  spdlog::debug("Device disconnected.");
}

void USBHandler::read(std::atomic<bool>& kill) {
  while (!kill.load()) {
    while (!device_connected.load() && !kill.load()) {
      std::this_thread::yield();
    }

    unsigned char data[64];
    int actual_len;
    int result;

    std::unique_lock<std::mutex> lock(mtx);
    if (!active_device.handler) {
      lock.unlock();
      continue;
    }

    // Process received data (requires timeout so the thread can terminate)
    result = libusb_bulk_transfer(active_device.handler, 0x81, data,
                                  sizeof(data), &actual_len, 1);

    switch (result) {
      case LIBUSB_ERROR_TIMEOUT:
        lock.unlock();
        continue;
      case LIBUSB_SUCCESS:
        break;
      default:
        std::cout << "Decant: Connection Lost." << std::endl;
        lock.unlock();
        disconnect();
        continue;
    }

    if (actual_len != sizeof(CANInterface::CANopenMessageRAW)) {
      lock.unlock();
      continue;
    }

    CANInterface::CANopenMessageRAW raw_msg{};
    memcpy(&raw_msg, data, sizeof(raw_msg));
    CANInterface::push(raw_msg);

    lock.unlock();
  }

  // Disconnect if connected
  if (device_connected.load()) {
    disconnect(true);
  }

  // USB context cleanup
  if (ctx) {
    libusb_exit(ctx);
  }

  spdlog::debug("USB RX thread terminated.");
}

bool USBHandler::is_connected() {
  std::unique_lock<std::mutex> lock(mtx);
  return device_connected;
}
