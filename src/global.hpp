#pragma once

#ifndef __linux__
    #error Only linux supported
#endif

#include <iostream>
#include <fstream>
#include <vector>

#define __LOG_FILE__ "log.txt"

enum ArgType {
    NoArg,
    None,
    Integer,
    String,
    Float,
    OptionalInteger,
    OptionalString,
    OptionalFloat,
    VariableValue,
    NxtUndefNum
};

extern std::ofstream file;
extern int debug_mode_enabled;
extern std::string current_source_file;
extern int current_line;
extern int compiled_successfully;
extern std::vector<std::string> modules;

inline void init_debug() {
    if (!debug_mode_enabled) return;
    file = std::ofstream(__LOG_FILE__);
    if (!file.is_open()) {
        std::cerr << "Error trying to open debug file\n";
        exit(EXIT_FAILURE);
    }
    file << "[DEBUG STARTED]\n\n";
}

inline void LOG(const std::string& source_file, const std::string& msg) {
    if (!debug_mode_enabled) return;
    file << "[" << source_file << "] " << msg << "\n";
}

inline void terminate(int status) {
    if (debug_mode_enabled) {
        file << "\n[DEBUG FINISHED | " << (status == 0 ? "SUCCESS" : "FAILURE") << "]\n";
        file.close();
    }
    exit(status);
}
