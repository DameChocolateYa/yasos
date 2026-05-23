/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cutype.h"
#include "mem.h"
#include <stdarg.h>
#include "vector.h"

__attribute__((visibility("default")))
Vec *new$MODVec(int elem_size) {
  Vec *self = (Vec *)alloc$MODmem(sizeof(Vec));
  
  self->data = alloc$MODmem(elem_size * 2);
  if (!self->data) {
    fprintf(stderr, "Vec::new: could not allocate memory for data\n");
    return self;
  }

  self->size = 0;
  self->capacity = 2;
  self->is_membusy = true;
  self->elem_size = elem_size;

  return self;
}

__attribute__((visibility("default")))
void push$MODVec(Vec *vec, void *elem) {
  if (!vec->data || !vec->is_membusy || !elem) {
    return;
  }

  if (vec->size + 1 >= vec->capacity) {
    vec->capacity *= 2;
    vec->data = realloc$MODmem(vec->data, vec->capacity * vec->elem_size);

    if (!vec->data) {
      fprintf(stderr, "cVec_push: Could not expand memory for push\n");
      return;
    }
  }

  memcpy((char *)vec->data + vec->size * vec->elem_size, elem, vec->elem_size);
  vec->size++;
}

__attribute__((visibility("default")))
Vec *new_ptr$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(void *));
  for (int i = 0; i < n; i++) {
    void *elem = va_arg(args, void *);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
Vec *new_int$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(int));
  for (int i = 0; i < n; i++) {
    int elem = va_arg(args, int);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
Vec *new_string$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(String *));
  for (int i = 0; i < n; i++) {
    String *elem = va_arg(args, String *);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
Vec *new_double$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(double));
  for (int i = 0; i < n; i++) {
    double elem = va_arg(args, double);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
Vec *new_char$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(char));
  for (int i = 0; i < n; i++) {
    char elem = (char)va_arg(args, int);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
Vec *new_vec$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  Vec *self = new$MODVec(sizeof(Vec *));
  for (int i = 0; i < n; i++) {
    Vec *elem = va_arg(args, Vec *);
    push$MODVec(self, &elem);
  }

  va_end(args);

  return self;
}

__attribute__((visibility("default")))
void pop$MODVec(Vec *vec) {
  if (!vec->data || !vec->is_membusy || vec->size <= 0)
    return;

  vec->size -= 1;
  memset((char *)vec->data + (vec->size) * vec->elem_size, 0xDD, vec->elem_size);
}

__attribute__((visibility("default")))
void erase_at$MODVec(Vec *vec, int index) {
  if (!vec->data || !vec->is_membusy) {
    return;
  }

  if (index < 0 || index >= vec->size) {
    fprintf(stderr, "cVec_erase: index out of range\n");
    return;
  }

  memmove((char *)vec->data + index * vec->elem_size,
    (char *)vec->data + (index + 1) * vec->elem_size,
    (vec->size - index - 1) * vec->elem_size);

  vec->size--;
}

__attribute__((visibility("default")))
void erase_all$MODVec(Vec *vec) {
  if (!vec->data || !vec->is_membusy) {
    return;
  }

  memset((char *)vec->data, 0xDD, (vec->size - 1) * vec->elem_size);
  vec->size = 0;
}

__attribute__((visibility("default")))
void set$MODVec(Vec *vec, int index, void *val) {
  if (!vec->data || !vec->is_membusy)
    return;

  if (index < 0 || index >= vec->size) {
    fprintf(stderr, "cVec_edit(generic): index out of range\n");
    return;
  }

  memcpy((char *)vec->data + index * vec->elem_size, val, vec->elem_size);
}

__attribute__((visibility("default")))
void push_ptr$MODVec(Vec *vec, void *ptr) {
  push$MODVec(vec, &ptr);
}

__attribute__((visibility("default")))
void push_int$MODVec(Vec *vec, int val) {
  push$MODVec(vec, &val);
}

__attribute__((visibility("default")))
void push_string$MODVec(Vec *vec, String *val) {
  push$MODVec(vec, &val);
}

__attribute__((visibility("default")))
void push_double$MODVec(Vec *vec, double val) {
  push$MODVec(vec, &val);
}

__attribute__((visibility("default")))
void push_char$MODVec(Vec *vec, char val) {
  push$MODVec(vec, &val);
}

