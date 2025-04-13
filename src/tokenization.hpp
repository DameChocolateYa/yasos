#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    str_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    var,
    eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
    private:
        [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
            if (m_index + offset >= m_src.length()) {
                return {};
            }
            else {
                return m_src.at(m_index + offset);
            }
        }

        inline char consume() {
            return m_src.at(m_index++);
        }

        const std::string m_src;
        size_t m_index = 0;

    public:
        inline explicit Tokenizer(const std::string& src) : m_src(std::move(src)) {
        }

        inline std::vector<Token> tokenize() {
            std::vector<Token> tokens {};

            std::string buf = "";

            while(peek().has_value()) {
                if (std::isalpha(peek().value())) {
                    buf.push_back(consume());
                    while(peek().has_value() && std::isalnum(peek().value())) {
                        buf.push_back(consume());
                    }
                    if (buf == "exit") {
                        tokens.push_back({.type = TokenType::exit});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "set") {
                        tokens.push_back({.type = TokenType::var});
                        buf.clear();
                        continue;
                    }
                    else {
                        tokens.push_back({.type = TokenType::ident, .value = buf});
                        buf.clear();
                    }
                }
                else if (std::isdigit(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::int_lit, .value = buf});
                    buf.clear();
                }
                else if (peek().value() == '"') {
                    consume();
                    while (peek().has_value() && peek().value() != '"') {
                        buf.push_back(consume());
                    }
                    if (!peek().has_value() || peek().value() != '"') {
                        std::cerr << "Undeterminated string literal\n";
                        exit(EXIT_FAILURE);
                    }
                    consume();

                    tokens.push_back({.type = TokenType::str_lit, .value = buf});
                    buf.clear();
                }
                else if (peek().value() == '(') {
                    consume();
                    tokens.push_back({.type = TokenType::open_paren});
                    continue;
                }
                else if (peek().value() == ')') {
                    consume();
                    tokens.push_back({.type = TokenType::close_paren});
                    continue;
                }
                else if (peek().value() == ';') {
                    consume();
                    tokens.push_back({.type = TokenType::semi});
                    continue;
                }
                else if (peek().value() == '=') {
                    consume();
                    tokens.push_back({.type = TokenType::eq});
                    continue;
                }
                else if (std::isspace(peek().value())) {
                    consume();
                    continue;
                }
                else {
                    std::cout << "AH\n";
                    exit(EXIT_FAILURE);
                }
            }
            m_index = 0;
            return tokens;
        }
};
