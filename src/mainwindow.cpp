#include "mainwindow.h"

#include <QDoubleValidator>

#include "ui_mainwindow.h"

extern "C" char* to_postfix(char* infix_str);
extern "C" double calc_postfix(char* postfix_str);
extern "C" char* replace_x(char* str, double x);
extern "C" int check_input(char* input_str);
extern "C" char* replace_mod(char* input_str);
extern "C" int string_contains_x(char* input_str);

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QRegularExpressionValidator* double_validator =
      new QRegularExpressionValidator(
          QRegularExpression("[-]{0,1}[0-9]{0,}[.]{0,1}[0-9]{0,}"), 0);
  ui->lineEdit_x->setValidator(double_validator);
  ui->lineEdit_scope_from->setValidator(double_validator);
  ui->lineEdit_scope_to->setValidator(double_validator);
  ui->lineEdit_range_from->setValidator(double_validator);
  ui->lineEdit_range_to->setValidator(double_validator);

  QRegularExpressionValidator* input_validator =
      new QRegularExpressionValidator(
          QRegularExpression("^([-]{0,1}[0-9]{0,}[.]{0,1}[0-9]{0,}|[+-/"
                             "*^]|[(]|[)]|cos[(]|sin[(]|tan[(]|acos[(]|asin[(]|"
                             "atan[(]|sqrt[(]|ln[(]|log[(]|mod|[xX])*$"));
  ui->lineEdit_input->setValidator(input_validator);

  ui->label_result->setTextInteractionFlags(Qt::TextSelectableByMouse);

  credit_window = new CreditWindow(this);
  deposite_window = new DepositeWindow(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_calculate_clicked() {
  QString qs_input = ui->lineEdit_input->text();
  QByteArray qba = qs_input.toLocal8Bit();
  char* fixed_input = replace_mod(qba.data());
  int error = check_input(fixed_input);

  if (error) {
    QMessageBox::warning(this, "Ошибка",
                         "Поле ввода выражения содержит ошибку");
  } else if (string_contains_x(fixed_input) && ui->lineEdit_x->text() == "") {
    QMessageBox::warning(this, "Ошибка", "Введите x");
  } else {
    char* postfix_input;

    if (string_contains_x(fixed_input)) {
      // если есть что-то в строке ввода х,
      // то сначала заменяем в строке ch_input все х на заданное значение,
      // а уже потом отправляем её в to_postfix
      double x = ui->lineEdit_x->text().toDouble();
      char* input_with_replaced_x = replace_x(fixed_input, x);
      postfix_input = to_postfix(input_with_replaced_x);

      free(input_with_replaced_x);
    } else {
      postfix_input = to_postfix(fixed_input);
    }

    printf("postfix_input = %s\n", postfix_input);

    double res = calc_postfix(postfix_input);
    QString res_str = QString::number(res, 'g', 15);
    ui->label_result->setText(res_str);

    free(postfix_input);
  }

  free(fixed_input);
}

void MainWindow::on_pushButton_graph_clicked() {
  QString qs_input = ui->lineEdit_input->text();
  QByteArray qba = qs_input.toLocal8Bit();
  char* not_fixed_input = qba.data();
  char* fixed_input = replace_mod(not_fixed_input);
  int error = check_input(fixed_input);

  double scope_from, scope_to;

  int scope_error = 0;

  if (ui->lineEdit_scope_from->text() != "" &&
      ui->lineEdit_scope_to->text() != "") {
    scope_from = ui->lineEdit_scope_from->text().toDouble();
    scope_to = ui->lineEdit_scope_to->text().toDouble();

    if ((scope_from >= scope_to) ||
        (scope_from < -1000000 || scope_from > 1000000 || scope_to < -1000000 ||
         scope_to > 1000000))
      scope_error = 1;

  } else {
    // Значения области определения по умолчанию
    scope_from = -3.50;
    scope_to = 3.50;
  }

  if (error) {
    QMessageBox::warning(this, "Ошибка",
                         "Поле ввода выражения содержит ошибку");
  } else if (scope_error) {
    QMessageBox::warning(this, "Ошибка",
                         "Поле ввода области определения содержит ошибку");
  } else {
    char* postfix_input;
    char* input_with_replaced_x;

    double h = 0.01;  // Шаг, с которым будем пробегать по оси Ox
    int N = (scope_to - scope_from) / h +
            1;  // Вычисляем количество точек, которые будем отрисовывать
    QVector<double> x(N), y(N);  // Массивы координат точек

    // Пробегаем по всем точкам
    double X = scope_from;
    for (int i = 0; i < N; i++, X += h) {
      x[i] = X;
      input_with_replaced_x = replace_x(fixed_input, X);
      postfix_input = to_postfix(input_with_replaced_x);
      y[i] = calc_postfix(postfix_input);
    }

    free(input_with_replaced_x);
    free(postfix_input);

    ui->widget_graph->clearGraphs();  // Если нужно, но очищаем все графики

    // Добавляем один график в widget
    ui->widget_graph->addGraph();

    // Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->widget_graph->graph(0)->setData(x, y);

    // Установим область, которая будет показываться на графике
    ui->widget_graph->xAxis->setRange(scope_from, scope_to);  // Для оси Ox

    double range_from = y[0], range_to = y[0];
    int range_error = 0;
    if (ui->lineEdit_range_from->text() == "" ||
        ui->lineEdit_range_to->text() == "") {
      // Для показа границ по оси Oy, если они не указаны, нужно вычислить
      // минимальное и максимальное значение в векторах
      for (int i = 1; i < N; i++) {
        if (y[i] < range_from)
          range_from = y[i];
        else if (y[i] > range_to)
          range_to = y[i];
      }
    } else {
      range_from = ui->lineEdit_range_from->text().toDouble();
      range_to = ui->lineEdit_range_to->text().toDouble();

      if ((range_from >= range_to) ||
          (range_from < -1000000 || range_from > 1000000 ||
           range_to < -1000000 || range_to > 1000000))
        range_error = 1;
    }

    if (range_error) {
      QMessageBox::warning(this, "Ошибка",
                           "Поле ввода области значений содержит ошибку");
    } else {
      ui->widget_graph->yAxis->setRange(range_from, range_to);  // Для оси Oy
      // Перерисуем график на нашем widget
      ui->widget_graph->replot();
    }
  }

  free(fixed_input);
}

void MainWindow::on_pushButton_credit_calc_clicked() {
  hide();
  credit_window->show();
}

void MainWindow::on_pushButton_deposite_calc_clicked() {
  hide();
  deposite_window->show();
}
