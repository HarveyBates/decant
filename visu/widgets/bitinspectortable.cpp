#include "bitinspectortable.h"

#include "ui_BitInspectorTable.h"

BitInspectorTable::BitInspectorTable(QWidget *parent)
    : QWidget(parent), ui(new Ui::BitInspectorTable) {
  ui->setupUi(this);

  // Create rows
  for (int i = 0; i < 7; i++) {
    ui->tableWidget->insertRow(i);
  }

  // Set vertical headers to start from 0
  QStringList headers;
  for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
    headers << QString::number(i);
  }

  // Assign new header labels 0 - 7
  ui->tableWidget->setVerticalHeaderLabels(headers);

  // Add table to vertical layout
  // ui->verticalLayout->addWidget(ui->tableWidget);

  ui->tableWidget->setAlternatingRowColors(true);

  // Set column widths
  for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
    ui->tableWidget->setColumnWidth(i, 35);
  }

  // ui->tableWidget->setFixedHeight(270);
  // ui->tableWidget->setFixedWidth(500);

  // this->setLayout(ui->verticalLayout);

  ui->tableWidget->show();
}

BitInspectorTable::~BitInspectorTable() { delete ui; }
