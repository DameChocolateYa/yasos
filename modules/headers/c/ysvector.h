/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#ifndef YSVEC_H
#define YSVEC_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "ysstring.h"

/* =========================
   TYPE
   ========================= */

typedef struct YSVec {
    void *data;
    int size;
    int capacity;
    int elem_size;
    bool is_membusy;
} YSVec;

/* =========================
   INTERNAL (HIDDEN SYMBOLS)
   ========================= */

__attribute__((visibility("default")))
YSVec *new$MODVec(int elem_size);

__attribute__((visibility("default")))
void push$MODVec(YSVec *vec, void *elem);

__attribute__((visibility("default")))
YSVec *new_ptr$MODVec(int n, ...);

__attribute__((visibility("default")))
YSVec *new_int$MODVec(int n, ...);

__attribute__((visibility("default")))
YSVec *new_string$MODVec(int n, ...);

__attribute__((visibility("default")))
YSVec *new_double$MODVec(int n, ...);

__attribute__((visibility("default")))
YSVec *new_char$MODVec(int n, ...);

__attribute__((visibility("default")))
YSVec *new_vec$MODVec(int n, ...);

__attribute__((visibility("default")))
void pop$MODVec(YSVec *vec);

__attribute__((visibility("default")))
void erase_at$MODVec(YSVec *vec, int index);

__attribute__((visibility("default")))
void erase_all$MODVec(YSVec *vec);

__attribute__((visibility("default")))
void set$MODVec(YSVec *vec, int index, void *val);

__attribute__((visibility("default")))
void push_ptr$MODVec(YSVec *vec, void *ptr);

__attribute__((visibility("default")))
void push_int$MODVec(YSVec *vec, int val);

__attribute__((visibility("default")))
void push_string$MODVec(YSVec *vec, YSString *val);

__attribute__((visibility("default")))
void push_double$MODVec(YSVec *vec, double val);

__attribute__((visibility("default")))
void push_char$MODVec(YSVec *vec, char val);

__attribute__((visibility("default")))
void push_vec$MODVec(YSVec *vec, YSVec *val);

__attribute__((visibility("default")))
void pushm$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void pushm_int$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void pushm_string$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void pushm_double$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void pushm_vec$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void pushm_char$MODVec(YSVec *vec, int n, ...);

__attribute__((visibility("default")))
void* get$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
void *get_ptr$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
int get_int$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
YSString *get_string$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
double get_double$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
YSVec get_vec$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
char get_char$MODVec(YSVec *vector, int index);

__attribute__((visibility("default")))
void set_int$MODVec(YSVec* vector, int index, int val);

__attribute__((visibility("default")))
void set_string$MODVec(YSVec* vector, int index, YSString *val);

__attribute__((visibility("default")))
void set_double$MODVec(YSVec* vector, int index, double val);

__attribute__((visibility("default")))
void set_char$MODVec(YSVec *vector, int index, char val);

__attribute__((visibility("default")))
void set_vec$MODVec(YSVec* vector, int index, YSVec *val);

__attribute__((visibility("default")))
void setm_int$MODVec(YSVec* vector, int n, ...);

__attribute__((visibility("default")))
void setm_string$MODVec(YSVec* vector, int n, ...);

__attribute__((visibility("default")))
void setm_double$MODVec(YSVec* vector, int n, ...);

__attribute__((visibility("default")))
void setm_char$MODVec(YSVec* vector, int n, ...);

__attribute__((visibility("default")))
void setm_vec$MODVec(YSVec* vector, int n, ...);

__attribute__((visibility("default")))
void destroy$MODVec(YSVec* vector);

__attribute__((visibility("default")))
void destroym$MODVec(int n, ...);

__attribute__((visibility("default")))
void disable$MODVec(YSVec* vec);

__attribute__((visibility("default")))
void enable$MODVec(YSVec* vec);

__attribute__((visibility("default")))
void cpy$MODVec(YSVec* dest, YSVec *src);

