#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "std.h"
#include "memory.h"

#define true 1
#define false 0
#define bool int

void *alloc$MODmem(int size);
void *realloc$MODmem(void *ptr, int size);
void free$MODmem(void *ptr);

__attribute__((visibility("default")))
int len$MODstring(const char *s1) {
  int len = (int)strlen(s1);
  return len;
}

__attribute__((visibility("default")))
void badd_nil$MODstring(char **s1) {
  if (!s1 || !*s1) return;

  int len = len$MODstring(*s1);
  (*s1)[len] = '\0';
}

__attribute__((visibility("default")))
char *cat$MODstring(const char *s1, const char *s2) {
  if (!s1 || !s2)
    return NULL;

  size_t len1 = len$MODstring(s1);
  size_t len2 = len$MODstring(s2);

  char *result = (char *)alloc$MODmem(len1 + len2 + 1);
  result[len1 + len2] = '\0';
  if (!result)
    return NULL;

  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2);
  result[len1 + len2] = '\0';

  return result;
}

__attribute__((visibility("default")))
void  bufcat$MODstring(char **s1, const char *s2) {
  if (!s1 || !s2)
    return;

  size_t len1 = len$MODstring(*s1);
  size_t len2 = len$MODstring(s2);

  char *old = *s1;
  char *newbuf = (char *)alloc$MODmem(len1 + len2 + 1);
  if (!newbuf) return;

  memcpy(newbuf, old, len1);
  memcpy(newbuf + len1, s2, len2);
  newbuf[len1 + len2] = '\0';

  *s1 = newbuf;
}

__attribute__((visibility("default")))
char *cutidx$MODstring(const char *s1, int begin, int end) {
  int len = len$MODstring(s1);

  if (begin < 0)
    begin = 0;
  if (end > len)
    end = len;
  if (begin >= end)
    return strdup(s1);

  int new_len = len - (end - begin);
  char *result = (char *)alloc$MODmem(new_len);
  if (!result)
    return NULL;

  strncpy(result, s1, begin);

  strcpy(result + begin, s1 + end);

  return result;
}

__attribute__((visibility("default")))
void bufcutidx$MODstring(const char **s1, int begin, int end) {
  if (!s1)
    return;
  *s1 = cutidx$MODstring(*s1, begin, end);
}

__attribute__((visibility("default")))
char *cut$MODstring(const char *str, const char *sub) {
  const char *pos = strstr(str, sub);
  if (pos == NULL) {
    return strdup(str);
  }

  size_t len_before = pos - str;
  size_t len_sub = len$MODstring(sub);
  size_t len_after = len$MODstring(pos + len_sub);

  char *result = (char *)alloc$MODmem(len_before + len_after + 1);
  if (!result)
    return NULL;

  strncpy(result, str, len_before);
  strcpy(result + len_before, pos + len_sub);
  return result;
}

__attribute__((visibility("default")))
void bufcut$MODstring(char **str_ptr, const char *sub) {
  char *str = *str_ptr;
  char *pos = strstr(str, sub);
  if (!pos)
    return;

  size_t len_before = pos - str;
  size_t len_sub = len$MODstring(sub);
  size_t len_after = len$MODstring(pos + len_sub);

  char *nuevo = (char *)alloc$MODmem(len_before + len_after + 1);
  if (!nuevo)
    return;

  strncpy(nuevo, str, len_before);
  strcpy(nuevo + len_before, pos + len_sub);

  free$MODmem(str);
  *str_ptr = nuevo;
}

__attribute__((visibility("default")))
char *sub$MODstring(const char *s1, int begin, int end) {
  int len = len$MODstring(s1);

  if (begin < 0)
    begin = 0;
  if (end > len)
    end = len;
  if (begin >= end)
    return strdup("");

  int sub_len = end - begin;
  char *result = (char *)alloc$MODmem(sub_len + 1);
  result[sub_len] = '\0';
  if (!result)
    return NULL;

  strncpy(result, s1 + begin, sub_len);

  return result;
}

void bufsub$MODstring(const char **s1, int begin, int end) {
  if (!s1)
    return;
  *s1 = sub$MODstring(*s1, begin, end);
}

