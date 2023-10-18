#include "depositewindow.h"

#include "ui_depositewindow.h"

extern "C" double simple_interest(int P, double I, int T);
extern "C" double daily_capitalisation(int P, double N, int T);
extern "C" double monthly_capitalisation(int P, double N, int T);
extern "C" double quarterly_capitalisation(int P, double N, int T);

DepositeWindow::DepositeWindow(QWidget* parent)
    : QDialog(parent), ui(new Ui::DepositeWindow) {
  ui->setupUi(this);
  ui->comboBox_periodicity->hide();
  ui->label_5->hide();

  QRegularExpressionValidator* int_validator =
      new QRegularExpressionValidator(QRegularExpression("[0-9]{0,}"), 0);
  ui->lineEdit_deposite_amount->setValidator(int_validator);
  ui->lineEdit_deposite_amount->setMaxLength(20);
  ui->lineEdit_placement_period->setValidator(int_validator);
  ui->lineEdit_placement_period->setMaxLength(5);

  QRegularExpressionValidator* double_validator =
      new QRegularExpressionValidator(
          QRegularExpression("[-]{0,1}[0-9]{0,}[.]{0,1}[0-9]{0,}"), 0);
  ui->lineEdit_interest_rate->setValidator(double_validator);
  ui->lineEdit_interest_rate->setMaxLength(10);
  ui->lineEdit_tax_rate->setValidator(double_validator);
  ui->lineEdit_tax_rate->setMaxLength(10);
}

DepositeWindow::~DepositeWindow() { delete ui; }

void DepositeWindow::on_pushButton_arithmetic_calc_clicked() {
  hide();
  qobject_cast<QWidget*>(parent())->show();
}

QString formatString(double n) {  // assumes max precision of 2
  int i = rint(n * 100.0);
  if (i % 100)
    return QString::number(n, 'f', i % 10 ? 2 : 1);
  else
    return QString::number(i / 100);
}

void DepositeWindow::on_pushButton_calculate_clicked() {
  int deposite_amount = ui->lineEdit_deposite_amount->text().toInt();
  int placement_period = ui->lineEdit_placement_period->text().toInt();
  double interest_rate = ui->lineEdit_interest_rate->text().toDouble();
  double tax_rate = ui->lineEdit_tax_rate->text().toDouble();

  double profit = 0, taxes = 0, end_amount = 0;
  QString taxes_str, profit_str, end_amount_str;

  if (!ui->checkBox_capitalisation->isChecked()) {
    profit = simple_interest(deposite_amount, interest_rate, placement_period);
    taxes = profit * (tax_rate / 100 / 365 * placement_period);
    profit -= taxes;
    end_amount = deposite_amount + profit;
  } else {
    if (ui->comboBox_periodicity->currentIndex() == 0)
      end_amount = daily_capitalisation(deposite_amount, interest_rate,
                                        placement_period);
    else if (ui->comboBox_periodicity->currentIndex() == 1)
      end_amount = monthly_capitalisation(deposite_amount, interest_rate,
                                          placement_period);
    else if (ui->comboBox_periodicity->currentIndex() == 2)
      end_amount = quarterly_capitalisation(deposite_amount, interest_rate,
                                            placement_period);

    profit = end_amount - deposite_amount;
    taxes = profit * (tax_rate / 100 / 365 * placement_period);
    profit -= taxes;
    end_amount -= taxes;
  }

  taxes_str = formatString(taxes);
  ui->label_tax_amount->setText(taxes_str);
  profit_str = formatString(profit);
  ui->label_interest_charges->setText(profit_str);
  end_amount_str = formatString(end_amount);
  ui->label_end_amount->setText(end_amount_str);
}

void DepositeWindow::on_checkBox_capitalisation_stateChanged(int state) {
  if (state == 2) {
    ui->comboBox_periodicity->show();
    ui->label_5->show();
  } else if (state == 0) {
    ui->comboBox_periodicity->hide();
    ui->label_5->hide();
  }
}
