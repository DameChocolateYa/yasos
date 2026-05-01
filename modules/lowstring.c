/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

// IMPORTANT: string functions as strcar could have important memory issues

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "std.h"
#include "memory.h"
#include "cutype.h"
#include "mem.h"
#include "string.h"

#define true 1
#define false 0
#define bool int

void *alloc$MODmem(int size);
void *realloc$MODmem$MODmem(void *ptr, int size);
void free$MODmem$MODmem(void *ptr);

__attribute__((visibility("default")))
int len$MODstring(const char *s1) {
  int len = (int)strlen(s1);
  return len;
}

/*__attribute__((visibility("default")))
void union$MODstring(char *dest, const char *src) {
  while (*dest) dest++;
  while (*src) {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = '\0';
}*/

/*__attribute__((visibility("default")))
void new_union$MODstring(char *dest, const char *s1, const char *s2) {
  while (*s1) {
    *dest = *s1;
    dest++;
    s1++;
  }
  while (*s2) {
    *dest = *s2;
    dest++;
    s2++;
  }

  *dest = '\0';
}*/

__attribute__((visibility("default")))
void badd_nil$MODstring(char **s1) {
  if (!s1 || !*s1) return;

  int len = len$MODstring(*s1);
  (*s1)[len] = '\0';
}

__attribute__((visibility("default")))
bool is_badd_nil$MODstring(const char c) {
  //if (!c) return false;
  if (c == '\0') {
    return true;
  }

  return false;
}