__attribute__((visibility("default")))
void cpy_and_push$MODVec(YSVec* dest, YSVec *src, int index);

__attribute__((visibility("default")))
void cpy_and_pushm$MODVec(YSVec* dest, YSVec *src, int n, ...);

__attribute__((visibility("default")))
void cpy_in$MODVec(YSVec* dest, int to, YSVec *src, int from);

__attribute__((visibility("default")))
void cpy_xelem_in$MODVec(YSVec* dest, YSVec *src, int n_par, ...);

__attribute__((visibility("default")))
YSVec *clone$MODVec(YSVec *og);

__attribute__((visibility("default")))
void *iterate$MODVec(YSVec *self, int *i);

__attribute__((visibility("default")))
int size$MODVec(YSVec *self);

__attribute__((visibility("default")))
int ssize$MODVec();

__attribute__((visibility("default")))
YSVec *abc$MODVec();

__attribute__((visibility("default")))
YSVec *ABC$MODVec();

__attribute__((visibility("default")))
YSVec *abcABC$MODVec();

__attribute__((visibility("default")))
YSVec *ABCabc$MODVec();

__attribute__((visibility("default")))
YSVec *AaBbCc$MODVec();

__attribute__((visibility("default")))
YSVec *aAbBcC$MODVec();

/* =========================
   PUBLIC WRAPPERS (STRICT 1:1)
   ========================= */

static inline YSVec *ysvec_new(int elem_size) {
    return new$MODVec(elem_size);
}

static inline void ysvec_push(YSVec *vec, void *elem) {
    push$MODVec(vec, elem);
}

static inline YSVec *ysvec_new_ptr(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_ptr$MODVec(n, args);
    va_end(args);
    return v;
}

static inline YSVec *ysvec_new_int(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_int$MODVec(n, args);
    va_end(args);
    return v;
}

static inline YSVec *ysvec_new_string(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_string$MODVec(n, args);
    va_end(args);
    return v;
}

static inline YSVec *ysvec_new_double(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_double$MODVec(n, args);
    va_end(args);
    return v;
}

static inline YSVec *ysvec_new_char(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_char$MODVec(n, args);
    va_end(args);
    return v;
}

static inline YSVec *ysvec_new_vec(int n, ...) {
    va_list args;
    va_start(args, n);
    YSVec *v = new_vec$MODVec(n, args);
    va_end(args);
    return v;
}

static inline void ysvec_pop(YSVec *v) {
    pop$MODVec(v);
}

static inline void ysvec_erase_at(YSVec *v, int i) {
    erase_at$MODVec(v, i);
}

static inline void ysvec_erase_all(YSVec *v) {
    erase_all$MODVec(v);
}

static inline void ysvec_set(YSVec *v, int i, void *val) {
    set$MODVec(v, i, val);
}

static inline void ysvec_push_int(YSVec *v, int x) {
    push_int$MODVec(v, x);
}

static inline void ysvec_push_char(YSVec *v, char x) {
    push_char$MODVec(v, x);
}

static inline void ysvec_push_double(YSVec *v, double x) {
    push_double$MODVec(v, x);
}

static inline void ysvec_push_string(YSVec *v, YSString *x) {
    push_string$MODVec(v, x);
}

static inline void ysvec_push_vec(YSVec *v, YSVec *x) {
    push_vec$MODVec(v, x);
}

static inline void ysvec_destroy(YSVec *v) {
    destroy$MODVec(v);
}

static inline void ysvec_disable(YSVec *v) {
    disable$MODVec(v);
}

static inline void ysvec_enable(YSVec *v) {
    enable$MODVec(v);
}

static inline void *ysvec_get(YSVec *v, int i) {
    return get$MODVec(v, i);
}

static inline int ysvec_size(YSVec *v) {
    return size$MODVec(v);
}

static inline int ysvec_ssize(void) {
    return ssize$MODVec();
}

#endif