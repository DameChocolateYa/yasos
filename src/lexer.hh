#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>

#include "global.hh"

enum class TokenType {
  null_tok,
    int_lit,
    str_lit,
    char_lit,
    float_lit,
    semi,
    comma,
    open_paren,
    close_paren,
    ident,
    var,
    cnst,
    eq,
    str_type,
    char_type,
    int_type,
    double_type,
	any_type,
  ptr_type,
    dp,
    comment_begin,
    comment_end,
    import,
    use,
    l_arrow,
    r_arrow,
    l_key,
    r_key,
    l_bracket,
    r_bracket,
    plus,
    minus,
    amp,
    pipe,
    ref,
    deref,
    star,
    slash,
    percent,
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
    bang,
    none,
    _nullptr,
    no_arg,
    cr,
	  _fnc,
    endfn,
    ret,
    mkpub,
    get,
    _while,
    _loop,
    _for,
    _foreach,
    _stop,
    _continue,
    _true,
    _false,
    _unload,
    dot,
    _err,
    _declmod,
    _endmod,
    _umod,
    _ubeepmod,
    _llibrary,
    _libpath,
	_gptr,
	_sptr,
	_ptr,
	_globl,
    _header,
    _uhead,
	_leave,
	_pub,
	_extern,
	_list,
	_struct,
	_nwstruct,
  _new,
  _def,
  _undef,
  _is_def,
  _is_ndef,
  _pre_if,
  _pre_else,
  _pre_elif,
  _pre_endif,
  _pre_error,
  _pre_warning,
  _size_of
};

struct Token {
    TokenType type;
    std::optional<std::string> value = "";
    int line;
};

class Lexer {
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
        inline explicit Lexer(const std::string& src) : m_src(std::move(src)) {
        }

        std::vector<Token> tokenize();
};
