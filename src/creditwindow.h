#ifndef CREDITWINDOW_H
#define CREDITWINDOW_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class CreditWindow;
}

class CreditWindow : public QDialog {
  Q_OBJECT

 public:
  explicit CreditWindow(QWidget *parent = nullptr);
  ~CreditWindow();

 private slots:
  void on_pushButton_arithmetic_calc_clicked();

  void on_pushButton_calculate_clicked();

 private:
  Ui::CreditWindow *ui;
  QStandardItemModel *model;
};

#endif  // CREDITWINDOW_H
