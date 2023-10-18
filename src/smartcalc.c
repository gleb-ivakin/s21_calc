#include "smartcalc.h"

void push_char(CharNode** head, char data) {
  CharNode* tmp = (CharNode*)malloc(sizeof(CharNode));
  tmp->value = data;
  tmp->next = (*head);
  (*head) = tmp;
}

char pop_char(CharNode** head) {
  CharNode* prev = NULL;
  prev = (*head);
  char ret_val = 0;
  if (prev) ret_val = prev->value;
  if (*head) (*head) = (*head)->next;
  free(prev);

  return ret_val;
}

void push_double(DoubleNode** head, double data) {
  DoubleNode* tmp = (DoubleNode*)malloc(sizeof(DoubleNode));
  tmp->value = data;
  tmp->next = (*head);
  (*head) = tmp;
}

double pop_double(DoubleNode** head) {
  DoubleNode* prev = NULL;
  prev = (*head);
  double ret_val = NAN;
  if (prev) ret_val = prev->value;
  if (*head) (*head) = (*head)->next;
  free(prev);

  return ret_val;
}

char* get_string_num(char* str, int* pos) {
  char* str_num = malloc(strlen(str) + 1);
  int i;
  for (i = 0; *pos < (int)strlen(str); i++, (*pos)++) {
    char c = str[*pos];
    if (is_digit(c) || is_point(c)) {
      str_num[i] = c;
    } else {
      (*pos)--;
      str_num[i] = 0;
      break;
    }
  }
  str_num[i] = 0;

  return str_num;
}

int it_is_operation(char c) {
  int ret = 0;

  if (c == '(')
    ret = 1;
  else if (c == '+' || c == '-')
    ret = 2;
  else if (c == '*' || c == '/' || c == '%')
    ret = 3;
  else if (c == '^')
    ret = 4;
  else if (c == '~' || c == '#')
    ret = 5;

  return ret;
}

double process_function(char* str, int* pos) {
  // передаю в функцию строку и номер символа
  // узнаю, для какого выражения применить функцию
  // узнаю, какая это функция
  // применяю её и возвращаю результат

  char buff[256] = "";

  int i = 0;
  while (str[*pos] != '(' && str[*pos]) buff[i++] = str[(*pos)++];
  buff[i] = 0;

  char expression[256] = "";
  i = 0;
  (*pos)++;
  int brackets_counter = 0;

  for (;; (*pos)++) {
    char c = str[(*pos)];
    if (c == ')' && brackets_counter == 0) break;

    if (c == '(')
      brackets_counter++;
    else if (c == ')')
      brackets_counter--;

    expression[i++] = c;
  }
  expression[i] = 0;

  char* postfix_expr = to_postfix(expression);
  double num = calc_postfix(postfix_expr);

  char* functions[9] = {"cos",  "sin", "acos", "asin", "atan",
                        "sqrt", "ln",  "log",  "tan"};
  for (i = 0; i < 9; i++)
    if (strcmp(buff, functions[i]) == 0) break;

  double ret = 0;
  if (i == 0)
    ret = cos(num);
  else if (i == 1)
    ret = sin(num);
  else if (i == 2)
    ret = acos(num);
  else if (i == 3)
    ret = asin(num);
  else if (i == 4)
    ret = atan(num);
  else if (i == 5)
    ret = sqrt(num);
  else if (i == 6)
    ret = log(num);
  else if (i == 7)
    ret = log10(num);
  else if (i == 8)
    ret = tan(num);

  free(postfix_expr);

  return ret;
}

int is_mod(char* str, int pos) {
  char buff[4];
  int i;
  for (i = 0; i < 3 && str[pos]; i++, pos++) {
    buff[i] = str[pos];
  }
  buff[i] = 0;

  int ret = 0;
  if (strcmp(buff, "mod") == 0) ret = 1;

  return ret;
}

char* replace_mod(char* input_str) {
  char* output_str = (char*)malloc(256);
  int i, j;
  char c;
  for (i = 0, j = 0; input_str[i]; i++, j++) {
    c = input_str[i];
    if (is_mod(input_str, i)) {
      output_str[j] = '%';
      i += 2;
    } else {
      output_str[j] = c;
    }
  }
  output_str[j] = 0;

  return output_str;
}

int check_input(char* input_str) {
  int error = 0;
  int brackets_ratio = 0;

  char c = input_str[0];
  int inp_str_len = (int)strlen(input_str);
  if (c == '*' || c == '/' ||
      c == '%')  // первый символ не должен быть /, * или mod
    error = 1;
  else if (is_op(input_str[inp_str_len - 1]))  // последний символ не должен
                                               // быть знаком операции
    error = 1;

  int i = 0, j = 0;

  for (; i < inp_str_len && !error; i++, j++) {
    c = input_str[i];
    char next_c = input_str[i + 1];

    if (brackets_ratio < 0) {  // скобка закрылась, не успев открыться
      error = 1;
    } else if (is_digit(c) && is_letter(next_c)) {
      error = 1;
    } else if (is_letter(c)) {
      char buff[256] = "";
      int k = 0;

      for (int l = i; is_letter(input_str[l]);) buff[k++] = input_str[l++];
      buff[k] = 0;
      char* functions[11] = {"cos",  "sin", "tan", "acos", "asin", "atan",
                             "sqrt", "ln",  "log", "x",    "X"};

      for (k = 0; k < 11; k++)
        if (strcmp(buff, functions[k]) == 0) break;

      if (k == 11) {  // буквы не совпали - значит, ошибка
        error = 1;
      } else {
        i += (int)strlen(functions[k]) - 1;

        if (k <= 8 && input_str[i + 1] != '(') error = 1;
      }
    } else if (is_op(c) && !(is_digit(next_c) || next_c == '(' ||
                             is_letter(next_c) || is_x(next_c))) {
      // после знака операции должна быть цифра, x, функция или открывающая
      // скобка
      error = 1;
    } else if (c == '(') {
      if (next_c == ')' || c == '*' || c == '/' || c == '%')
        error = 1;
      else
        brackets_ratio++;
    } else if (c == ')') {
      if (next_c == '(' || is_letter(next_c))
        error = 1;
      else
        brackets_ratio--;
    } else if (c == ',') {
      error = 1;
    }
  }

  if (brackets_ratio != 0)  // скобка открылась, но не закрылась
    error = 1;

  return error;
}

