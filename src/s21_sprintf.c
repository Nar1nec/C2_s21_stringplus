#include "s21_sprintf.h"

#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list argptr;
  va_start(argptr, format);
  char *str_start = str;

  while (*format) {
    parameters Format = {0};
    Format.precision = -1;
    if (*format == '%') {
      format++;
      format = get_flags(format, &Format);
      format = get_int_from_str(format, &Format.width);
      if (*format == '.')
        format = get_int_from_str(++format, &Format.precision);
      format = get_length_flag(format, &Format);
      format = check_specifier(format, &Format);
    } else {
      *str = *format;
      *(++str) = '\0';
    }
    format++;
    char buff[BUFF_SIZE] = {'\0'};
    process(buff, &Format, argptr);
    for (int i = 0; buff[i]; i++, str++) *str = buff[i];
    // if (Format.specifier == 'n') parse_n(str - str_start, argptr);
  }
  *str = '\0';
  va_end(argptr);
  return (int)s21_strlen(str_start);
}

const char *get_flags(const char *format, parameters *Format) {
  while (s21_memchr("+- 0#", *format, 5)) {
    if (*format == '-') {
      Format->flag_minus = 1;
    } else if (*format == '+') {
      Format->flag_plus = 1;
    } else if (*format == ' ') {
      Format->flag_space = 1;
    } else if (*format == '0') {
      Format->flag_zero = 1;
    } else if (*format == '#') {
      Format->flag_sharp = 1;
    }
    format++;
  }
  return format;
}

const char *get_int_from_str(const char *format, int *parametr) {
  char *number = S21_NULL;
  s21_size_t digit_len = s21_strspn(format, NUMBERS);
  number = (char *)calloc(digit_len + 1, sizeof(char));
  if (number) {
    s21_strncpy(number, format, digit_len);
    *parametr = s21_atoi(number);
    format += digit_len;
    free(number);
  }
  return format;
}

const char *get_length_flag(const char *format, parameters *Format) {
  if (*format == 'h') {
    Format->length = 'h';
    format++;
  } else if (*format == 'l') {
    Format->length = 'l';
    format++;
  } else if (*format == 'L') {
    Format->length = 'L';
    format++;
  }
  return format;
}

const char *check_specifier(const char *format, parameters *Format) {
  int is_spec = 0;
  if (s21_strspn(format, SPEC)) {
    Format->specifier = *format;
    is_spec = 1;
  }
  return is_spec ? format : S21_NULL;
}

void process(char *str, parameters *Format, va_list argptr) {
  if (str != S21_NULL) {
    if (Format->specifier == 'c') {
      parse_c(str, Format, argptr);
    } else if (Format->specifier == 'd' || Format->specifier == 'i') {
      parse_d_i(str, argptr, *Format);
    } else if (s21_memchr("eEfgGoxXp", Format->specifier, 9)) {
      parse_f(str, argptr, *Format);
    } else if (Format->specifier == 's') {
      parse_s(str, Format, argptr);
    } else if (Format->specifier == 'u') {
      parse_u(str, argptr, *Format);
    } else if (Format->specifier == '%') {
      format_in_char(str, '%', Format);
    }
  }
}

void parse_c(char *str, parameters *Format, va_list argptr) {
  if (Format->length == 'l') {
    wchar_t wch;
    wch = va_arg(argptr, wchar_t);
    format_in_wchar(str, wch, Format);
  } else {
    char sym = va_arg(argptr, int);
    format_in_char(str, sym, Format);
  }
}

void format_in_wchar(char *buff, wchar_t wch, parameters *Format) {
  if (!Format->flag_minus && Format->width) {
    for (int i = 0; i < Format->width; i++) {
      buff[i] = ' ';
      if (i == Format->width - 1) {
        wcstombs(buff + s21_strlen(buff) - 1, &wch, BUFF_SIZE);
      }
    }
  } else if (Format->width && Format->flag_minus) {
    wcstombs(buff, &wch, BUFF_SIZE);
    for (int i = s21_strlen(buff); i < Format->width; i++) buff[i] = ' ';
  } else {
    wcstombs(buff, &wch, BUFF_SIZE);
  }
  buff[s21_strlen(buff)] = '\0';
}

void format_in_char(char *buff, char symbol, parameters *Format) {
  if (!Format->flag_minus && Format->width) {
    for (int i = 0; i < Format->width; i++) {
      buff[i] = ' ';
      if (i == Format->width - 1) buff[i] = symbol;
    }
  } else if (Format->width && Format->flag_minus) {
    buff[0] = symbol;
    for (int i = s21_strlen(buff); i < Format->width; i++) buff[i] = ' ';
  } else if (symbol == '\0' && symbol == 0) {
    Format->flag_symb = 1;
    buff[0] = symbol;
  } else {
    buff[0] = symbol;
  }
}

void parse_s(char *str, parameters *Format, va_list argptr) {
  if (Format->length == 'l') {
    wchar_t *wch;
    wch = va_arg(argptr, wchar_t *);
    format_wchar_str(str, wch, Format);
  } else {
    char *str2 = va_arg(argptr, char *);
    format_string(str, str2, Format);
  }
}

