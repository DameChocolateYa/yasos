#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

std::vector<Token> tokenize(const std::string& str) {
}

int main(int argc, char** argv) {
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

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    //system("rm out.asm");

    return EXIT_SUCCESS;
    return 0;
}
