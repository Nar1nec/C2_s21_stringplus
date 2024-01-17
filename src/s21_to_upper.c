#include "s21_string.h"

void* s21_to_upper(const char* str) {
  char* ans = S21_NULL;
  if (str != S21_NULL) {
    ans = (char*)calloc(s21_strlen(str) + 1, sizeof(char));
    for (int i = 0; i < (int)s21_strlen(str); ++i) {
      ans[i] = str[i];
      if (str[i] >= 'a' && str[i] <= 'z') ans[i] -= 32;
    }
  }
  return ans;
}
