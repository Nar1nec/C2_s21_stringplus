#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = S21_NULL;
  if (trim_chars == S21_NULL || trim_chars[0] == '\0')
    trim_chars = " \n\t\v\f\r";
  if (src && src[0] != '\0') {
    int start_index = 0;
    int end_index = s21_strlen(src) - 1;
    for (; s21_strchr(trim_chars, src[start_index]) && src[start_index];
         start_index++) {
    }
    for (; s21_strchr(trim_chars, src[end_index]) && end_index > start_index;
         end_index--) {
    }
    res = (char *)calloc(end_index - start_index + 2, sizeof(char));
    if (res) {
      for (int i = start_index, j = 0; i <= end_index; i++, j++)
        res[j] = src[i];
      res[end_index - start_index + 1] = '\0';
    }
  } else if (src[0] == '\0') {
    res = (char *)calloc(1, sizeof(char));
    res[0] = '\0';
  }
  return res;
}
