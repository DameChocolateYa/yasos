#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int len(const char* s1) {
    int len = (int)strlen(s1);
    return len;
}

char* cat(const char* s1, const char* s2) {
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

int stoint(const char* s1) {
    return atoi(s1);
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return *s1 == *s2;  // Ambos deben llegar al final ('\0') al mismo tiempo
}