void format_string(char *buff, char *str2, parameters *Format) {
  char str_tmp[BUFF_SIZE] = {'\0'};
  s21_strcpy(str_tmp, str2);
  if (Format->precision) str_tmp[Format->precision] = '\0';

  int len = s21_strlen(str_tmp);
  int diff = Format->width - s21_strlen(str_tmp);

  if (diff > 0 && Format->flag_minus) {
    s21_strcpy(buff, str_tmp);
    s21_memset(buff + len, ' ', diff);
  } else if (diff > 0) {
    s21_memset(buff, ' ', diff);
    s21_strcpy(buff + diff, str_tmp);
  } else {
    s21_strcpy(buff, str_tmp);
  }
}

void format_wchar_str(char *buff, wchar_t *wch, parameters *Format) {
  char str_tmp[BUFF_SIZE] = {'\0'};
  wcstombs(str_tmp, wch, BUFF_SIZE);
  if (Format->precision) str_tmp[Format->precision] = '\0';

  int len = s21_strlen(str_tmp);
  int diff = Format->width - s21_strlen(str_tmp);

  if (diff > 0 && Format->flag_minus) {
    s21_strcpy(buff, str_tmp);
    s21_memset(buff + len, ' ', diff);
  } else if (diff > 0) {
    s21_memset(buff, ' ', diff);
    s21_strcpy(buff + diff, str_tmp);
  } else {
    s21_strcpy(buff, str_tmp);
  }
}

void parse_d_i(char *tmp_str, va_list argptr, parameters Format) {
  long int num_d;
  if (Format.length == 'l') {
    num_d = (long int)va_arg(argptr, long long int);
  } else if (Format.length == 'h') {
    num_d = (short int)va_arg(argptr, int);
  } else {
    num_d = va_arg(argptr, int);
  }
  s21_itoa(tmp_str, num_d);
  precision_integer(tmp_str, Format);
  flags_processing(tmp_str, Format);
}

void parse_u(char *tmp_str, va_list argptr, parameters Format) {
  unsigned long num_u;
  if (Format.length == 'l') {
    num_u = va_arg(argptr, unsigned long long);
  } else if (Format.length == 'h') {
    num_u = (unsigned short)va_arg(argptr, unsigned long long);
  } else {
    num_u = (unsigned int)va_arg(argptr, unsigned long);
  }
  s21_itoa(tmp_str, num_u);
  precision_integer(tmp_str, Format);
  flags_processing(tmp_str, Format);
}

char *s21_itoa(char *tmp_str, long long int num) {
  char *str_start = tmp_str;
  int minus = 0;
  if (num < 0) {
    minus = 1;
    num *= -1;
  }
  if (num == 0) {
    *tmp_str = '0';
    *(++tmp_str) = '\0';
  }
  while (num > 0) {
    *tmp_str = (num % 10) + '0';
    num /= 10;
    *(++tmp_str) = '\0';
  }
  if (minus == 1) *tmp_str = '-';
  tmp_str = str_start;
  s21_reverse(tmp_str);
  return str_start;
}

char *s21_reverse(char *str) {
  int ch;
  for (s21_size_t i = 0, j = s21_strlen(str) - 1; i < j; i++, j--) {
    ch = str[i];
    str[i] = str[j];
    str[j] = ch;
  }
  return str;
}

void precision_integer(char *tmp_str, parameters Format) {
  char buff[BUFF_SIZE + 1] = {'\0'};
  if (Format.precision == 0 && tmp_str[0] == '0') {
    buff[0] = '\0';
    s21_strcpy(tmp_str, buff);
  }
  if (Format.precision > 0) {
    s21_size_t length = s21_strlen(tmp_str);
    int i = 0, minus = 0;
    if (tmp_str[0] == '-') {
      length -= 1;
      buff[0] = '-';
      i++;
      minus = 1;
    }
    if (Format.precision > (int)length) {
      for (i = minus; i < Format.precision - (int)length + minus; i++) {
        buff[i] = '0';
      }
      for (int j = minus; tmp_str[j]; j++) {
        buff[i++] = tmp_str[j];
      }
      s21_strcpy(tmp_str, buff);
    }
  }
}

void flags_processing(char *tmp_str, parameters Format) {
  int length = (int)s21_strlen(tmp_str);
  char buff[BUFF_SIZE + 1] = {'\0'};
  if (Format.flag_space && tmp_str[0] != '-' &&
      Format.specifier != 'u') {  //  its for spase
    buff[0] = ' ';
    s21_strcat(buff, tmp_str);
    s21_strcpy(tmp_str, buff);
  }
  if (Format.flag_plus && tmp_str[0] != '-' &&
      Format.specifier != 'u') {  //  its for plus
    buff[0] = '+';
    length++;
    s21_strcat(buff, tmp_str);
    s21_strcpy(tmp_str, buff);
  }
  if (Format.width > length) {  //  its fot minus
    if (Format.flag_minus) {
      flag_minus(tmp_str, buff, length, Format);
    } else {
      flags_without_minus(tmp_str, buff, length, Format);
    }
    s21_strcpy(tmp_str, buff);
  }
}

