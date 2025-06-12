#include "error.hpp"
#include "global.hpp"
#include <string>

void add_error(const std::string& msg, int line) {
    std::cerr << "\033[1;31mError\033[0m in file " << current_source_file << " at line " << std::to_string(line) << ": " << msg << "\n";
    compiled_successfully = false;
}

void add_warning(const std::string& msg, int line) {
    std::cerr << "\033[1;35mWarning\033[0m in file " << current_source_file << " at line " << std::to_string(line) << ": " << msg << "\n";
}
