#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  bool *was = malloc(ASCII_SIZE * sizeof(bool));
  int len_str1 = (int)s21_strlen(str1), len_str2 = (int)s21_strlen(str2);
  for (int i = 0; i < ASCII_SIZE; ++i) was[i] = false;
  for (int i = 0; i < len_str2; ++i) was[(int)str2[i]] = true;
  bool founded = false;
  int cnt = 0;
  for (int i = 0; ((i < len_str1) && !founded); ++i) {
    if (!was[(int)str1[i]]) {
      cnt++;
    } else {
      founded = true;
    }
  }
  free(was);
  return (founded ? (char *)(str1 + cnt) : S21_NULL);
}
