#include "s21_string.h"

char *s21_strcpy(char *dest, const char *src) {
  char *ptr = dest;
  s21_size_t i = 0;
  for (; *(src + i) != '\0'; i++) {
    *(dest + i) = *(src + i);
  }
  dest[i] = '\0';
  return ptr;
}
