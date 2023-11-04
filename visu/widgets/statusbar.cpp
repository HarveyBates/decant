#include "statusbar.h"

#include "ui_statusbar.h"

static const QString connected_text    = "Device: Online";
static const QString disconnected_text = "Device: Offline";

static uint32_t n_rx_messages      = 0;
static int n_rx_messages_per_timer = 0;

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent), ui(new Ui::StatusBar) {
  ui->setupUi(this);

  connectionStatusLabel   = addLabel(disconnected_text);
  messageReceivedLabel    = addLabel("RX: 0", 100);
  messageTransmittedLabel = addLabel("TX: 0", 100);
  bitRateLabel            = addLabel("Bit Rate: 0 kbps");
  auto barLabel           = addLabel("Bus Utilisation: ");
  utilizationBar          = addProgressBar();
  utilisationLabel        = addLabel("0 %", 50);

  // addWidget(connectionStatusLabel);
  addWidget(connectionStatusLabel);
  addWidget(messageReceivedLabel);
  addWidget(messageTransmittedLabel);
  addWidget(bitRateLabel);
  addPermanentWidget(barLabel);
  addPermanentWidget(utilizationBar);
  addPermanentWidget(utilisationLabel);

  // Timer to calculate CAN bus utilisation
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &StatusBar::updateUtilisation);
  timer->start(100);

  // Update status bar device connected display
  connect(&USBEmitter::instance(), SIGNAL(updateConnectionStatus(bool)), this,
          SLOT(showConnectionStatus(bool)));

  // Update status bar RX messages display
  connect(&USBEmitter::instance(), SIGNAL(updateRXMessageCount()), this,
          SLOT(updateRXMessages()));
}

StatusBar::~StatusBar() { delete ui; }

void StatusBar::addVerticalDivider() {
  auto* divider = new QFrame();
  divider->setFrameShape(QFrame::VLine);
  divider->setStyleSheet("background-color: lightgray;");
  addWidget(divider);
}

QLabel* StatusBar::addLabel(const QString& text, const int width) {
  auto* label = new QLabel(text);
  label->setStyleSheet("margin-right: 5px; margin-left: 5px;");
  if (width != 0) {
    label->setFixedWidth(width);
  }
  return label;
}

QProgressBar* StatusBar::addProgressBar() {
  auto* progressBar = new QProgressBar();
  progressBar->setStyleSheet("margin-right: 5px; margin-left: 5px;");
  progressBar->setFixedWidth(200);
  progressBar->setRange(0, 100);
  progressBar->setTextVisible(false);
  return progressBar;
}

void StatusBar::showConnectionStatus(bool connected) {
  spdlog::debug("Update statusbar connection status.");
  connectionStatusLabel->setText(connected ? connected_text
                                           : disconnected_text);
}

void StatusBar::updateRXMessages() {
  n_rx_messages++;
  n_rx_messages_per_timer++;
  messageReceivedLabel->setText(QString("RX: %1").arg(n_rx_messages));
}

void StatusBar::updateUtilisation() {
  // Number of bits in a message (estimate)
  const int BITS_PER_MESSAGE = 106;
  // Total number of bits received in interval
  double total_bits          = (BITS_PER_MESSAGE * n_rx_messages_per_timer);
  // Utilisation as a function of total possible utilisation (CAN bit-rate)
  double utilization         = round((total_bits / 50000) * 100);

  // Constrain to bar bounds
  if (utilization > 100) utilization = 100;
  if (utilization < 0) utilization = 0;

  spdlog::trace("Utilisation: {}", utilization);

  utilizationBar->setValue(static_cast<uint8_t>(utilization));
  utilisationLabel->setText(QString("%1 %").arg(utilization));
  n_rx_messages_per_timer = 0;  // Reset messages per timer counter
}