void flag_minus(char *tmp_str, char *buff, int length, parameters Format) {
  int space_index = Format.width - length;
  if (Format.flag_space && tmp_str[0] != '-' && Format.specifier != 'u') {
    s21_strcpy(buff, tmp_str);
    s21_memset(buff + length + 1, ' ', space_index - 1);
  } else if (!Format.flag_space && tmp_str[0] == '-') {
    s21_strcpy(buff, tmp_str);
    s21_memset(buff + length, ' ', space_index);
  } else if (Format.flag_space && tmp_str[0] != '-' &&
             Format.specifier == 'u') {
    s21_strcpy(buff, tmp_str);
    s21_memset(buff + length, ' ', space_index);
  } else {
    s21_strcpy(buff, tmp_str);
    s21_memset(buff + length, ' ', space_index);
  }
}

void flags_without_minus(char *tmp_str, char *buff, int length,
                         parameters Format) {
  int space_index = Format.width - length;
  if (Format.flag_space && tmp_str[0] != '-') {
    s21_memset(buff, Format.flag_zero ? '0' : ' ', space_index);
    s21_strcat(buff, tmp_str);
  } else if (!Format.flag_space && tmp_str[0] == '-' && Format.flag_zero) {
    s21_memset(buff, '-', 1);
    s21_memset(buff + 1, Format.flag_zero ? '0' : ' ', space_index);
    s21_strcat(buff, tmp_str + 1);
  } else if (Format.flag_space && tmp_str[0] != '-' &&
             Format.specifier == 'u') {
    s21_strcpy(buff, tmp_str);
    s21_memset(buff + length, Format.flag_zero ? '0' : ' ', space_index);
  } else {
    s21_memset(buff, Format.flag_zero ? '0' : ' ', space_index);
    *(buff + space_index) = '\0';
    s21_strcat(buff, tmp_str);
  }
}

void parse_f(char *tmp_str, va_list argptr, parameters Format) {
  long double l_number = 0;
  double d_number = 0;
  if (Format.precision == -1 && !Format.point) Format.precision = 6;
  switch (Format.length) {
    case 'L':
      l_number = va_arg(argptr, long double);
      s21_float_to_str(tmp_str, l_number, &Format);
      break;
    default:
      d_number = va_arg(argptr, double);
      s21_float_to_str(tmp_str, d_number, &Format);
      break;
  }
  s21_make_string_flags(Format, tmp_str);
  s21_make_string_width(Format, tmp_str);
}

void s21_float_to_str(char *str, long double number, parameters *Format) {
  Format->sign = number < 0 ? -1 : 1;
  number *= Format->sign;
  if (Format->precision <= 0) {
    Format->flag_minus = number < 0 ? 1 : 0;
    s21_itoa(str, (long long)roundl(number));
  } else {
    long double left = 0, right = 0;
    int pos = 0;
    char result[256] = {0};
    right = modfl(number, &left);
    s21_itoa(str, (long long)roundl(left));
    s21_strcat(str, ".");
    int local_precision = Format->precision;

    for (int i = 0; i < local_precision; i++) right *= 10;
    s21_itoa(result, (long long)roundl(right));
    pos = (int)s21_strlen(result);
    for (int i = 0; pos < local_precision; i++, pos++) {
      s21_move_string(result);
      result[i] = '0';
    }
    s21_strcat(str, result);
  }
}

void s21_make_string_flags(parameters Format, char *str) {
  int pos = 0;
  if (Format.sign < 0) {
    s21_move_string(str);
    str[pos++] = '-';
    Format.sign = 0;
  }
  if (Format.flag_plus && Format.sign != 0) {
    s21_move_string(str);
    str[pos] = (Format.sign > 0) ? '+' : '-';
    Format.sign = 0;
  }
  if (Format.flag_space && Format.sign != 0) {
    s21_move_string(str);
    str[pos] = (Format.sign > 0) ? ' ' : '-';
    Format.sign = 0;
  }
  if (Format.flag_sharp && !s21_strchr(str, '.')) {
    str[s21_strlen(str)] = '.';
  }

  if (Format.flag_minus) {
    pos = (int)s21_strlen(str);
    while (pos < Format.width) {
      str[pos++] = ' ';
    }
  }
}

/* Функция сдвига строки на 1 позицию вправо */
void s21_move_string(char *str) {
  int pos = (int)s21_strlen(str);
  str[pos + 1] = '\0';
  while (pos > 0) {
    str[pos] = str[pos - 1];
    pos--;
  }
}

/* Функция обработки ширины форматной строки */
void s21_make_string_width(parameters Format, char *str) {
  int pos_str = (int)s21_strlen(str);
  if (Format.width > pos_str) {
    str[Format.width + 1] = '\0';
    while (pos_str >= 0) {
      str[Format.width--] = str[pos_str--];
    }
    for (pos_str = 0; pos_str <= Format.width; pos_str++) {
      str[pos_str] = (Format.flag_zero) ? '0' : ' ';
    }
  }
}
