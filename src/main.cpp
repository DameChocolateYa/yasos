#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"
#include "global.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <string>
#include <cstring>
#include <vector>
#include <filesystem>
#include <unordered_set>

#undef __FILE__
#define __FILE__ "src/main.cpp"

namespace fs = std::filesystem;

std::ofstream file;

int main(int argc, char** argv) {
    bool keep_asm = false;
    std::vector<std::string> input_files;

    init_debug();

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--preserve-asm") == 0) {
            keep_asm = true;
        } else {
            input_files.emplace_back(argv[i]);
        }
    }

    if (input_files.empty()) {
        std::cerr << "Usage: beepc [--preserve-asm] <input1.bp> [input2.bp ...]\n";
        return EXIT_FAILURE;
    }

    initialize_func_map();
    initialize_func_ret_map();

    std::vector<std::string> object_files;
    std::vector<std::string> all_libraries;
    std::unordered_set<std::string> seen_libraries;

    for (size_t index = 0; index < input_files.size(); ++index) {
        const auto& filename = input_files[index];

        if (!filename.ends_with(".bp")) {
            std::cerr << "Skipping non-.bp file: " << filename << "\n";
            continue;
        }

        std::ifstream input_file(filename);
        if (!input_file.is_open()) {
            std::cerr << "Error: Could not open " << filename << "\n";
            return EXIT_FAILURE;
        }

        std::stringstream buffer;
        buffer << input_file.rdbuf();
        std::string source_code = buffer.str();

        Tokenizer tokenizer(std::move(source_code));
        std::vector<Token> tokens = tokenizer.tokenize();

        Parser parser(std::move(tokens));
        std::optional<NodeProg> program = parser.parse_prog();

        if (!program.has_value()) {
            std::cerr << "Parsing failed in file: " << filename << "\n";
            return EXIT_FAILURE;
        }

        // Name for generator: "main" for first file, base filename for others
        std::string base_name = fs::path(filename).stem().string();
        std::string gen_name = (index == 0) ? "main" : base_name;

        Generator generator(program.value(), gen_name);
        std::string asm_code = generator.gen_prog();

        std::string asm_file = base_name + ".asm";
        std::string obj_file = base_name + ".o";

        std::ofstream asm_out(asm_file);
        asm_out << asm_code;
        asm_out.close();

        std::string assemble_cmd = "nasm -f elf64 -o " + obj_file + " " + asm_file;
        if (system(assemble_cmd.c_str()) != 0) {
            std::cerr << "Assembly failed for: " << filename << "\n";
            return EXIT_FAILURE;
        }

        if (!keep_asm) {
            std::remove(asm_file.c_str());
        }

        object_files.push_back(obj_file);
        for (const auto& lib : generator.libraries) {
            if (seen_libraries.insert(lib).second) {  // true si no existía
                all_libraries.push_back(lib);
            }
        }

        LOG(__FILE__, "Compiled: " + filename);
    }

    std::string link_command = "g++ -o out ";
    for (const auto& obj : object_files) {
        link_command += obj + " ";
    }

    const std::string func_dir = "src/functions/asm/";
    for (const auto& lib : all_libraries) {
        link_command += func_dir + lib + ".o ";
    }

    link_command += "-no-pie";

    if (system(link_command.c_str()) != 0) {
        std::cerr << "Linking failed.\n";
        return EXIT_FAILURE;
    }

    LOG(__FILE__, "Build complete: Executable 'out'");
    terminate(EXIT_SUCCESS);
    return 0;
}
