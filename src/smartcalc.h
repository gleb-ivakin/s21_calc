#ifndef SMARTCALC_H
#define SMARTCALC_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define is_digit(c) (c >= '0' && c <= '9')
#define is_point(c) (c == '.')
#define is_letter(c) (c >= 'a' && c <= 'z')
#define is_op(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
#define is_bracket(c) (c == '(' || c == ')')
#define is_x(c) (c == 'x' || c == 'X')

typedef struct CharNode {
  char value;
  struct CharNode* next;
} CharNode;

typedef struct DoubleNode {
  double value;
  struct DoubleNode* next;
} DoubleNode;

void push_char(CharNode** head, char data);
char pop_char(CharNode** head);
void push_double(DoubleNode** head, double data);
double pop_double(DoubleNode** head);
char* get_string_num(char* str, int* pos);
int it_is_operation(char c);
double process_function(char* str, int* pos);
int check_input(char* input_str);
char* to_postfix(char* infix_str);
double calculate(double a, double b, char op);
double calc_postfix(char* postfix_str);
int is_mod(char* str, int pos);
char* replace_mod(char* input_str);
int string_contains_x(char* input_str);
char* replace_x(char* str, double x);

#endif  // SMARTCALC_H
