#ifndef QT_VISU_STATUSBAR_H
#define QT_VISU_STATUSBAR_H

#include <QLabel>
#include <QProgressBar>
#include <QStatusBar>
#include <QTimer>

#include "usb/usb_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class StatusBar;
}
QT_END_NAMESPACE

class StatusBar : public QStatusBar {
  Q_OBJECT

 public:
  explicit StatusBar(QWidget *parent = nullptr);
  ~StatusBar() override;
  void addVerticalDivider();

 private:
  Ui::StatusBar *ui;
  QLabel *connectionStatusLabel;
  QLabel *messageReceivedLabel;
  QLabel *messageTransmittedLabel;
  QLabel *bitRateLabel;
  QProgressBar *utilizationBar;
  QLabel *utilisationLabel;
  QTimer *timer;

  static QLabel *addLabel(const QString &text, int width = 0);
  static QProgressBar *addProgressBar();
  void updateUtilisation();

 public slots:
  void showConnectionStatus(bool connected);
  void updateRXMessages();
};

#endif  // QT_VISU_STATUSBAR_H
