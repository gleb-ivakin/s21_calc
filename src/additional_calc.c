#include "additional_calc.h"

double monthly_payment_annuity(double S, double P, int N) {
  // Здесь S — сумма займа, P — 1/100 доля процентной ставки (в месяц), N — срок
  // кредитования (в месяцах).
  return S * (P + (P / (pow(1 + P, (double)N) - 1)));
}

double monthly_payment_differentiated(double S, double Sn, double P, int N) {
  // Здесь Sn — остаток тела кредита.
  return (S / N) + (Sn * P);
}

double simple_interest(int P, double I, int T) {
  // P — первоначальная сумма вложений, I — годовая ставка, T — количество дней
  // вклада, Tax - налоговая ставка
  return (P * I * T / 365) / 100;
}

double daily_capitalisation(int P, double N, int T) {
  // Р — первоначальный размер депозита, N — годовая процентная ставка, не
  // разделенная на 100, Т — срок вложения в днях.
  return P * pow(1 + N / 100 / 365, (double)T);
}

double monthly_capitalisation(int P, double N, int T) {
  return P * pow(1 + N / 100 / 12, (double)T / 30);
}

double quarterly_capitalisation(int P, double N, int T) {
  return P * pow(1 + N / 100 / 4, (double)T / 91.25);
}
