#include "rxcantable.h"

#include <QVBoxLayout>

#include "ui_RXCANTable.h"

RXCANTable::RXCANTable(QWidget* parent)
    : QWidget(parent), ui(new Ui::RXCANTable) {
  ui->setupUi(this);

  // VBox to contain the table (vbox is parent)
  auto* mainLayout = new QVBoxLayout(this);

  // General formatting of table
  formatTable();
  ui->tableWidget->setRowCount(MAX_ROWS);

  // Adding the tableWidget to the mainLayout
  mainLayout->addWidget(ui->tableWidget);

  // Setting the mainLayout as the layout for RXCANTable
  this->setLayout(mainLayout);

  ui->tableWidget->show();
}

RXCANTable::~RXCANTable() { delete ui; }

void RXCANTable::formatTable() {
  // Scale to fit parent
  ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Expanding);

  // Set row vertical height (reduce padding)
  ui->tableWidget->verticalHeader()->setDefaultSectionSize(15);

  // Show header
  ui->tableWidget->horizontalHeader()->setVisible(true);

  // Set width of particular columns
  ui->tableWidget->setColumnWidth(0, 150);  // Timestamp
  ui->tableWidget->setColumnWidth(5, 50);   // Data length code (DLC)
  for (uint8_t i = 6; i < 14; i++) {
    ui->tableWidget->setColumnWidth(i, 30);  // Bytes 0 to 7
  }
  ui->tableWidget->setColumnWidth(14, 60);  // ASCII
}

void RXCANTable::updateTable(CANInterface::CANopenMessage& msg) {
  // Round-robin display items (overwrite after reaching the bottom)
  if (row_index > MAX_ROWS) {
    row_index = 0;
  }

  // Message received timestamp
  ui->tableWidget->setItem(row_index, 0, new QTableWidgetItem(msg.timestamp));

  // COB-ID
  ui->tableWidget->setItem(
      row_index, 1,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.cob_id, 4, 16, QChar('0')).toUpper()));

  // CANopen function code
  ui->tableWidget->setItem(
      row_index, 2,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.fc, 4, 16, QChar('0')).toUpper()));

  // CANopen plain text function code
  ui->tableWidget->setItem(row_index, 3, new QTableWidgetItem(msg.canopen_fc));

  // CAN-ID
  ui->tableWidget->setItem(
      row_index, 4,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.can_id, 4, 16, QChar('0')).toUpper()));

  // Data length code
  ui->tableWidget->setItem(
      row_index, 5,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.dlc, 2, 16, QChar('0')).toUpper()));

  // Bytes 0 to 7
  int column_idx = 6;
  for (unsigned char b : msg.data) {
    auto cell =
        new QTableWidgetItem(QString("%1").arg(b, 2, 16, QChar('0')).toUpper());
    if (b == 0x00) {
      QColor color(Qt::lightGray);
      cell->setForeground(color);
    }
    ui->tableWidget->setItem(row_index, column_idx++, cell);
  }

  // ASCII representation
  ui->tableWidget->setItem(row_index, column_idx,
                           new QTableWidgetItem(msg.ascii));

  row_index++;
}