#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *ptr = S21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (*((unsigned char *)str) == (unsigned char)c) {
      ptr = (unsigned char *)str;
      break;
    }
    str++;
  }
  return ptr;
}
