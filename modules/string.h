/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#ifndef STRING_h
#define STRING_H

typedef struct {
  char *data;
  int size;
  int mem_busy;  
} String;

int len$MODstring(const char* s);
void cpy$MODstring(char *dest, const char *src);

String new$MODString();
String from$MODString(const char *src);
void destroy$MODString(String *s);

#endif