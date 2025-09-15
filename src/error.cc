#include "error.hh"
#include "global.hh"
#include <string>

std::unordered_map<ErrType, std::string> error_types;
std::unordered_map<ErrType, std::string> warning_types;

void add_error(const std::string& msg, int line, ErrType type) {
    static std::string last_msg = "";
    static int msg_repeated_counter = 0;

    if (msg_repeated_counter > 5) {
      std::exit(1);
    }

    if (last_msg == msg) {
      ++msg_repeated_counter;
      return;
    }

    last_msg = msg;

    if (type != ErrType::DefErr && error_types.contains(type)) return;
    std::cerr << "\033[1;31mError\033[0m in file " << current_source_file << " at line " << std::to_string(line) << ": " << msg << "\n";
    compiled_successfully = false;
    error_types.insert({type, msg});
}

void add_warning(const std::string& msg, int line, WarnType type) {
    std::cerr << "\033[1;35mWarning\033[0m in file " << current_source_file << " at line " << std::to_string(line) << ": " << msg << "\n";
}
