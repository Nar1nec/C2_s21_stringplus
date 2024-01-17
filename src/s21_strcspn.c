#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  bool *was = malloc(ASCII_SIZE * sizeof(bool));
  int len_str1 = (int)s21_strlen(str1), len_str2 = (int)s21_strlen(str2);
  int pos = 0;
  for (int i = 0; i < ASCII_SIZE; ++i) was[i] = false;
  for (int i = 0; i < len_str2; ++i) was[(int)str2[i]] = true;
  while (pos < len_str1 && !was[(int)str1[pos]]) pos++;
  free(was);
  return (s21_size_t)pos;
}
