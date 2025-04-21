#pragma once

#ifndef __linux__
    #error Only linux supported
#endif

#include <iostream>
#include <fstream>

#define __LOG_FILE__ "log.txt"

std::ofstream file;

inline void init_debug() {
    file = std::ofstream(__LOG_FILE__);
    if (!file.is_open()) {
        std::cerr << "Error trying to open debug file\n";
        exit(EXIT_FAILURE);
    }
    file << "[DEBUG STARTED]\n\n";
}

inline void LOG(const std::string& source_file, const std::string& msg) {
    file << "[" << source_file << "] " << msg << "\n";
}

inline void terminate(int status) {
    file << "\n[DEBUG FINISHED | " << (status == 0 ? "SUCCESS" : "FAILURE") << "]\n";
    file.close();
    exit(status);
}
