#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *copy_str = dest;
  while (n--) {
    if (*src != '\0')
      *dest++ = *src++;
    else
      *dest++ = '\0';
  }
  return copy_str;
}
