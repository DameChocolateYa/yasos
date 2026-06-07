/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

// This header is just to let the C/C++ code interact with YASOS code

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *data;
    int size;
    bool is_membusy;
} YSString;

// str functions won't be included since they are obsoletes and it is more easy to manage the str manipulation in C/C**

__attribute__((visibility("default")))
void nnew$MODString(YSString *string);

__attribute__((visibility("default")))
YSString *new$MODString();

__attribute__((visibility("default")))
YSString *from$MODString(const char *src);

__attribute__((visibility("default")))
void set$MODString(YSString *string, const char *src);

__attribute__((visibility("default")))
YSString *clone$MODString(YSString *string);

__attribute__((visibility("default")))
char iterate$MODString(YSString *self, int *i);

__attribute__((visibility("default")))
void clear$MODString(YSString *string);

__attribute__((visibility("default")))
int find$MODString(YSString *s1, const char *s2);

__attribute__((visibility("default")))
void cat$MODString(YSString *string, const char *s2);

__attribute__((visibility("default")))
void cat_char$MODString(YSString *self, const char c);

__attribute__((visibility("default")))
void merge$MODString(YSString *s1, YSString *s2);

__attribute__((visibility("default")))
void cut_pos$MODString(YSString *string, int begin, int end);

__attribute__((visibility("default")))
void cut_str$MODString(YSString *string, const char *substr);

__attribute__((visibility("default")))
char *substr_raw$MODString(YSString *string, int begin, int end);

__attribute__((visibility("default")))
YSString *substr$MODString(YSString *string, int begin, int end);

__attribute__((visibility("default")))
void repl$MODString(YSString *string, const char *old, const char *neww);

__attribute__((visibility("default")))
void fmt$MODString(YSString *string, ...);

__attribute__((visibility("default")))
void newfmt$MODString(YSString *string, const char *fmt, ...);

__attribute__((visibility("default")))
void ask$MODString(YSString *string, const char *fmt, ...);

__attribute__((visibility("default")))
void destroy$MODString(YSString *string);

__attribute__((visibility("default")))
void destroym$MODString(int n, ...);

__attribute__((visibility("default")))
char *to_str$MODString(YSString *string);

__attribute__((visibility("default")))
bool is_empty$MODString(YSString *string);

__attribute__((visibility("default")))
int to_int$MODString(YSString *string);

__attribute__((visibility("default")))
float to_float$MODString(YSString *string);

__attribute__((visibility("default")))
double to_double$MODString(YSString *string);

__attribute__((visibility("default")))
void upper$MODString(YSString *string);

__attribute__((visibility("default")))
void lower$MODString(YSString *string);

__attribute__((visibility("default")))
void trim$MODString(YSString *self);

__attribute__((visibility("default")))
void reverse$MODString(YSString *self);

__attribute__((visibility("default")))
bool is_palindrome$MODString(YSString *self);

__attribute__((visibility("default")))
char ch$MODString(YSString *string, int index);

__attribute__((visibility("default")))
int is_whitespace$MODString(YSString *string);

__attribute__((visibility("default")))
int cmp_str$MODString(YSString *self, const char *src);

__attribute__((visibility("default")))
int size$MODString(YSString *self);

__attribute__((visibility("default")))
size_t ssize$MODString();

/* =========================
   PUBLIC WRAPPERS
   ========================= */

static inline void ysstring_nnew(YSString *string) {
    nnew$MODString(string);
}

static inline YSString *ysstring_new(void) {
    return new$MODString();
}

static inline YSString *ysstring_from(const char *src) {
    return from$MODString(src);
}

static inline void ysstring_set(YSString *string, const char *src) {
    set$MODString(string, src);
}

static inline YSString *ysstring_clone(YSString *string) {
    return clone$MODString(string);
}

static inline char ysstring_iterate(YSString *self, int *i) {
    return iterate$MODString(self, i);
}

static inline void ysstring_clear(YSString *string) {
    clear$MODString(string);
}

static inline int ysstring_find(YSString *s1, const char *s2) {
    return find$MODString(s1, s2);
}

static inline void ysstring_cat(YSString *string, const char *s2) {
    cat$MODString(string, s2);
}

static inline void ysstring_cat_char(YSString *self, const char c) {
    cat_char$MODString(self, c);
}

static inline void ysstring_merge(YSString *s1, YSString *s2) {
    merge$MODString(s1, s2);
}

static inline void ysstring_cut_pos(YSString *string, int begin, int end) {
    cut_pos$MODString(string, begin, end);
}

static inline void ysstring_cut_str(YSString *string, const char *substr) {
    cut_str$MODString(string, substr);
}

static inline char *ysstring_substr_raw(YSString *string, int begin, int end) {
    return substr_raw$MODString(string, begin, end);
}

static inline YSString *ysstring_substr(YSString *string, int begin, int end) {
    return substr$MODString(string, begin, end);
}

static inline void ysstring_repl(YSString *string, const char *old, const char *neww) {
    repl$MODString(string, old, neww);
}

static inline void ysstring_fmt(YSString *string, ...) {
    va_list args;
    va_start(args, string);
    fmt$MODString(string, args);
    va_end(args);
}

static inline void ysstring_newfmt(YSString *string, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    newfmt$MODString(string, fmt, args);
    va_end(args);
}

static inline void ysstring_ask(YSString *string, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ask$MODString(string, fmt, args);
    va_end(args);
}

static inline void ysstring_destroy(YSString *string) {
    destroy$MODString(string);
}

static inline void ysstring_destroym(int n, ...) {
    destroym$MODString(n);
}

static inline char *ysstring_to_str(YSString *string) {
    return to_str$MODString(string);
}

static inline bool ysstring_is_empty(YSString *string) {
    return is_empty$MODString(string);
}

static inline int ysstring_to_int(YSString *string) {
    return to_int$MODString(string);
}

static inline float ysstring_to_float(YSString *string) {
    return to_float$MODString(string);
}

static inline double ysstring_to_double(YSString *string) {
    return to_double$MODString(string);
}

static inline void ysstring_upper(YSString *string) {
    upper$MODString(string);
}

static inline void ysstring_lower(YSString *string) {
    lower$MODString(string);
}

static inline void ysstring_trim(YSString *self) {
    trim$MODString(self);
}

static inline void ysstring_reverse(YSString *self) {
    reverse$MODString(self);
}

static inline bool ysstring_is_palindrome(YSString *self) {
    return is_palindrome$MODString(self);
}

static inline char ysstring_ch(YSString *string, int index) {
    return ch$MODString(string, index);
}

static inline int ysstring_is_whitespace(YSString *string) {
    return is_whitespace$MODString(string);
}

static inline int ysstring_cmp_str(YSString *self, const char *src) {
    return cmp_str$MODString(self, src);
}

static inline int ysstring_size(YSString *self) {
    return size$MODString(self);
}

static inline size_t ysstring_ssize(void) {
    return ssize$MODString();
}
