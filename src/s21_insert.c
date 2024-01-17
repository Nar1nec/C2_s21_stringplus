#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = S21_NULL;
  if (src && str) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    s21_size_t new_len = src_len + str_len;

    if (start_index <= src_len) {
      res = (char *)calloc(new_len + 1, sizeof(char));

      if (res) {
        s21_strncpy(res, src, start_index);
        src = src + start_index;
        s21_strncat(res, str, str_len);
        s21_strncat(res, src, src_len - start_index);
      }
    }
  } else if (str == S21_NULL && src != S21_NULL) {
    res = calloc(s21_strlen(src) + 1, sizeof(char));
    s21_strcpy(res, src);
  }
  return (void *)res;
}
