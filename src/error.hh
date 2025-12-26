/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#pragma once

#include <iostream>
#include <unordered_map>

enum ErrType {
  DefErr,
  WrongAndOr,
};

enum WarnType {
  DefWarn,
};

extern std::unordered_map<ErrType, std::string> error_types;
extern std::unordered_map<ErrType, std::string> warning_types;

void add_error(const std::string &msg, int line = -1, ErrType type = ErrType::DefErr);
void add_warning(const std::string &msg, int line = -1,
                 WarnType type = WarnType::DefWarn);
