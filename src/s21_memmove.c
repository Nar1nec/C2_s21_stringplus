#include "s21_string.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  if (dest < src) {
    char *p = (char *)dest;
    char *q = (char *)src;
    while (n--) {
      *p++ = *q++;
    }
  } else {
    char *p = (char *)dest + n;
    char *q = (char *)src + n;
    while (n--) {
      *--p = *--q;
    }
  }
  return dest;
}
