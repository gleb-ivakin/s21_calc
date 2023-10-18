#include <check.h>

#include "additional_calc.h"
#include "smartcalc.h"

START_TEST(smartcalc_tests) {
  double res;
  char* postfix_str;
  char* fixed_str;
  char* repl_x;
  int check_res;

  postfix_str = to_postfix("2+2");
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, 4, 9e-7);

  postfix_str = to_postfix(
      "(1+sin(3.50))*(1+0.9*cos(8*3.50))*(1+0.1*cos(24*3.50))*(0.9+0.05*cos("
      "200*3.50))");
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, 0.06939030646, 9e-7);

  postfix_str = to_postfix(
      "(1+asin(0.5))*(1+0.9*acos(0.5))*(1+0.1*tan(24*3.50))*(0.9+0.05*atan(0.5)"
      ")");
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, 2.4376281528805, 9e-7);

  postfix_str = to_postfix(
      "(1+sqrt(3.50))*(1+0.9*log(8*3.50))*(1+0.1*ln(24*3.50))*(0.9+0.05*sin("
      "200*3.50))");
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, 8.84422262636, 9e-7);

  fixed_str = replace_mod("(-1+2*(+3-4)/10.3mod3^2)");
  postfix_str = to_postfix(fixed_str);
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, -1.19417475728155, 9e-7);

  check_res = check_input(
      "(1+sqrt(3.50))*(1+0.9*log(8*3.50))*(1+0.1*ln(24*3.50))*(0.9+0.05*sin("
      "200*3.50))");
  ck_assert_int_eq(check_res, 0);

  check_res = check_input("cosx");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("/1");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("1*");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("mod1");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("1+2cos");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("1+2+()");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("))))");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("5+(3)(");
  ck_assert_int_eq(check_res, 1);

  check_res = check_input("4,3");
  ck_assert_int_eq(check_res, 1);

  check_res = string_contains_x("cos(x)");
  ck_assert_int_eq(check_res, 1);

  repl_x = replace_x(
      "(1+sin(x))*(1+0.9*cos((8)*x))*(1+0.1*cos(24*x))*(0.9+0.05*cos(200*x))",
      3.50);
  postfix_str = to_postfix(repl_x);
  res = calc_postfix(postfix_str);
  ck_assert_double_eq_tol(res, 0.06939030646, 9e-7);

  res = calc_postfix("nan");
  ck_assert_double_nan(res);

  free(postfix_str);
  free(fixed_str);
}
END_TEST

START_TEST(additional_calc_tests) {
  double res, rate;

  rate = (double)20 / 100 / 12;
  res = monthly_payment_annuity(50000, rate, 12);
  // printf("rate: %lf, res: %lf\n", rate, res);
  ck_assert_double_eq_tol(res, 4631.73, 0.01);

  res = monthly_payment_differentiated(50000, 50000, rate, 12);
  ck_assert_double_eq_tol(res, 5000, 0.01);

  res = simple_interest(50000, 20, 365);
  ck_assert_double_eq_tol(res, 10000, 0.01);

  res = daily_capitalisation(50000, 20, 365);
  ck_assert_double_eq_tol(res, 61066.79, 0.01);

  res = monthly_capitalisation(50000, 20, 365);
  ck_assert_double_eq_tol(res, 61137.75, 0.01);

  res = quarterly_capitalisation(50000, 20, 365);
  ck_assert_double_eq_tol(res, 60775.31, 0.01);
}
END_TEST

int main(void) {
  Suite* s1 = suite_create("SmartCalc");
  TCase* tc1 = tcase_create("smartcalc_tests");
  TCase* tc2 = tcase_create("additional_calc_tests");
  SRunner* sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1);
  tcase_add_test(tc1, smartcalc_tests);

  suite_add_tcase(s1, tc2);
  tcase_add_test(tc2, additional_calc_tests);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
