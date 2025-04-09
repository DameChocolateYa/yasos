#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens {};

    std::string buf = "";
    for (int i = 0; i < str.length(); ++i) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buf.push_back(c);
            ++i;
            while (std::isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                ++i;
            }
            --i;

            if (buf == "exit") {
                tokens.push_back({.type = TokenType::exit});
                buf.clear();
                continue;
            }
        }
        else if (std::isdigit(c)) {
            buf.push_back(c);
            ++i;
            while (std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                ++i;
            }
            --i;
            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
        }
        else if (c == ';') {
            tokens.push_back({.type = TokenType::semi});
        }
        else if (std::isspace(c)) continue;
        else {
            std::cerr << "SHIT!\n";
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "     mov rax, 60\n";
                    output << "     mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "     syscall\n";
                }
            }
        }
    }
    return output.str();
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

    std::vector<Token> tokens = tokenize(contents);
    std::cout << tokens_to_asm(tokens) << "\n";
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
    return 0;
}
