#include <string>
#include <filesystem>

#include "generation.hh"

inline std::string get_current_path() {
    return std::filesystem::current_path();
}
std::filesystem::path seek_for_file(Generator *gen, const std::string &relative_path);