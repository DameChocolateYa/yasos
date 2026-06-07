/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "string.h"
#include <stdbool.h>

typedef struct {
  void *data;
  int size;
  int capacity;
  int elem_size;
  bool is_membusy;
} Vec;

Vec *new$MODVec(int elem_size);
void push_int$MODVec(Vec *self, int n);
void push_string$MODVec(Vec *vec, String *val);
void push_vec$MODVec(Vec *self, Vec *vec);
int get_int$MODVec(Vec *self, int index);
String *get_string$MODVec(Vec *vec, int index);
void erase_all$MODVec(Vec *self);
void destroy$MODVec(Vec *vec);

#endif