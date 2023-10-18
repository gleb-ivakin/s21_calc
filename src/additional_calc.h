#ifndef ADDITIONAL_CALC_H
#define ADDITIONAL_CALC_H
#include <math.h>

double monthly_payment_annuity(double S, double P, int N);
double monthly_payment_differentiated(double S, double Sn, double P, int N);
double simple_interest(int P, double I, int T);
double daily_capitalisation(int P, double N, int T);
double monthly_capitalisation(int P, double N, int T);
double quarterly_capitalisation(int P, double N, int T);

#endif  // ADDITIONAL_CALC_H
