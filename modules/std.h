/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025-2026 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#ifndef STD_H
#define STD_H

void perror$MODstd(const char *fmt, ...);
void print$MODstd(const char *fmt, ...);
void println$MODstd(const char *fmt, ...);
void exit$MODstd(int status);
char *strbuf$MODstd(const char *fmt, int *len_r, va_list args);

int is_valid_integer$MODstd(const char *str);
int is_valid_float$MODstd(const char *str);
int is_valid_double$MODstd(const char *str);

#endif // STD_H