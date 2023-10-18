#include "creditwindow.h"

#include "ui_creditwindow.h"

extern "C" double monthly_payment_annuity(double S, double P, int N);
extern "C" double monthly_payment_differentiated(double S, double Sn, double P,
                                                 int N);

CreditWindow::CreditWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreditWindow) {
  ui->setupUi(this);
  ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);

  QRegularExpressionValidator *int_validator =
      new QRegularExpressionValidator(QRegularExpression("[0-9]{0,}"), 0);
  ui->lineEdit_credit_amount->setValidator(int_validator);
  ui->lineEdit_credit_amount->setMaxLength(20);
  ui->lineEdit_months->setValidator(int_validator);
  ui->lineEdit_months->setMaxLength(4);
  ui->lineEdit_interest_rate->setValidator(int_validator);
  ui->lineEdit_interest_rate->setMaxLength(4);
}

CreditWindow::~CreditWindow() { delete ui; }

QString variableFormat(double n) {  // assumes max precision of 2
  int i = rint(n * 100.0);
  if (i % 100)
    return QString::number(n, 'f', i % 10 ? 2 : 1);
  else
    return QString::number(i / 100);
}

void CreditWindow::on_pushButton_calculate_clicked() {
  double amount = ui->lineEdit_credit_amount->text().toDouble();
  int months = ui->lineEdit_months->text().toInt();
  double rate = ui->lineEdit_interest_rate->text().toDouble() / 100 / 12;
  int credit_type = 0;
  if (ui->radioButton_differentiated->isChecked()) credit_type = 1;

  model = new QStandardItemModel(months, 1, this);

  ui->tableView->setModel(model);

  QModelIndex index;
  QString payment_str;

  model->setHeaderData(0, Qt::Horizontal, "Ежемесячные платежи");

  double payment = 0;
  double total_payment = 0;

  if (credit_type == 0) {
    payment = monthly_payment_annuity(amount, rate, months);
    total_payment = payment * months;

    payment_str = variableFormat(payment);
    for (int i = 0; i < months; i++) {
      index = model->index(i, 0);
      model->setData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
      model->setData(index, payment_str);
    }
  } else if (credit_type == 1) {
    payment = monthly_payment_differentiated(amount, amount, rate, months);
    double debt_part = amount / months;
    double loan_balance = amount;

    for (int i = 0; i < months; i++) {
      index = model->index(i, 0);
      model->setData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
      model->setData(index, variableFormat(payment));
      total_payment += payment;
      loan_balance -= debt_part;
      payment =
          monthly_payment_differentiated(amount, loan_balance, rate, months);
    }
  }

  double overpayment = total_payment - amount;

  QString total_payment_str = variableFormat(total_payment);
  ui->label_total_payment->setText(total_payment_str);
  QString overpayment_str = variableFormat(overpayment);
  ui->label_overpayment->setText(overpayment_str);
}

void CreditWindow::on_pushButton_arithmetic_calc_clicked() {
  hide();
  qobject_cast<QWidget *>(parent())->show();
}
