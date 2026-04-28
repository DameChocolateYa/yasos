/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

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