char* to_postfix(char* infix_str) {
  char* postfix_str = (char*)malloc(9999);
  postfix_str[0] = 0;
  CharNode* stack = NULL;
  int p = 0;  // for symbols of postfix_str

  for (int i = 0; i < (int)strlen(infix_str); i++) {
    char c = infix_str[i];

    // если символ - цифра, то записываем число в строку
    if (is_digit(c) || is_point(c)) {
      postfix_str[p++] = c;
      char next = infix_str[i + 1];
      if (!is_digit(next) && !is_point(next)) postfix_str[p++] = ' ';
      // если символ - буква, то вычисляем результат функции и записываем его в
      // строку
    } else if (is_letter(c)) {
      double func_res = process_function(infix_str, &i);
      int flag = 0;

      if (func_res < 0) {
        flag = 1;
        func_res *= -1;
      }

      postfix_str[p] = 0;
      size_t old_len = strlen(postfix_str);
      char buff[999] = "";
      sprintf(buff, "%.53lf", func_res);
      strcat(postfix_str, buff);
      strcat(postfix_str, " \0");

      if (flag) strcat(postfix_str, "~ \0");

      size_t new_len = strlen(postfix_str);
      p += (int)(new_len - old_len);
      // если символ - открывающая скобка, то заносим её в стэк
    } else if (c == '(') {
      push_char(&stack, c);
      // если символ - закрывающая скобка, то записываем в строку элементы из
      // стэка, пока скобка не закроется
    } else if (c == ')') {
      while (stack && stack->value != '(') postfix_str[p++] = pop_char(&stack);
      pop_char(&stack);
      // если символ - знак операции, то записываем в строку элементы из стэка,
      // пока приоритет этой операции ниже их
    } else if (it_is_operation(c)) {
      char op = c;

      if (op == '-' && (i == 0 || (i > 0 && it_is_operation(infix_str[i - 1]))))
        op = '~';
      else if (op == '+' &&
               (i == 0 || (i > 0 && it_is_operation(infix_str[i - 1]))))
        op = '#';

      while (stack && (it_is_operation(stack->value) >= it_is_operation(op))) {
        postfix_str[p++] = pop_char(&stack);
        postfix_str[p++] = ' ';
      }

      push_char(&stack, op);
    }
  }

  while (stack) postfix_str[p++] = pop_char(&stack);

  postfix_str[p] = 0;

  return postfix_str;
}

double calculate(double a, double b, char op) {
  double ret = 0;

  if (op == '+')
    ret = a + b;
  else if (op == '-')
    ret = a - b;
  else if (op == '*')
    ret = a * b;
  else if (op == '/')
    ret = a / b;
  else if (op == '^')
    ret = pow(a, b);
  else if (op == '%')
    ret = fmod(a, b);

  return ret;
}

double calc_postfix(char* postfix_str) {
  DoubleNode* stack = NULL;
  int no_errors = 1;

  for (int i = 0; postfix_str[i] && no_errors; i++) {
    char c = postfix_str[i];

    if (is_digit(c)) {
      char* number = get_string_num(postfix_str, &i);
      push_double(&stack, atof(number));
      free(number);
    } else if (it_is_operation(c)) {
      if (c == '~') {  // унарный минус
        push_double(&stack, pop_double(&stack) * (-1));
      } else if (c == '#') {  // унарный плюс
        push_double(&stack, pop_double(&stack));
      } else {
        double second = pop_double(&stack);
        double first = pop_double(&stack);
        push_double(&stack, calculate(first, second, c));
      }
    } else if (c == 'n') {  // если в строке содержится "nan"
      no_errors = 0;
    }
  }

  double ret;
  if (no_errors && stack)
    ret = stack->value;
  else
    ret = NAN;
  free(stack);

  return ret;
}

char* replace_x(char* str, double x) {
  char* ret = (char*)malloc(9999);
  char x_str[999] = "";
  sprintf(x_str, "%.53lf", x);
  strcat(x_str, "\0");
  size_t x_str_len = strlen(x_str);

  int i = 0, j = 0;
  for (; str[i]; i++, j++) {
    if (str[i] == 'x' || str[i] == 'X') {
      ret[j] = 0;
      strcat(ret, x_str);
      j += x_str_len - 1;
    } else {
      ret[j] = str[i];
    }
  }
  ret[j] = 0;

  return ret;
}

int string_contains_x(char* input_str) {
  int ret = 0;

  for (int i = 0; input_str[i] && !ret; i++)
    if (is_x(input_str[i])) ret = 1;

  return ret;
}
