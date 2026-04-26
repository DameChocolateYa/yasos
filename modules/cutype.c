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