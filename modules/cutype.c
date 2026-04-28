/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#include "cutype.h"
#include <stdlib.h>

size_t get_cutype_size(CType type) {
  switch (type) {
  case Int:
    return IntS;
  case StringT:
    return StringS;
  case Float:
    return FloatS;
  case Double:
    return DoubleS;
  default:
    return PtrS;
  }
}