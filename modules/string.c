#include <stdio.h>
#define strcat beep_strcat
#include <string.h>
#undef strcat
#include <stdlib.h>
#include <math.h>

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

char* strcut(const char* s1, int begin, int end) { 
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

int stoint(const char* s1) {
    return atoi(s1);
}

float stofl(const char* s1) {
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

char* ftostr(double n, int decimals) {
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

char* digtoabc(const int n) {
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
