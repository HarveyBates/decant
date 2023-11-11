#ifndef QT_VISU_BITINSPECTORTABLE_H
#define QT_VISU_BITINSPECTORTABLE_H

#include <QCheckBox>
#include <QGroupBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class BitInspectorTable;
}
QT_END_NAMESPACE

class BitInspectorTable : public QWidget {
  Q_OBJECT

 public:
  explicit BitInspectorTable(QWidget* parent = nullptr);
  ~BitInspectorTable() override;

 private:
  Ui::BitInspectorTable* ui;
};

#endif  // QT_VISU_BITINSPECTORTABLE_H
