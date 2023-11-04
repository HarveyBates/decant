#ifndef QT_VISU_MODESELECTOR_H
#define QT_VISU_MODESELECTOR_H

#include <QGroupBox>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ModeSelector;
}
QT_END_NAMESPACE

class ModeSelector : public QWidget {
  Q_OBJECT

 public:
  explicit ModeSelector(QWidget* parent = nullptr);
  ~ModeSelector() override;

 private:
  Ui::ModeSelector* ui;
  QPushButton* receiveButton;
  QPushButton* transmitButton;
  QPushButton* settingsButton;
  QGroupBox* groupBox;

  QPushButton* createButton(const QString& label, const QString& path);
};

#endif  // QT_VISU_MODESELECTOR_H
