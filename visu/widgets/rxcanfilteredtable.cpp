#include "rxcanfilteredtable.h"

#include "ui_RXCANFilteredTable.h"

RXCANFilteredTable::RXCANFilteredTable(QWidget* parent)
    : QWidget(parent), ui(new Ui::RXCANFilteredTable) {
  ui->setupUi(this);
  auto* mainLayout = new QVBoxLayout(this);

  formatTable();

  ui->tableWidget->setRowCount(MAX_ROWS);

  mainLayout->addWidget(ui->tableWidget);

  auto* buttonLayout = new QHBoxLayout;
  clearButton        = new QPushButton("Clear");
  buttonLayout->addWidget(clearButton);
  auto* buttonSpacer =
      new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  buttonLayout->addSpacerItem(buttonSpacer);
  mainLayout->addLayout(buttonLayout);

  this->setLayout(mainLayout);

  ui->tableWidget->show();
}

RXCANFilteredTable::~RXCANFilteredTable() { delete ui; }

void RXCANFilteredTable::formatTable() {
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

void RXCANFilteredTable::updateTable(CANInterface::CANopenMessage& msg) {
  if (row_index > MAX_ROWS) {
    return;
  }

  int row_idx = 0;
  for (const auto& row : filteredRows) {
    if (msg.cob_id == row.cob_id) {
      updateRow(msg, row_idx);
      return;
    }
    row_idx++;
  }

  // COB-ID not found, create a new row with this current message
  updateRow(msg, row_index);                          // Create row
  row_index++;                                        // Update number of rows
  ui->tableWidget->sortItems(1, Qt::AscendingOrder);  // Sort table

  // Sort filtered rows in ascending order
  int arraySize = sizeof(filteredRows) / sizeof(filteredRows[0]);
  std::sort(filteredRows, filteredRows + arraySize,
            [](const CANInterface::CANopenMessage& a,
               const CANInterface::CANopenMessage& b) {
              return a.cob_id < b.cob_id;
            });
}

void RXCANFilteredTable::updateRow(CANInterface::CANopenMessage& msg,
                                   int row_idx) {
  // Message received timestamp
  ui->tableWidget->setItem(row_idx, 0, new QTableWidgetItem(msg.timestamp));

  // COB-ID
  ui->tableWidget->setItem(
      row_idx, 1,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.cob_id, 4, 16, QChar('0')).toUpper()));

  // CANopen function code
  ui->tableWidget->setItem(
      row_idx, 2,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.fc, 4, 16, QChar('0')).toUpper()));

  // CANopen plain text function code
  ui->tableWidget->setItem(row_idx, 3, new QTableWidgetItem(msg.canopen_fc));

  // CAN-ID
  ui->tableWidget->setItem(
      row_idx, 4,
      new QTableWidgetItem(
          "0x" + QString("%1").arg(msg.can_id, 4, 16, QChar('0')).toUpper()));

  // Data length code
  ui->tableWidget->setItem(
      row_idx, 5,
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
    ui->tableWidget->setItem(row_idx, column_idx++, cell);
  }

  // ASCII representation
  ui->tableWidget->setItem(row_idx, column_idx,
                           new QTableWidgetItem(msg.ascii));
}
