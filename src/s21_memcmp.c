#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int num = 0;
  s21_size_t i = 0;
  while (i < n) {
    if (*(char *)str1 != *(char *)str2) {
      num = *(char *)str1 - *(char *)str2;
      break;
    }
    str1++;
    str2++;
    i++;
  }
  return num;
}
