#include "mainwindow.h"

#include "ui_mainwindow.h"

StatusBar* statusbar = nullptr;

/**
 * @brief Construct Main Window
 * @param parent Parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // RX table setup
  auto* rx_table = new RXCANTable(this);
  USBHandler::add_table(rx_table);

  // Mode selector
  auto* modeSelector = new ModeSelector(this);

  // Bit table inspector
  auto* bitTable = new BitInspectorTable(this);
  bitTable->setFixedWidth(450);

  // Filtered table
  auto* filteredTable = new RXCANFilteredTable(this);
  USBHandler::add_table(filteredTable);

  ui->horizontalLayout->addWidget(modeSelector);
  ui->horizontalLayout->addWidget(ui->tabWidget);
  ui->horizontalLayout->addWidget(bitTable);

  this->setCentralWidget(ui->horizontalLayoutWidget);
  ui->horizontalLayoutWidget->setContentsMargins(10, 10, 10, 10);

  ui->tabWidget->addTab(rx_table, "Unfiltered");
  ui->tabWidget->addTab(filteredTable, "Filtered");

  // Setup status bar (bottom of screen bar)
  statusbar = new StatusBar(this);
  statusbar->setObjectName("statusbar");
  this->setStatusBar(statusbar);
  statusbar->showConnectionStatus(USBHandler::is_connected());

  this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  centre_on_screen();
  this->show();
}

/**
 * @brief Delete main window.
 */
MainWindow::~MainWindow() { delete ui; }

/**
 * @brief Sets the main window to occupy the centre of the screen on startup.
 */
void MainWindow::centre_on_screen() {
  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  int x                = (screenGeometry.width() - this->width()) / 2;
  int y                = (screenGeometry.height() - this->height()) / 2;
  this->move(x, y);
}
