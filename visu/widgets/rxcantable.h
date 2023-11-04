#ifndef QT_VISU_RXCANTABLE_H
#define QT_VISU_RXCANTABLE_H

#include <QBrush>
#include <QColor>
#include <QWidget>
#include <utility>

#include "can/can_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class RXCANTable;
}
QT_END_NAMESPACE

class RXCANTable : public QWidget {
  Q_OBJECT

  static constexpr int MAX_ROWS = 1000;
  static inline int row_index   = 0;

 public:
  explicit RXCANTable(QWidget* parent = nullptr);
  ~RXCANTable() override;
  void updateTable(CANInterface::CANopenMessage& msg);

 private:
  Ui::RXCANTable* ui;
  void formatTable();
};

#endif  // QT_VISU_RXCANTABLE_H
