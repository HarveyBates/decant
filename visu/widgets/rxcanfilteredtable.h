#ifndef DECANTVISUALISER_RXCANFILTEREDTABLE_H
#define DECANTVISUALISER_RXCANFILTEREDTABLE_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "can/can_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class RXCANFilteredTable;
}
QT_END_NAMESPACE

class RXCANFilteredTable : public QWidget {
  Q_OBJECT

  static const int MAX_ROWS   = 100;
  static inline int row_index = 0;

 public:
  explicit RXCANFilteredTable(QWidget *parent = nullptr);
  ~RXCANFilteredTable() override;
  void updateTable(CANInterface::CANopenMessage &msg);

 private:
  Ui::RXCANFilteredTable *ui;
  QPushButton *clearButton;
  void formatTable();
  void updateRow(CANInterface::CANopenMessage &msg, int row_idx);
};

#endif  // DECANTVISUALISER_RXCANFILTEREDTABLE_H
