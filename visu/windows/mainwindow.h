#ifndef QT_VISU_MAINWINDOW_H
#define QT_VISU_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

#include "modeselector.h"
#include "rxcantable.h"
#include "statusbar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

  static constexpr int DEFAULT_WIDTH  = 1700;
  static constexpr int DEFAULT_HEIGHT = 950;

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 private:
  Ui::MainWindow* ui;
  void centre_on_screen();
};

#endif  // QT_VISU_MAINWINDOW_H
