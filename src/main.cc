#include "error.hh"
#include "generation.hh"
#include "global.hh"
#include "lexer.hh"
#include "parser.hh"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#define __YASOS_ORIGINAL_COMPILER_DEV__

const std::string username = getlogin();

namespace fs = std::filesystem;

std::ofstream file;

std::string current_source_file = "";
int current_line = 1;
int compiled_successfully = true;

int debug_mode_enabled = false;
std::string func_dir = "/usr/lib/yslib";

int main(int argc, char **argv) {
#ifdef __DEBUG__
  debug_mode_enabled = true;
  func_dir = "src/functions";
#endif

#ifdef __linux__
  m_preprocessor_bool.insert("__LINUX__");
#elif __WIN32
  m_preprocessor_bool.insert("__WIN__");
#ifdef __WIN64
  m_preprocessor_bool.insert("__WIN64__");
#endif
#elif __APPLE__
  m_preprocessor_bool.insert("__APPLE__");
#ifdef TARGET_OS_MAC
  m_preprocessor_bool.insert("__MACOS__");
#elif TARGET_OS_IPHONE("__IOS__");
#endif
#endif

#if defined(__i386__)
  m_preprocessor_bool.insert("__i386__");
#elif defined(__x86_64__) || defined(_M_X64)
  m_preprocessor_bool.insert("__X86_64__");
#endif

#if defined(__arm__)
  m_preprocessor_bool.insert("__arm__");
#elif defined(__aarch64__)
  m_preprocessor_bool.insert("__aarch64__");
#endif

#if defined(__ppc64__) || defined(_M_PPC)
  m_preprocessor_bool.insert("__ppc64__");
#elif defined(__riscv)
  m_preprocessor_bool.insert("_M_PPC");
  m_preprocessor_bool.insert("__RICV_V__");
#endif

#if INTPTR_MAX == INT64_MAX
  m_preprocessor_bool.insert("__64BYTES__");
#elif INTPTR_MAX == INT32_MAX
  m_preprocessor_bool.insert("__32BYTES__");
#else
#error "Architecture not compatible"
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  m_preprocessor_bool.insert("__ORDER_LITTLE_ENDIAN__");
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  m_preprocessor_bool.insert("__ORDER_BIG_ENDIAN__");
#endif

  m_preprocessor_bool.insert("__YASOS_ORIGINAL_COMPILER__");

  m_preprocessor_bool.insert("__YASOS_ORIGINAL_COMPILER_VERSION__");
  #if __YASOS_ORIGINAL_COMPILER_BETA__
  m_preprocessor_bool.insert("__YASOS_ORIGINAL_COMPILER_BETA__");
  #elif defined(__YASOS_ORIGINAL_COMPILER_DEV__)
  m_preprocessor_bool.insert("__YASOS_ORIGINAL_COMPILER_DEV__");
  #elif defined(__YASOS_ORIGINAL_COMPILER_TESTING__)
  #endif
  m_preprocessor.insert({"__YASOS_ORIGINAL_COMPILER_VERSION__", {Token {.type = TokenType::float_lit, .value = "0.0"}}});

  const std::string cache_directory = "/home/" + username + "/.cache/yasos/";
  if (system(("test -d " + cache_directory).c_str()) != 0) {
    if (system(("mkdir " + cache_directory).c_str()) != 0) {
      add_warning("Could not create cache directory", -1);
    }
  }

  bool keep_asm = false;
  bool keep_ll = false;
  bool generate_asm_only = false;  // -S
  bool compile_only = false;       // -c
  bool generate_shared = false;    // -shared
  std::string output_file = "out"; // -o
  bool generate_executable = false;
  bool print_link_command = false;
  std::vector<std::string> input_files;

  init_debug();

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-o" && i + 1 < argc) {
      output_file = argv[++i];
      generate_executable = true;
    } else if (arg == "-S") {
      generate_asm_only = true;
    } else if (arg == "-c") {
      compile_only = true;
    } else if (arg == "-shared") {
      generate_shared = true;
    } else if (arg == "--preserve-asm") {
      keep_asm = true;
    } else if (arg == "--preserve-ll") {
      keep_ll = true;
    } else if (arg == "--print-link-command") {
      print_link_command = true;
    } else {
      input_files.emplace_back(arg);
    }
  }

  if (input_files.empty()) {
    std::cerr << "Usage: yasos [-o output] [-S|-c|-shared] [--preserve-asm] "
                 "<input1.ys> [input2.ys ...]\n";
    return EXIT_FAILURE;
  }

  std::vector<std::string> object_files;
  std::vector<std::string> all_libraries;
  std::vector<std::string> all_libpaths;
  std::unordered_set<std::string> seen_libraries;
  std::unordered_set<std::string> seen_libpaths;

  for (size_t index = 0; index < input_files.size(); ++index) {
    const auto &filename = input_files[index];

    if (!filename.ends_with(".ys")) {
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

    Lexer lexer(std::move(source_code));
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeProg> program = parser.parse_prog();

    if (!program.has_value()) {
      std::cerr << "Parsing failed in file: " << filename << "\n";
      return EXIT_FAILURE;
    }

    std::string base_name = fs::path(filename).stem().string();
    // std::string gen_name = (index == 0) ? "main" : base_name;
    std::string gen_name = base_name;

    Generator generator(program.value(), gen_name, std::move(TheModule));
    generator.gen_prog();
    std::string ll_file = base_name + ".ll";
    std::error_code EC;
    llvm::raw_fd_ostream ll_out(ll_file, EC);
    generator.ModModule->print(ll_out, nullptr);
    ll_out.close();

    std::string asm_file = base_name + ".s";
    if (!generate_asm_only)
      asm_file = base_name + ".s";
    std::string obj_file = base_name + ".o";

    std::string llc_cmd =
        "llc -relocation-model=pic -filetype=asm " + ll_file + " -o " + asm_file;
    if (system(llc_cmd.c_str()) != 0) {
      std::cerr << "LLC failed for: " << filename << "\n";
      return EXIT_FAILURE;
    }

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

    if (!keep_ll) {
      const std::string command_rm_ll = "rm " + ll_file;
      system(command_rm_ll.c_str());
    }

    if (generate_asm_only) {
      continue;
    }

    for (const auto &libp : generator.libpaths) {
      if (seen_libpaths.insert(libp).second) {
        all_libpaths.push_back(libp);
      }
    }

    for (const auto &lib : generator.libraries) {
      if (seen_libraries.insert(lib).second) {
        all_libraries.push_back(lib);
      }
    }

    LOG(__FILE__, "Compiled: " + filename);
    current_line = 1;
  }

  if (generate_asm_only || compile_only) {
    LOG(__FILE__, "Compilation finished without linking.");
    return EXIT_SUCCESS;
  }

  std::string link_command;

  if (generate_executable) {
    link_command = "g++ -g -no-pie -o " + output_file + " ";
  } else {
    link_command = "g++ -g -no-pie ";
  }

  if (generate_shared) {
    link_command += "-shared ";
  }

  for (const auto &obj : object_files) {
    link_command += obj + " ";
  }

  for (const auto &obj : modules) {
    link_command += obj + " ";
  }

  link_command += "-L" + func_dir + " ";
  for (const auto &libp : all_libpaths) {
    link_command += "-L" + libp + " ";
  }
  for (const auto &lib : all_libraries) {
    link_command += "-l" + lib + " ";
  }

  link_command += "-L/usr/lib/yslib -lys -O2 -Wl,-rpath=" + func_dir;
  if (print_link_command) std::cerr << "LINK COMMAND: " << link_command << "\n";

  if (!compiled_successfully) {
    std::cerr << "\nErrors in compilation\n";
    return EXIT_FAILURE;
  }

  if (generate_executable) {
    if (system(link_command.c_str()) != 0) {
      add_error("Linking failed", -1);
      return EXIT_FAILURE;
    }

    for (const auto &obj : object_files) {
      if (system(("rm " + obj).c_str()) != 0) {
        add_warning("Could not delete objects files", -1);
        break;
      }
    }
  }

  LOG(__FILE__, "Build complete: Output '" + output_file + "'");
  terminate(EXIT_SUCCESS);
  return 0;
}
