#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>

enum class TokenType {
    int_lit,
    str_lit,
    semi,
    comma,
    open_paren,
    close_paren,
    ident,
    var,
    eq,
    str_type,
    int_type,
    dp,
    comment_begin,
    comment_end,
    include,
    use,
    l_arrow,
    r_arrow,
    l_key,
    r_key,
    plus, // 20
    minus,
    star,
    slash,
    plus_eq,
    minus_eq,
    star_eq,
    slash_eq,
    plusplus,
    minusminus
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
                    if (buf == "let") {
                        tokens.push_back({.type = TokenType::var});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "str") {
                        tokens.push_back({.type = TokenType::str_type});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "int") {
                        tokens.push_back({.type = TokenType::int_type});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "add") {
                        tokens.push_back({.type = TokenType::plus, .value="+"});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "include") {
                        tokens.push_back({.type = TokenType::include});
                        buf.clear();
                        continue;
                    }
                    else if (buf == "use") {
                        tokens.push_back({.type = TokenType::use});
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
                else if (peek().value() == ':') {
                    consume();
                    tokens.push_back({.type = TokenType::dp});
                    continue;
                }
                else if (peek().value() == ',') {
                    consume();
                    tokens.push_back({.type = TokenType::comma});
                    continue;
                }
                else if (peek().value() == '+' && peek(1).has_value() && peek(1).value() == '+') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::plusplus, .value="++"});
                    continue;
                }
                else if (peek().value() == '-' && peek(1).has_value() && peek(1).value() == '-') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::minusminus, .value="--"});
                    continue;
                }
                else if (peek().value() == '+' && peek(1).has_value() && peek(1).value() == '=') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::plus_eq, .value = "+="});
                    continue;
                }
                else if (peek().value() == '-' && peek(1).has_value() && peek(1).value() == '=') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::minus_eq, .value = "-="});
                    continue;
                }
                else if (peek().value() == '*' && peek(1).has_value() && peek(1).value() == '=') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::star_eq, .value = "*="});
                    continue;
                }
                else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '=') {
                    consume(); consume();
                    tokens.push_back({.type = TokenType::slash_eq, .value = "/="});
                    continue;
                }
                else if (peek().value() == '=') {
                    consume();
                    tokens.push_back({.type = TokenType::eq});
                    continue;
                }
                else if (peek().value() == '+') {
                    consume();
                    tokens.push_back({.type = TokenType::plus, .value="+"});
                    continue;
                }
                else if (peek().value() == '-') {
                    consume();
                    tokens.push_back({.type = TokenType::minus, .value="-"});
                    continue;
                }
                else if (peek().value() == '*') {
                    consume();
                    tokens.push_back({.type = TokenType::star, .value="*"});
                    continue;
                }
                else if (peek().value() == '/') {
                    consume();
                    tokens.push_back({.type = TokenType::slash, .value="/"});
                    continue;
                }
                else if (peek().value() == '!') {
                    consume();
                    while (peek().has_value() && peek().value() != '%') {
                        consume(); // ignora todo dentro del comentario
                    }
                    if (!peek().has_value()) {
                        std::cerr << "Unclosed comment\n";
                        exit(EXIT_FAILURE);
                    }
                    consume(); // consume '%'
                    continue;  // skip a agregar token
                }
                else if (peek().value() == '<') {
                    consume();
                    tokens.push_back({.type = TokenType::l_arrow});
                    continue;
                }
                else if (peek().value() == '>') {
                    consume();
                    tokens.push_back({.type = TokenType::r_arrow});
                    continue;
                }
                else if (peek().value() == '{') {
                    consume();
                    tokens.push_back({.type = TokenType::l_key});
                    continue;
                }
                else if (peek().value() == '}') {
                    consume();
                    tokens.push_back({.type = TokenType::r_key});
                    continue;
                }

                else if (std::isspace(peek().value())) {
                    consume();
                    continue;
                }
                else {
                    std::cout << "TOKEN ERROR" << peek().value() << "\n";
                    exit(EXIT_FAILURE);
                }
            }
            m_index = 0;
            return tokens;
        }
};
