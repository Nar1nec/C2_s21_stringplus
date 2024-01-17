#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#include "s21_string.h"

#define BUFF_SIZE 1024
#define NUMBERS "0123456789"
#define SPEC "cdieEfgGosuxXpn%"

typedef struct parameters {
  int flag_minus;
  int flag_plus;
  int flag_space;
  int flag_sharp;
  int flag_zero;
  int width;
  int precision;
  int point;
  char length;     //  h, l
  char specifier;  // c, d, i, f, s, u, %
  int sign;
  int flag_symb;
} parameters;

int s21_sprintf(char *str, const char *format, ...);

int s21_atoi(const char *str);
const char *get_flags(const char *format, parameters *Format);
const char *get_int_from_str(const char *format, int *parametr);
const char *get_length_flag(const char *format, parameters *Format);
const char *check_specifier(const char *format, parameters *Format);
void process(char *str, parameters *Format, va_list argptr);
void process(char *str, parameters *Format, va_list argptr);

void parse_n(int num_chars, va_list argptr);

void parse_c(char *str, parameters *Format, va_list argptr);
void format_in_wchar(char *buff, wchar_t wch, parameters *Format);
void format_in_char(char *buff, char symbol, parameters *Format);
void parse_s(char *str, parameters *Format, va_list argptr);
void format_string(char *buff, char *str2, parameters *Format);
void format_wchar_str(char *buff, wchar_t *wch, parameters *Format);

void parse_d_i(char *tmp_str, va_list argptr, parameters Format);
void parse_u(char *tmp_str, va_list argptr, parameters Format);
char *s21_itoa(char *tmp_str, long long int num);
char *s21_reverse(char *str);
void precision_integer(char *tmp_str, parameters Format);
void flags_processing(char *tmp_str, parameters Format);
void flag_minus(char *tmp_str, char *buff, int length, parameters Format);
void flags_without_minus(char *tmp_str, char *buff, int length,
                         parameters Format);

void parse_f(char *tmp_str, va_list argptr, parameters Format);
void s21_float_to_str(char *str, long double number, parameters *Format);
void s21_make_string_flags(parameters Format, char *str);
void s21_move_string(char *str);
void s21_make_string_width(parameters Format, char *str);

#endif  // SRC_S21_SPRINTF_H_
