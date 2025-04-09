#pragma once

#include <iostream>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
    private:
        [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
            if (m_index + ahead >= m_src.length()) {
                return {};
                //minute 13 part 2
            }
            else {
                return m_src.at(m_index);
            }
        }

        const std::string m_src;
        int m_index;

    public:
        inline Tokenizer(const std::string& src) : m_src(std::move(src)) {
        }

        inline std::vector<Token> tokenize(const std::string& str) {
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
}
