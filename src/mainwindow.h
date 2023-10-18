#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "creditwindow.h"
#include "depositewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_pushButton_calculate_clicked();

  void on_pushButton_graph_clicked();

  void on_pushButton_credit_calc_clicked();

  void on_pushButton_deposite_calc_clicked();

 private:
  Ui::MainWindow *ui;
  CreditWindow *credit_window;
  DepositeWindow *deposite_window;
};
#endif  // MAINWINDOW_H
