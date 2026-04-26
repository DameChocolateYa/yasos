#ifndef CUTYPE_H
#define CUTYPE_H

#include <stdlib.h>

typedef enum {
  Int, StringT, Float, Double
} CType;

typedef enum {
  IntS = 4,
  StringS = 40, // From CString
  FloatS = 6,
  DoubleS = 8,
  PtrS = 8,
} CTypeSize;

extern size_t get_cutype_size(CType type);

#endif