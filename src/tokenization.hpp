#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>

#include "global.hpp"

enum class TokenType {
    int_lit,
    str_lit,
    float_lit,
    semi,
    comma,
    open_paren,
    close_paren,
    ident,
    var,
    eq,
    str_type,
    int_type,
    float_type,
    dp,
    comment_begin,
    comment_end,
    import,
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
    minusminus,
    _if,
    _else,
    _elif,
    _and,
    _or,
    lt,
    lte,
    gt,
    gte,
    eq_eq,
    bang_eq,
    none,
    no_arg,
    cr
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

        std::vector<Token> tokenize();
};
