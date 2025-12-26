/*
 * yasos - a compiler for yasos language
 * Copyright (c) 2025 DameChocolateYa
 * Licensed under the BSD 3-Clause License.
 * See LICENSE file in the project root for full license text.
*/

#pragma once

#ifndef __linux__
#error Only linux supported
#endif

#include <fstream>
#include <iostream>
#include <vector>

#define __LOG_FILE__ "log.txt"

enum ArgType {
  Void = -1,
  Integer = 0,
  Float = 1,
  String = 2,
  None = 3,
  Other = 4,
  List = 5,
  CustomIdent = 6,
  Any = 7,
  Ptr = 8,

  NoArg = 9,
  NxtUndefNum = 10,
};

extern std::ofstream file;
extern int debug_mode_enabled;
extern std::string current_source_file;
extern int current_line;
extern int compiled_successfully;
extern std::vector<std::string> modules;

inline void init_debug() {
  if (!debug_mode_enabled)
    return;
  file = std::ofstream(__LOG_FILE__);
  if (!file.is_open()) {
    std::cerr << "Error trying to open debug file\n";
    exit(EXIT_FAILURE);
  }
  file << "[DEBUG STARTED]\n\n";
}

inline void LOG(const std::string &source_file, const std::string &msg) {
  if (!debug_mode_enabled)
    return;
  file << "[" << source_file << "] " << msg << "\n";
}

inline void terminate(int status) {
  if (debug_mode_enabled) {
    file << "\n[DEBUG FINISHED | " << (status == 0 ? "SUCCESS" : "FAILURE") << "]\n";
    file.close();
  }
  exit(status);
}