__attribute__((visibility("default")))
void add_badd_nil$MODstring(char *s1) {
  (s1)[len$MODstring(s1)] = '\0';
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
void bufcutidx$MODstring(char **s1, int begin, int end) {
  if (!s1)
    return;
  *s1 = cutidx$MODstring(*s1, begin, end);
}

/*__attribute__((visibility("default")))
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
}*/

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

extern char *sub$MODstring(const char *string, int begin, int end);

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
  }
  else {
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
  }
  else {
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
int cmp$MODcharacter(const char c1, const char c2) { return c1 == c2; }

__attribute__((visibility("default")))
int cmp_int$MODcharacter(const char c, const int i) { return c == i; }

__attribute__((visibility("default")))
char *to_str$MODcharacter(char c) {
  char *s = alloc$MODmem(2);
  s[0] = c;
  s[1] = '\0';

  return s;
}

__attribute__((visibility("default")))
char *dig_to_abc$MODstring(const int n) {
  if (n >= 0 && n < 26) {
    char *res = (char *)alloc$MODmem(2); // 1 para la letra, 1 para '\0'
    if (res == NULL)
      return NULL; // Verificación de alloc$MODmem
    res[0] = 'A' + n;
    res[1] = '\0';
    return res;
  }
  else {
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

/*__attribute__((visibility("default")))
bool is_whitespace$MODstring(const char *s) {
  bool is_whitespace = true;
  for (int i = 0; i < len$MODstring; i++) {
    if (s[i] != ' ') {
      is_whitespace = false;
      break;
    }
  }

  return is_whitespace;
}*/

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
  memcpy(new, begin, len);
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
    }
    else {
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

extern int find$MODstring(const char *str, const char *substring);

__attribute__((visibility("default")))
char **get_splited$MODstring(const char *str, const char *delimiter, int *bufsize) {
  if (delimiter == NULL) delimiter = "\n";

  int capacity = 2;
  int size = 0;
  if (bufsize) *bufsize = 0;

  char **buf = (char **)alloc$MODmem(sizeof(char *) * capacity);
  if (buf == NULL) {
    perror$MODstd("ERROR: could not allocate memory for string::get_splited function\n");
    return NULL;
  }

  const char *cursor = str;
  int delimiter_pos;

  while ((delimiter_pos = find$MODstring(cursor, delimiter)) != -1) {
    if (size + 1 >= capacity) {
      capacity *= 2;
      buf = (char **)realloc$MODmem(buf, sizeof(char *) * capacity);
      if (buf == NULL) {
        perror$MODstd("ERROR: realloc$MODmem failed in string::get_splited\n");
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
      buf = (char **)realloc$MODmem(buf, sizeof(char *) * capacity);
      if (buf == NULL) {
        perror$MODstd("ERROR: realloc$MODmem failed in string::get_splited\n");
        return NULL;
      }
    }

    char *s = sub$MODstring(cursor, 0, len$MODstring(cursor));
    buf[size++] = s;
  }

  if (size + 1 >= capacity) {
    buf = (char **)realloc$MODmem(buf, sizeof(char *) * (capacity + 1));
  }
  buf[size] = NULL;

  if (bufsize) *bufsize = size;
  return buf;
}

__attribute__((visibility("default")))
void cpy$MODstring(char *dest, const char *src) {
  while ((*dest++ = *src++))
    ;
}

// Functions of String struct implementation

#include "critical.h"
#include <stdarg.h>

__attribute__((visibility("default")))
void nnew$MODString(String *string) {
  string->data = alloc$MODmem(1);
  string->size = 0;

  if (!string->data) {
    fprintf(stderr, "Could not create String\n");
    return;
  }

  string->mem_busy = true;
}

__attribute__((visibility("default")))
String *new$MODString() {
  String *string = alloc$MODmem(sizeof(String));
  string->data = alloc$MODmem(1);
  string->size = 0;

  if (!string->data) {
    fprintf(stderr, "Could not create String\n");
    return NULL;
  }

  string->mem_busy = true;

  return string;
}

__attribute__((visibility("default")))
String *from$MODString(const char *src) {
  String *string = new$MODString();

  string->data = realloc$MODmem(string->data, len$MODstring(src) + 1);
  string->size = len$MODstring(src);

  if (!string->data) {
    fprintf(stderr, "Could not create String\n");
    return NULL;
  }

  cpy$MODstring(string->data, src);
  string->mem_busy = true;

  return string;
}

__attribute__((visibility("default")))
void set$MODString(String *string, const char *src) {
  if (!string->data || !string->mem_busy || !src) return;

  strcpy(string->data, "");
  string->data = realloc$MODmem(string->data, strlen(src));

  if (!string->data) {
    fprintf(stderr, "Could not realloc$MODmemate memory for String\n");
    return;
  }

  strcpy(string->data, src);
}

__attribute__((visibility("default")))
String *clone$MODString(String *string) {
  if (!string->data || !string->mem_busy) {
    fprintf(stderr, "Could not clone string\n");
    return NULL;
  }

  return from$MODString(string->data);
}

__attribute__((visibility("default")))
void clear$MODString(String *string) {
  if (!string->data || !string->mem_busy) return;

  strcpy(string->data, "");
  string->size = 0;
}

__attribute__((visibility("default")))
int find$MODString(String *s1, const char *s2) {
  int n = s1->size;
  int m = strlen(s2);

  if (m > n) return -1;

  for (int i = 0; i <= n - m; i++) {
    int j = 0;
    for (; j < m; j++) {
      if (s1->data[i + j] != s2[j]) break;
    }
    if (j == m) return i;
  }

  return -1;
}

__attribute__((visibility("default")))
void cat$MODString(String *string, const char *s2) {
  if (!string->data || !string->mem_busy || !s2) return;

  int new_size = string->size + strlen(s2);
  new_size = new_size == 0 ? 1 : new_size;

  string->data = realloc$MODmem(string->data, new_size);
  if (!string->data) {
    fprintf(stderr, "Could not realloc$MODmemate memory for string concatenation\n");
    return;
  }

  strcpy(string->data + string->size, s2);
  string->size += strlen(s2);
}

__attribute__((visibility("default")))
void merge$MODString(String *s1, String *s2) {
  if (!s1->data || !s1->mem_busy || !s2->data || !s2->mem_busy) return;

  s1->data = realloc$MODmem(s1->data, s1->size + s2->size);
  if (!s1->data) {
    fprintf(stderr, "Could not realloc$MODmemate memory for string concatenation\n");
    return;
  }

  strcpy(s1->data + s1->size, s2->data);
  s1->size += s2->size;
  s1->data[s1->size] = '\0';
}

__attribute__((visibility("default")))
void cut_pos$MODString(String *string, int begin, int end) {
  if (!string->data || !string->mem_busy) return;

  int new_size = string->size - (end - begin);

  memmove(string->data + begin,
    string->data + end,
    string->size - end + 1);

  string->data = realloc$MODmem(string->data, new_size + 1);
  if (!string->data) {
    fprintf(stderr, "cut_string_pos: Could not realloc$MODmemate memory for string\n");
    return;
  }

  string->size = new_size;
  string->data[string->size] = '\0';
}

__attribute__((visibility("default")))
void cut_str$MODString(String *string, const char *substr) {
  int loc = find$MODString(string, substr);
  if (loc < 0) return;

  size_t len = strlen(substr);

  memmove(string->data + loc,
    string->data + loc + len,
    string->size - (loc + len));

  string->size -= len;
  string->data = realloc$MODmem(string->data, string->size + 1);
  string->data[string->size] = '\0';
}

__attribute__((visibility("default")))
char *substr_raw$MODString(String *string, int begin, int end) {
  if (begin > end) {
    int t = begin; begin = end; end = t;
  }

  if (begin < 0 || end > string->size) return NULL;

  int len = end - begin + 1;
  char *s = alloc$MODmem(len + 1);

  for (int i = 0; i < len; i++)
    s[i] = string->data[begin + i];

  s[len] = '\0';
  return s;
}

__attribute__((visibility("default")))
String *substr$MODString(String *string, int begin, int end) {
  if (begin > end) {
    int t = begin; begin = end; end = t;
  }

  if (begin < 0 || end > string->size)
    {}//return (String) { NULL, 0, false};

  int len = end - begin + 1;
  char *s = alloc$MODmem(len + 1);

  for (int i = 0; i < len; i++)
    s[i] = string->data[begin + i];

  s[len] = '\0';

  String *new_string = from$MODString(s);
  free$MODmem(s);
  return new_string;
}

void destroy$MODString(String *string);
__attribute__((visibility("default")))
void repl$MODString(String *string, const char *old, const char *new) {
  int loc = find$MODString(string, old);
  if (loc == -1) return;

  String *after = substr$MODString(string, loc + strlen(old), string->size);
  cut_pos$MODString(string, loc, string->size);
  cat$MODString(string, new);
  merge$MODString(string, after);

  destroy$MODString(after);
}

__attribute__((visibility("default")))
void fmt$MODString(String *string, ...) {
  va_list args;
  va_start(args, string);

  int len;
  char *s = strbuf$MODstd(string->data, &len, args);

  va_end(args);

  free$MODmem(string->data);
  string->size = len;
  string->data = alloc$MODmem(len + 1);
  strcpy(string->data, s);
  string->data[string->size] = '\0';
}

__attribute__((visibility("default")))
void newfmt$MODString(String *string, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int len;
  char *s = strbuf$MODstd(fmt, &len, args);

  va_end(args);

  free$MODmem(string->data);
  string->size = len;
  string->data = alloc$MODmem(len + 1);
  strcpy(string->data, s);
  string->data[string->size] = '\0';
}

__attribute__((visibility("default")))
void ask$MODString(String *string, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int len;
  char *s = strbuf$MODstd(fmt, &len, args);

  va_end(args);

  printf("%s", s);
  free$MODmem(s);

  char *ret = alloc$MODmem(120);
  if (fgets(ret, 120, stdin) != NULL) {
    ret[strcspn(ret, "\n")] = '\0';
  }

  ret = realloc$MODmem(ret, strlen(ret) + 1);

  free$MODmem(string->data);
  string->size = strlen(ret);
  string->data = ret;
  string->data[string->size] = '\0';
}

__attribute__((visibility("default")))
void destroy$MODString(String *string) {
  if (!string) return;
  
  if (string->mem_busy) {
    free$MODmem(string->data);
    string->size = 0;
    string->mem_busy = false;
  }

  free$MODmem(string);
}

__attribute__((visibility("default")))
void destroym$MODString(int n, ...) { // THIS FUNCION SHOULD NOT BE CALLED FROM OBJECT
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    String *string = va_arg(args, String *);
    destroy$MODString(string);
  }

  va_end(args);
}

__attribute__((visibility("default")))
char *to_str$MODString(String *string) {
  if (!string->data || !string->mem_busy)
    return "(null)";

  return string->data;
}

__attribute__((visibility("default")))
bool is_empty$MODString(String *string) {
  return !string->data || !string->mem_busy || string->size <= 0;
}

__attribute__((visibility("default")))
int to_int$MODString(String *string) {
  if (!is_valid_integer$MODstd(string->data)) return -1;
  return atoi(string->data);
}

__attribute__((visibility("default")))
float to_float$MODString(String *string) {
  if (!is_valid_float$MODstd(string->data)) return -1;
  return (float)atof(string->data);
}

__attribute__((visibility("default")))
double to_double$MODString(String *string) {
  if (!is_valid_double$MODstd(string->data)) return -1;
  return atof(string->data);
}

__attribute__((visibility("default")))
void upper$MODString(String *string) {
  if (is_empty$MODString(string)) return;

  for (size_t i = 0; string->data[i]; i++) {
    if (string->data[i] >= 'a' && string->data[i] <= 'z')
      string->data[i] -= 32;
  }
}

__attribute__((visibility("default")))
void lower$MODString(String *string) {
  if (is_empty$MODString(string)) return;

  for (size_t i = 0; string->data[i]; i++) {
    if (string->data[i] >= 'A' && string->data[i] <= 'Z')
      string->data[i] += 32;
  }
}

__attribute__((visibility("default")))
char ch$MODString(String *string, int index) {
  if (index < 0 || index >= string->size)
    return '?';

  return string->data[index];
}

__attribute__((visibility("default")))
int is_whitespace$MODString(String *string) {
  if (is_empty$MODString(string)) return false; // Does not apply for empty Strings

  for (int i = 0; i < string->size; i++) {
    if (string->data[i] != ' ') return false;
  }

  return true;
}

__attribute__((visibility("default")))
int cmp_str$MODString(String *self, const char *src) {
  if (self->size != len$MODstring(src)) return false;

  for (int i = 0; i < self->size; i++) {
    if (self->data[i] != src[i]) return false;
  }

  return true;
}

__attribute__((visibility("default")))
int size$MODString(String *self) {
  if (!self) return -1;
  return self->size;
}

/*__attribute__((visibility("default")))
int cmp$MODString(String *self, String *s2) { // This should not work in most of the cases, it needs to have the same memory direction
  printf("%d, %d\n", self->mem_busy, s2->mem_busy);
  if (self->data == s2->data && self->mem_busy == s2->mem_busy && self->size == s2->size) return true;
  return false;
}*/

// This should not be used frecuently, 'cause in yasos it is used pointers
__attribute__((visibility("default")))
size_t ssize$MODString() {
  return sizeof(String);
}