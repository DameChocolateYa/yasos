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
#include <vector>

#undef __FILE__
#define __FILE__ "src/main.cpp"

int main(int argc, char** argv) {
    init_debug();
    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct usage is...\n";
        std::cerr << "beep++ <input.bp>\n";
        return EXIT_FAILURE;
    }

    std::string contents; {
        std::fstream input(argv[1], std::ios::in);
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        input.close();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));


    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << "Invalid Program\n";
        return EXIT_FAILURE;
    }

    Generator generator(prog.value());
    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog();
    }

    std::string link_command = "g++ -o out out.o ";

    if (system("nasm -f elf64 -o out.o out.asm ") != 0) {
        LOG(__FILE__, std::string("Failure compiling source file (").append(argv[1]) + ")");
        terminate(EXIT_FAILURE);
    }
    else {
        LOG(__FILE__, std::string("Success compilation of source file (").append(argv[1]) + ")");
    }

    const std::string func_dir = "src/functions/";

    for (int i = 0; i < generator.libraries.size(); ++i) {
        link_command.append(func_dir + generator.libraries[i] + ".o ");
    }
    link_command.append("-no-pie");

    system(link_command.c_str());
    //system("rm out.asm");

    terminate(EXIT_SUCCESS);
    return EXIT_SUCCESS;
    return 0;
}
