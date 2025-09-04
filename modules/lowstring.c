#include <stdio.h>
#define strcat beep_strcat
#include <string.h>
#undef strcat
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

const char* STR_EMPTY = "";
const char* STR_WHITE = " ";
const char* STR_NULLT = "\0";

int len(const char* s1) {
    int len = (int)strlen(s1);
    return len;
}

char* strcat(const char* s1, const char* s2) {
    if (!s1 || !s2) return NULL;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char* result = (char*)malloc(len1 + len2 + 1);
    if (!result) return NULL;

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2);
    result[len1 + len2] = '\0';

    return result;
}

void bufcat(char** s1, const char* s2) {
  if (!s1 || !s2) return;

  size_t len1 = strlen(*s1);
  size_t len2 = strlen(s2);

  s1 = (char**)realloc(s1, len1 + len2 + 1);
  if (!s1) {
    return;
  }

  *s1 = strcat(*s1, s2);
}

char* strcut_index(const char* s1, int begin, int end) { 
    int len = strlen(s1);

    if (begin < 0) begin = 0;
    if (end > len) end = len;
    if (begin >= end) return strdup(s1);

    int new_len = len - (end - begin);
    char* result = (char*)malloc(new_len);
    if (!result) return NULL;

    strncpy(result, s1, begin);

    strcpy(result + begin, s1 + end);

    return result;
}

void bufcut_index(const char** s1, int begin, int end) {
    if (!s1) return;
    *s1 = strcut_index(*s1, begin, end);
}

char* strcut(const char* str, const char* sub) {
    const char* pos = strstr(str, sub);
    if (pos == NULL) {
        return strdup(str);
    }

    size_t len_before = pos - str;
    size_t len_sub = strlen(sub);
    size_t len_after = strlen(pos + len_sub);

    char* result = (char*)malloc(len_before + len_after + 1);
    if (!result) return NULL;

    strncpy(result, str, len_before);
    strcpy(result + len_before, pos + len_sub);
    return result;
}

void bufcut(char **str_ptr, const char *sub) {
    char *str = *str_ptr;
    char *pos = strstr(str, sub);
    if (!pos) return;

    size_t len_before = pos - str;
    size_t len_sub = strlen(sub);
    size_t len_after = strlen(pos + len_sub);

    char *nuevo = (char*)malloc(len_before + len_after + 1);
    if (!nuevo) return;

    strncpy(nuevo, str, len_before);
    strcpy(nuevo + len_before, pos + len_sub);

    free(str);
    *str_ptr = nuevo;
}

char* strsub(const char* s1, int begin, int end) {
    int len = strlen(s1);

    if (begin < 0) begin = 0;
    if (end > len) end = len;
    if (begin >= end) return strdup(s1);

    int sub_len = end - begin;
    char* result = (char*)malloc(sub_len);
    if (!result) return NULL;

    strncpy(result, s1 + begin, sub_len);

    return result;
}

void bufsub(const char** s1, int begin, int end) {
  if (!s1) return;
  *s1 = strsub(*s1, begin, end);
}

int stoint(const char* s1) {
    return atoi(s1);
}

float stdb(const char* s1) {
    return atof(s1);
}

char* itostr(const int n) {
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

    char* res = (char*)malloc(len);
    if (!res) return NULL;

    for (int i = 0; i < len; ++i) {
        res[i] = buf[i];
    }

    return res;
}

char* db_to_str(double n, int decimals) {
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

    char* res = (char*)malloc(len);
    if (!res) return NULL;

    for (int i = 0; i < len; i++) {
        res[i] = buf[i];
    }

    return res;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return *s1 == *s2;  // Ambos deben llegar al final ('\0') al mismo tiempo
}

char* dig_to_abc(const int n) {
    if (n >= 0 && n < 26) {
        char* res = (char*)malloc(2);  // 1 para la letra, 1 para '\0'
        if (res == NULL) return NULL; // Verificación de malloc
        res[0] = 'A' + n;
        res[1] = '\0';
        return res;
    } else {
        char* res = (char*)malloc(2);
        if (res == NULL) return NULL;
        res[0] = '?';
        res[1] = '\0';
        return res;
    }
}

bool strempty(const char* s) {
  bool empty = true;
  for (int i = 0; i < strlen(s); ++i) {
    if (s[i] != '\0') empty = false;
  }

  return empty;
}

bool is_whitespace(const char* s) {
  bool is_whitespace = true;
  for (int i = 0; i < strlen(s); ++i) {
    if (s[1] != ' ') is_whitespace = false;
  }

  return is_whitespace;
}

char* trim(const char* s) {
  if (s == NULL) return NULL;

  const char* begin = s;
  const char* end;

  while (*begin && isspace((unsigned char)*begin)) {
    ++begin;
  }

  if (*begin == '\0') {
    char* empty = (char*)malloc(1);
    if (empty) empty[0] = '\0';
    return empty;
  }

  end = begin + strlen(begin) - 1;
  while (end > begin && isspace((unsigned char)*end)) {
    --end;
  }

  size_t len = end - begin + 1;

  char* new = (char*)malloc(len);
  if (!new) return NULL;
  strncpy(new, begin, len);

  return new;
}

void buftrim(const char** buf) {
  if (!buf) return;
  *buf = trim(*buf);
}

char* strreplace(const char* s, const char* old_sub, const char* new_sub) {
  if (!s || !old_sub || !new_sub) return NULL;

  size_t old_len = strlen(old_sub);
  size_t new_len = strlen(new_sub);
  size_t count = 0;

  const char* tmp = s;
  while ((tmp = strstr(tmp, old_sub))) {
    ++count;
    tmp += old_len;
  }

  size_t result_len = strlen(s) + count * (new_len - old_len);

  char* result = (char*)malloc(result_len);
  if (!result) return NULL;

  char* dest = result;
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

void bufreplace(const char** buf, const char* old_sub, const char* new_sub) {
  if (!buf) return;
  *buf = strreplace(*buf, old_sub, new_sub);
}

int strcontains(const char* str, const char* substring) {
  if (strstr(str, substring) != NULL) return 1;
  else return 0;
}

int strcontains_case(const char* str, const char* substring) {
  if (strcasestr(str, substring) != NULL) return 1;
  else return 0;
}

int strfind(const char* str, const char* substring) {
  char* pos = (char*)strstr(str, substring);
  if (pos == NULL) return -1;
  return(int)(pos - str);
}