__attribute__((visibility("default")))
int to_int$MODstring(const char *s1) { return atoi(s1); }

__attribute__((visibility("default")))
float to_double$MODstring(const char *s1) { return atof(s1); }

__attribute__((visibility("default")))
char *to_str$MODinteger(const int n) {
  char buf[32];
  int len = 0;
  int x = n;

  if (x == 0) {
    buf[len++] = '0';
  } else {
    int negative = 0;
    if (x < 0) {
      negative = 1;
      x = -x;
    }

    while (x > 0) {
      buf[len++] = '0' + (x % 10);
      x /= 10;
    }

    if (negative) {
      buf[len++] = '-';
    }
  }

  for (int i = 0; i < len / 2; ++i) {
    char tmp = buf[i];
    buf[i] = buf[len - 1 - i];
    buf[len - 1 - i] = tmp;
  }

  char *res = (char *)alloc$MODmem(len + 1);
  if (!res)
    return NULL;

  for (int i = 0; i < len; ++i) {
    res[i] = buf[i];
  }
  res[len] = '\0';

  return res;
}

__attribute__((visibility("default")))
char *to_str$MODdecimal(double n, int decimals) {
  char buf[64];
  int len = 0;

  if (n < 0) {
    buf[len++] = '-';
    n = -n;
  }

  int int_part = (int)n;

  float frac_part = n - int_part;

  char int_buf[32];
  int int_len = 0;
  if (int_part == 0) {
    int_buf[int_len++] = '0';
  } else {
    while (int_part > 0) {
      int_buf[int_len++] = '0' + (int_part % 10);
      int_part /= 10;
    }

    for (int i = 0; i < int_len / 2; i++) {
      char tmp = int_buf[i];
      int_buf[i] = int_buf[int_len - 1 - i];
      int_buf[int_len - 1 - i] = tmp;
    }
  }

  for (int i = 0; i < int_len; i++) {
    buf[len++] = int_buf[i];
  }

  buf[len++] = '.';

  for (int i = 0; i < decimals; i++) {
    frac_part *= 10;
    int digit = (int)frac_part;
    buf[len++] = '0' + digit;
    frac_part -= digit;
  }

  char *res = (char *)alloc$MODmem(len);
  if (!res)
    return NULL;

  for (int i = 0; i < len; i++) {
    res[i] = buf[i];
  }

  return res;
}

__attribute__((visibility("default")))
int cmp$MODstring(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (*s1 != *s2)
      return 0;
    s1++;
    s2++;
  }
  return *s1 == *s2;
}

__attribute__((visibility("default")))
int charcmp$MODcharacter(const char c1, const char c2) { return c1 == c2; }

__attribute__((visibility("default")))
char *dig_to_abc$MODstring(const int n) {
  if (n >= 0 && n < 26) {
    char *res = (char *)alloc$MODmem(2); // 1 para la letra, 1 para '\0'
    if (res == NULL)
      return NULL; // Verificación de malloc
    res[0] = 'A' + n;
    res[1] = '\0';
    return res;
  } else {
    char *res = (char *)alloc$MODmem(2);
    if (res == NULL)
      return NULL;
    res[0] = '?';
    res[1] = '\0';
    return res;
  }
}

__attribute__((visibility("default")))
bool is_empty$MODstring(const char *s) {
  bool empty = true;
  for (int i = 0; i < len$MODstring(s); i++) {
    if (s[i] != '\0') {
      empty = false;
      break;
    }
  }

  return empty;
}

__attribute__((visibility("default")))
bool is_whitespace$MODstring(const char *s) {
  bool is_whitespace = true;
  for (int i = 0; i < len$MODstring; i++) {
    if (s[i] != ' ') {
      is_whitespace = false;
      break;
    }
  }

  return is_whitespace;
}

