#include <string.h>

int memcmp (const void *s1_, const void *s2_, size_t n) {
  const uint8_t *s1 = s1_;
  const uint8_t *s2 = s2_;
  size_t i;

  for (i = 0; i < n; i++)
    if (s1[i] != s2[i])
      return s1[i] < s2[i] ? -1 : 1;
  return 0;
}