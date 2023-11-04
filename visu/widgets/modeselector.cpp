#include "modeselector.h"

#include "ui_ModeSelector.h"

ModeSelector::ModeSelector(QWidget* parent)
    : QWidget(parent), ui(new Ui::ModeSelector) {
  ui->setupUi(this);

  // Create groupbox
  groupBox = new QGroupBox;
  groupBox->setFixedWidth(140);

  // Create buttons with icons
  receiveButton = createButton("  Receive", ":/resources/icons/inbox.png");
  receiveButton->setChecked(true);
  transmitButton = createButton("  Transmit", ":/resources/icons/outbox.png");
  settingsButton = createButton("  Settings", ":/resources/icons/settings.png");

  // Create groupbox layout
  auto* groupBoxLayout = new QVBoxLayout;
  groupBoxLayout->addWidget(receiveButton);
  groupBoxLayout->addWidget(transmitButton);

  // Settings button at bottom left of screen
  groupBoxLayout->addItem(
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
  groupBoxLayout->addWidget(settingsButton);
  groupBox->setLayout(groupBoxLayout);

  // Add groupbox to window
  auto* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(groupBox);
  this->setLayout(mainLayout);

  this->show();
}

ModeSelector::~ModeSelector() { delete ui; }

QPushButton* ModeSelector::createButton(const QString& label,
                                        const QString& path) {
  QIcon settingsIcon(path);
  QPushButton* button = new QPushButton(label, this->groupBox);
  button->setCheckable(true);
  button->setIcon(settingsIcon);
  button->setStyleSheet(
      "QPushButton { icon-size: 18px; padding: 10px; font-size: 14px; } "
      "QPushButton::text { padding-left: 20px; }");

  return button;
}