__attribute__((visibility("default")))
void pushm$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    void *val = va_arg(args, void *);
    push$MODVec(vec, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void pushm_int$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int val = va_arg(args, int);
    push_int$MODVec(vec, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void pushm_string$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    String *val = va_arg(args, String*);
    push_string$MODVec(vec, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void pushm_double$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    double val = va_arg(args, double);
    push_double$MODVec(vec, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void pushm_vec$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    Vec *val = va_arg(args, Vec*);
    push$MODVec(vec, val);
  }

  va_end(args);
}
__attribute__((visibility("default")))
void pushm_char$MODVec(Vec *vec, int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    char c = (char)va_arg(args, int);
    push_char$MODVec(vec, c);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void* get$MODVec(Vec *vector, int index) {
  if (!vector->data || !vector->is_membusy) {
    return NULL;
  }

  if (index < 0 || index >= vector->size) {
    fprintf(stderr, "get$MODVec: index out of range\n");
    return NULL;
  }

  return (char*)vector->data + index * vector->elem_size;
}

__attribute__((visibility("default")))
void *get_ptr$MODVec(Vec *vector, int index) {
  void **ptr = (void**)get$MODVec(vector, index);
  return ptr ? *ptr : NULL;
}

__attribute__((visibility("default")))
int get_int$MODVec(Vec *vector, int index) {
  int* ptr = (int*)get$MODVec(vector, index);
  return ptr ? *ptr : 0;
}

__attribute__((visibility("default")))
String *get_string$MODVec(Vec *vector, int index) {
  String* ptr = *(String**)get$MODVec(vector, index);
  return ptr;
}

__attribute__((visibility("default")))
double get_double$MODVec(Vec *vector, int index) {
  double* ptr = (double*)get$MODVec(vector, index);
  return ptr ? *ptr : .0;
}

__attribute__((visibility("default")))
Vec get_vec$MODVec(Vec *vector, int index) {
  Vec* ptr = (Vec*)get$MODVec(vector, index);
  return ptr ? *ptr : (Vec) { NULL, 0, 0, 0, false };
}

__attribute((visibility("default")))
char get_char$MODVec(Vec *vector, int index) {
  char *ptr = (char *)get$MODVec(vector, index);
  return ptr ? *ptr : '\0';
}

__attribute__((visibility("default")))
void set_int$MODVec(Vec* vector, int index, int val) {
  set$MODVec(vector, index, &val);
}

__attribute__((visibility("default")))
void set_string$MODVec(Vec* vector, int index, String *val) {
  set$MODVec(vector, index, &val);
}

__attribute__((visibility("default")))
void set_double$MODVec(Vec* vector, int index, double val) {
  set$MODVec(vector, index, &val);
}

__attribute__((visibility("default")))
void set_char$MODVec(Vec *vector, int index, char val) {
  set$MODVec(vector, index, &val);
}

__attribute__((visibility("default")))
void set_vec$MODVec(Vec* vector, int index, Vec *val) {
  set$MODVec(vector, index, &val);
}

__attribute__((visibility("default")))
void setm_int$MODVec(Vec* vector, int n, ...) {
  if (!vector->data || !vector->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);
    int val = va_arg(args, int);

    if (index < 0 || index >= vector->size) {
      fprintf(stderr, "set$MODVecim: index out of range - omiting current element\n");
      continue;
    }

    set_int$MODVec(vector, index, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void setm_string$MODVec(Vec* vector, int n, ...) {
  if (!vector->data || !vector->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);
    String *val = va_arg(args, String *);

    if (index < 0 || index >= vector->size) {
      fprintf(stderr, "set$MODVecstringm: index out of range - omiting current element\n");
      continue;
    }

    set_string$MODVec(vector, index, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void setm_double$MODVec(Vec* vector, int n, ...) {
  if (!vector->data || !vector->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);
    double val = va_arg(args, double);

    if (index < 0 || index >= vector->size) {
      fprintf(stderr, "set$MODVecdm: index out of range - omiting current element\n");
      continue;
    }

    set_double$MODVec(vector, index, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void setm_char$MODVec(Vec* vector, int n, ...) {
  if (!vector->data || !vector->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);
    char val = (char)va_arg(args, int);

    if (index < 0 || index >= vector->size) {
      fprintf(stderr, "set$MODVecdm: index out of range - omiting current element\n");
      continue;
    }

    set_char$MODVec(vector, index, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void setm_vec$MODVec(Vec* vector, int n, ...) {
  if (!vector->data || !vector->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);
    Vec *val = va_arg(args, Vec*);

    if (index < 0 || index >= vector->size) {
      fprintf(stderr, "set$MODVecvecm: index out of range - omiting current element\n");
      continue;
    }

    set_vec$MODVec(vector, index, val);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void destroy$MODVec(Vec* vector) {
  if (!vector->data || !vector->is_membusy)
    return;

  free$MODmem(vector->data);
  vector->capacity = 0;
  vector->size = 0;
  vector->elem_size = 0;
  vector->is_membusy = false;

  free$MODmem(vector);
}

__attribute__((visibility("default")))
void destroym$MODVec(int n, ...) {
  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    Vec *vector = va_arg(args, Vec*);
    destroy$MODVec(vector);
  }

  va_end(args);
}

// It is possible that the programmer desires to disable it and manage by himself the memory
__attribute__((visibility("default")))
void disable$MODVec(Vec* vec) {
  vec->is_membusy = false;
}

// Just in case that the programmer regret about using the previous function ??
__attribute__((visibility("default")))
void enable$MODVec(Vec* vec) {
  if (!vec->data) return; // It makes no sense to activate membusy flag if there is not data

  vec->is_membusy = true;
}

__attribute__((visibility("default")))
void cpy$MODVec(Vec* dest, Vec *src) {
  if (src->data || !src->is_membusy || src->size <= 0 || src->elem_size <= 0 || src->capacity == 0)
    return;

  dest->data = (char *)alloc$MODmem(src->size + 1);
  if (!dest->data) {
    fprintf(stderr, "Vec_cpy: error allocating memory\n");
    return;
  }
  strcpy(dest->data, src->data);

  dest->size = src->size;
  dest->capacity = src->capacity;
  dest->elem_size = dest->elem_size;
  dest->is_membusy = true;
}

__attribute__((visibility("default")))
void cpy_and_push$MODVec(Vec* dest, Vec *src, int index) {
  if (dest->elem_size != src->elem_size) {
    fprintf(stderr, "Vec_cpy_and_push: vectors elem size are not equal\n");
  }

  if (!src->data || !dest->data || !src->is_membusy || !dest->is_membusy)
    return;

  if (index < 0 || index >= src->size) {
    fprintf(stderr, "Vec_cpy_and_push: index out of range\n");
    return;
  }

  void* elem = get$MODVec(src, index);
  push$MODVec(dest, elem);
}

__attribute__((visibility("default")))
void cpy_and_pushm$MODVec(Vec* dest, Vec *src, int n, ...) {
  if (dest->elem_size != src->elem_size) {
    fprintf(stderr, "Vec_cpy_and_pushm: vectors elem size are not equal\n");;
  }

  if (!src->data || !dest->data || !src->is_membusy || !dest->is_membusy)
    return;

  va_list args;
  va_start(args, n);

  for (int i = 0; i < n; i++) {
    int index = va_arg(args, int);

    if (index < 0 || index >= src->size) {
      fprintf(stderr, "Vec_cpy_and_pushm: index out of range - omiting current element\n");
      continue;
    }

    void* elem = get$MODVec(src, index);
    push$MODVec(dest, elem);
  }

  va_end(args);
}

__attribute__((visibility("default")))
void cpy_in$MODVec(Vec* dest, int to, Vec *src, int from) {
  if (dest->elem_size != src->elem_size) {
    fprintf(stderr, "Vec_cpy_in: vectors elem size are not equal\n");;
  }

  if (!src->data || !dest->data || !src->is_membusy || !dest->is_membusy)
    return;

  if (from < 0 || from >= src->size || to < 0) {
    fprintf(stderr, "Vec_cpy_in: index out of range\n");
    return;
  }

  void* elem = get$MODVec(src, from);

  if (to >= dest->size) {
    push$MODVec(dest, elem);
    return;
  }

  memcpy(dest->data + to * dest->elem_size, elem, dest->elem_size);
}

__attribute__((visibility("default")))
void cpy_xelem_in$MODVec(Vec* dest, Vec *src, int n_par, ...) {
  if (dest->elem_size != src->elem_size) {
    fprintf(stderr, "Vec_cpy_in: vectors elem size are not equal\n");;
  }

  if (!src->data || !dest->data || !src->is_membusy || !dest->is_membusy)
    return;

  va_list args;
  va_start(args, n_par);

  for (int i = 0; i < n_par; i++) {
    int from = va_arg(args, int);
    int to = va_arg(args, int);

    if (from < 0 || from >= src->size || to < 0) {
      fprintf(stderr, "Vec_cpy_in: index out of range\n");
      return;
    }

    void* elem = get$MODVec(src, from);

    if (to >= dest->size) {
      push$MODVec(dest, elem);
      return;
    }

    memcpy(dest->data + to * dest->elem_size, elem, dest->elem_size);
  }
}

__attribute__((visibility("default")))
Vec *clone$MODVec(Vec *og) {
  Vec *cloned = (Vec *)alloc$MODmem(sizeof(Vec));
  cloned->data = (char *)alloc$MODmem(og->elem_size * og->capacity);

  if (!cloned->data) {
    fprintf(stderr, "Vec::clone: Could not clone Vector (error allocating memory)\n");
    return cloned;
  }

  memcpy(cloned->data, og->data, og->capacity * og->elem_size);
  cloned->capacity = og->capacity;
  cloned->size = og->size;
  cloned->elem_size = og->elem_size;
  cloned->is_membusy = true;

  return cloned;
}

__attribute__((visibility("default")))
int size$MODVec(Vec *self) {
  return self->size;
}

__attribute__((visibility("default")))
int ssize$MODVec() {
  return sizeof(Vec);
}

__attribute__((visibility("default")))
Vec *abc$MODVec() {
  Vec *abc = new_char$MODVec(26, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z');
  return abc;
}

__attribute__((visibility("default")))
Vec *ABC$MODVec() {
  Vec *abc = new_char$MODVec(26, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z');
  return abc;
}

__attribute__((visibility("default")))
Vec *abcABC$MODVec() {
  Vec *abc = new_char$MODVec(52, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z');
  return abc;
}