#include "filesys.hh"

#include <filesystem>

namespace fs = std::filesystem;

fs::path seek_for_file(Generator *gen, const std::string &relative_path) {
    for (const auto &dir : gen->include_directories) {
        fs::path candidate = fs::path(dir) / relative_path;

        if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
            return fs::canonical(candidate);
        }
    }

    return {};
}