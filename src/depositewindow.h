#ifndef DEPOSITEWINDOW_H
#define DEPOSITEWINDOW_H

#include <QDialog>

namespace Ui {
class DepositeWindow;
}

class DepositeWindow : public QDialog {
  Q_OBJECT

 public:
  explicit DepositeWindow(QWidget *parent = nullptr);
  ~DepositeWindow();

 private slots:
  void on_pushButton_arithmetic_calc_clicked();

  void on_pushButton_calculate_clicked();

  void on_checkBox_capitalisation_stateChanged(int arg1);

 private:
  Ui::DepositeWindow *ui;
};

#endif  // DEPOSITEWINDOW_H