__attribute__((visibility("default")))
char *trim$MODstring(const char *s) {
  if (s == NULL)
    return NULL;

  const char *begin = s;
  const char *end;

  while (*begin && isspace((unsigned char)*begin)) {
    ++begin;
  }

  if (*begin == '\0') {
    char *empty = (char *)alloc$MODmem(1);
    if (empty)
      empty[0] = '\0';
    return empty;
  }

  end = begin + strlen(begin) - 1;
  while (end > begin && isspace((unsigned char)*end)) {
    --end;
  }

  size_t len = end - begin + 1;

  char *new = (char *)alloc$MODmem(len + 1);
  if (!new)
    return NULL;
  strncpy(new, begin, len);
  new[len] = '\0';

  return new;
}

__attribute__((visibility("default")))
void buftrim$MODstring(const char **buf) {
  if (!buf)
    return;
  *buf = trim$MODstring(*buf);
}

__attribute__((visibility("default")))
char *repl$MODstring(const char *s, const char *old_sub, const char *new_sub) {
  if (!s || !old_sub || !new_sub)
    return NULL;

  size_t old_len = len$MODstring(old_sub);
  size_t new_len = len$MODstring(new_sub);
  size_t count = 0;

  const char *tmp = s;
  while ((tmp = strstr(tmp, old_sub))) {
    ++count;
    tmp += old_len;
  }

  size_t result_len = len$MODstring(s) + count * (new_len - old_len);

  char *result = (char *)alloc$MODmem(result_len);
  if (!result)
    return NULL;

  char *dest = result;
  while (*s) {
    if (strstr(s, old_sub) == s) {
      memcpy(dest, new_sub, new_len);
      s += old_len;
    } else {
      *dest++ = *s++;
    }
  }

  return result;
}

__attribute__((visibility("default")))
void bufrepl$MODstring(const char **buf, const char *old_sub, const char *new_sub) {
  if (!buf)
    return;
  *buf = repl$MODstring(*buf, old_sub, new_sub);
}

__attribute__((visibility("default")))
int contains$MODstring(const char *str, const char *substring) {
  if (strstr(str, substring) != NULL)
    return 1;
  else
    return 0;
}

__attribute__((visibility("default")))
int constains_case$MODstring(const char *str, const char *substring) {
  if (strcasestr(str, substring) != NULL)
    return 1;
  else
    return 0;
}

__attribute__((visibility("default")))
int find$MODstring(const char *str, const char *substring) {
  char *pos = (char *)strstr(str, substring);
  if (pos == NULL)
    return -1;
  return (int)(pos - str);
}

__attribute__((visibility("default")))
char **get_splited$MODstring(const char *str, const char *delimiter, int *bufsize) {
  if (delimiter == NULL) delimiter = "\n";

  int capacity = 2;
  int size = 0;
  if (bufsize) *bufsize = 0;

  char **buf = (char**)alloc$MODmem(sizeof(char*) * capacity);
  if (buf == NULL) {
    perror$MODstd("ERROR: could not allocate memory for string::get_splited function\n");
    return NULL;
  }

  const char *cursor = str;
  int delimiter_pos;

  while ((delimiter_pos = find$MODstring(cursor, delimiter)) != -1) {
    if (size + 1 >= capacity) {
      capacity *= 2;
      buf = (char**)realloc$MODmem(buf, sizeof(char*) * capacity);
      if (buf == NULL) {
        perror$MODstd("ERROR: realloc failed in string::get_splited\n");
        return NULL;
      }
    }

    char *s = sub$MODstring(cursor, 0, delimiter_pos);
    buf[size++] = s;

    cursor += delimiter_pos + strlen(delimiter);
  }

  // agregar la última parte (después del último delimitador)
  if (*cursor != '\0') {
    if (size + 1 >= capacity) {
      capacity *= 2;
      buf = (char**)realloc$MODmem(buf, sizeof(char*) * capacity);
      if (buf == NULL) {
        perror$MODstd("ERROR: realloc failed in string::get_splited\n");
        return NULL;
      }
    }

    char *s = sub$MODstring(cursor, 0, len$MODstring(cursor));
    buf[size++] = s;
  }

  if (size + 1 >= capacity) {
    buf = (char**)realloc$MODmem(buf, sizeof(char*) * (capacity + 1));
  }
  buf[size] = NULL;

  if (bufsize) *bufsize = size;
  return buf;
}
