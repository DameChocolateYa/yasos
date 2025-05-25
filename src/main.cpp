#include "error.hpp"
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

std::string current_source_file = "";
int current_line = 1;
int compiled_successfully = true;

int debug_mode_enabled = false;
std::string func_dir = "/usr/lib/beeplib";

int main(int argc, char** argv) {
#ifdef __DEBUG__
    debug_mode_enabled = true;
    func_dir = "src/functions";
#endif

    bool keep_asm = false;
    bool generate_asm_only = false;  // -S
    bool compile_only = false;       // -c
    bool generate_shared = false;    // -shared
    std::string output_file = "out"; // -o
    std::vector<std::string> input_files;

    init_debug();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-S") {
            generate_asm_only = true;
        } else if (arg == "-c") {
            compile_only = true;
        } else if (arg == "-shared") {
            generate_shared = true;
        } else if (arg == "--preserve-asm") {
            keep_asm = true;
        } else {
            input_files.emplace_back(arg);
        }
    }

    if (input_files.empty()) {
        std::cerr << "Usage: beepc [-o output] [-S|-c|-shared] [--preserve-asm] <input1.bp> [input2.bp ...]\n";
        return EXIT_FAILURE;
    }

    initialize_func_map();
    initialize_func_ret_map();
    initialize_str_property_map();
    initialize_str_ret_property_map();

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

        std::stringstream full_source;
        std::string line;

        current_source_file = filename;

        while (std::getline(input_file, line)) {
            full_source << line << '\n';
            ++current_line;
        }

        std::string source_code = full_source.str();

        Tokenizer tokenizer(std::move(source_code));
        std::vector<Token> tokens = tokenizer.tokenize();

        Parser parser(std::move(tokens));
        std::optional<NodeProg> program = parser.parse_prog();

        if (!program.has_value()) {
            std::cerr << "Parsing failed in file: " << filename << "\n";
            return EXIT_FAILURE;
        }

        std::string base_name = fs::path(filename).stem().string();
        //std::string gen_name = (index == 0) ? "main" : base_name;
        std::string gen_name = base_name;

        Generator generator(program.value(), gen_name);
        std::string asm_code = generator.gen_prog();

        std::string asm_file = base_name + ".s";
        std::string obj_file = base_name + ".o";

        std::ofstream asm_out(asm_file);
        asm_out << asm_code;
        asm_out.close();

        if (!generate_asm_only && compiled_successfully) {
            std::string assemble_cmd = "as -o " + obj_file + " " + asm_file;
            if (system(assemble_cmd.c_str()) != 0) {
                std::cerr << "Assembly failed for: " << filename << "\n";
                return EXIT_FAILURE;
            }

            if (!keep_asm) {
                std::remove(asm_file.c_str());
            }

            object_files.push_back(obj_file);
        }

        if (generate_asm_only) {
            continue;
        }

        for (const auto& lib : generator.libraries) {
            if (seen_libraries.insert(lib).second) {
                all_libraries.push_back(lib);
            }
        }

        LOG(__FILE__, "Compiled: " + filename);
        current_line = 1;
    }

    if (!compiled_successfully) {
        std::cerr << "\nErrors in compilation\n";
        return EXIT_FAILURE;
    }

    if (generate_asm_only || compile_only) {
        LOG(__FILE__, "Compilation finished without linking.");
        return EXIT_SUCCESS;
    }

    std::string link_command;

    if (generate_shared) {
        link_command = "g++ -shared -o " + output_file + " ";
    } else {
        link_command = "g++ -o " + output_file + " ";
    }

    for (const auto& obj : object_files) {
        link_command += obj + " ";
    }

    for (const auto& obj : modules) {
        link_command += obj + " ";
    }

    link_command += "-L" + func_dir + " ";
    for (const auto& lib : all_libraries) {
        link_command += "-l" + lib + " ";
    }

    link_command += "-Wl,-rpath=" + func_dir;

    if (system(link_command.c_str()) != 0) {
        std::cerr << "Linking failed.\n";
        return EXIT_FAILURE;
    }

    LOG(__FILE__, "Build complete: Output '" + output_file + "'");
    terminate(EXIT_SUCCESS);
    return 0;
}
