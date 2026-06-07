/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

typedef struct {
  char *data;
  int size;
  bool mem_busy;  
} String;

int len$MODstring(const char* s);
void cpy$MODstring(char *dest, const char *src);

String *new$MODString();
String *from$MODString(const char *src);
String *clone$MODString(String *self);
char *to_str$MODString(String *s);
void destroy$MODString(String *s);

#endif
