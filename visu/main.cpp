#include <QApplication>

#include "usb/usb_handler.h"
#include "windows/mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  spdlog::set_level(spdlog::level::debug);

  // Reduce font size
  QFont defaultFont = QApplication::font();
  defaultFont.setPointSize(11);
  QApplication::setFont(defaultFont);

  // Setup USB RX thread
  USBHandlerThread usb_rx_thread;
  usb_rx_thread.start();

  // Show main window
  MainWindow w;
  w.show();

  // Start UI
  int res = QApplication::exec();

  // Cleanup threads
  usb_rx_thread.stop();
  usb_rx_thread.wait();

  return res;
}
