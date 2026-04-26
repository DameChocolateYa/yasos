#ifndef VECTOR_H
#define VECTOR_H

#include "string.h"

typedef struct {
  void *data;
  int size;
  int capacity;
  int elem_size;
  int is_membusy;
} Vec;

Vec new$MODVec(int elem_size);
void push_string$MODVec(Vec *vec, String val);
String get_string$MODVec(Vec *vec, size_t index);
void destroy$MODVec(Vec *vec);

#endif