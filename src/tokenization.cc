#include "tokenization.hh"
#include "error.hh"

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens {};

    std::string buf = "";
    int local_lines = 1;

    while(peek().has_value()) {
        if (std::isalpha(peek().value()) || peek().value() == '_') {
            buf.push_back(consume());
            while(peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
                buf.push_back(consume());
            }
            if (buf == "var") {
                tokens.push_back({.type = TokenType::var, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "cnst") {
                tokens.push_back({.type = TokenType::cnst, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "str") {
                tokens.push_back({.type = TokenType::str_type, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "int") {
                tokens.push_back({.type = TokenType::int_type, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "double") {
                tokens.push_back({.type = TokenType::double_type, .line = local_lines});
                buf.clear();
                continue;
            }
			else if (buf == "any") {
				tokens.push_back({.type = TokenType::any_type, .line = local_lines});
				buf.clear();
				continue;
			}
            else if (buf == "add") {
                tokens.push_back({.type = TokenType::plus, .value="+", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "sub") {
                tokens.push_back({.type = TokenType::minus, .value="-", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "mul") {
                tokens.push_back({.type = TokenType::star, .value="*", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "div") {
                tokens.push_back({.type = TokenType::slash, .value="/", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "adding") {
                tokens.push_back({.type = TokenType::plus_eq, .value="+=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "subing") { // substracting
                tokens.push_back({.type = TokenType::minus_eq, .value="-=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "muling") { //multiplicating
                tokens.push_back({.type = TokenType::star_eq, .value="*=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "diving") { // dividing
                tokens.push_back({.type = TokenType::slash_eq, .value="/=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "inc") {
                tokens.push_back({.type = TokenType::plusplus, .value="++", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "dec") {
                tokens.push_back({.type = TokenType::minusminus, .value="--", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "if") {
                tokens.push_back({.type = TokenType::_if, .value="if", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "else") {
                tokens.push_back({.type = TokenType::_else, .value="else", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "elif") {
                tokens.push_back({.type = TokenType::_elif, .value="elif", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "then") {
                tokens.push_back({.type = TokenType::_then, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "and") {
                tokens.push_back({.type = TokenType::_and, .value="&&", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "or") {
                tokens.push_back({.type = TokenType::_or, .value="||", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "eq") {
                tokens.push_back({.type = TokenType::eq_eq, .value="==", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "neq") {
                tokens.push_back({.type = TokenType::bang_eq, .value="!=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "gt") {
                tokens.push_back({.type = TokenType::gt, .value=">", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "gte") {
                tokens.push_back({.type = TokenType::gte, .value=">=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "lt") {
                tokens.push_back({.type = TokenType::lt, .value="<", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "lte") {
                tokens.push_back({.type = TokenType::lte, .value="<=", .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "import") {
                tokens.push_back({.type = TokenType::import, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "use") {
                tokens.push_back({.type = TokenType::use, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "none" || buf == "None") {
                tokens.push_back({.type = TokenType::none, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "NoArg" || buf == "noarg") {
                tokens.push_back({.type = TokenType::no_arg, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "cr" || buf == "ln") {
                tokens.push_back({.type = TokenType::cr, .line = local_lines});
                buf.clear();
                continue;
            }
			else if (buf == "fn") {
				tokens.push_back({.type = TokenType::_fnc, .line = local_lines});
				buf.clear();
				continue;
			}
            else if (buf == "endfn") {
                tokens.push_back({.type = TokenType::endfn, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "ret") {
                tokens.push_back({.type = TokenType::ret, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "mkpub") {
                tokens.push_back({.type = TokenType::mkpub, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "get") {
                tokens.push_back({.type = TokenType::get, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "while") {
                tokens.push_back({.type = TokenType::_while, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "stop") {
                tokens.push_back({.type = TokenType::_stop, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "continue") {
                tokens.push_back({.type = TokenType::_continue, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "do") {
                tokens.push_back({.type = TokenType::_do, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "true") {
                tokens.push_back({.type = TokenType::_true, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "false") {
                tokens.push_back({.type = TokenType::_false, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "unload") {
                tokens.push_back({.type = TokenType::_unload, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "declmod") {
                tokens.push_back({.type = TokenType::_declmod, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "endmod") {
                tokens.push_back({.type = TokenType::_endmod, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "umod") {
                tokens.push_back({.type = TokenType::_umod, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "ubeepmod") {
                tokens.push_back({.type = TokenType::_ubeepmod, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "llibrary") {
                tokens.push_back({.type = TokenType::_llibrary, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "libpath") {
                tokens.push_back({.type = TokenType::_libpath, .line = local_lines});
                buf.clear();
                continue;
            }
			else if (buf == "gptr") {
				tokens.push_back({.type = TokenType::_gptr, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "sptr") {
				tokens.push_back({.type = TokenType::_sptr, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "ptr") {
				tokens.push_back({.type = TokenType::_ptr, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "globl") {
				tokens.push_back({.type = TokenType::_globl, .line = local_lines});
				buf.clear();
				continue;
			}
            else if (buf == "header") {
                tokens.push_back({.type = TokenType::_header, .line = local_lines});
                buf.clear();
                continue;
            }
            else if (buf == "uhead") {
				tokens.push_back({.type = TokenType::_uhead, .line = local_lines});
				buf.clear();
                continue; 
            }
			else if (buf == "leave") {
				tokens.push_back({.type = TokenType::_leave, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "pub") {
				tokens.push_back({.type = TokenType::_pub, .line = local_lines});
				buf.clear();
				continue;
			} 
			else if (buf == "extern") {
				tokens.push_back({.type = TokenType::_extern, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "list") {
				tokens.push_back({.type = TokenType::_list, .line = local_lines});
				buf.clear();
				continue;
			}
			else if (buf == "struct") {
				tokens.push_back({.type = TokenType::_struct, .line = local_lines});
				buf.clear();
				continue;
			} else if (buf == "nwstruct") {
				tokens.push_back({.type = TokenType::_nwstruct, .line = local_lines});
				buf.clear();
				continue;
			} 
            else {
                tokens.push_back({.type = TokenType::ident, .value = buf, .line = local_lines});
                buf.clear();
            }
        }
        else if (peek().value() == '-' && std::isdigit(peek(1).value())
            || peek().value() == '+' && std::isdigit(peek(1).value()) 
            || std::isdigit(peek().value())) {
            if (peek().value() == '-' || peek().value() == '+') {
                buf.push_back(consume());
            }

            buf.push_back(consume());  // Primer dígito
            bool is_float = false;

            // Parte entera
            while (peek().has_value() && std::isdigit(peek().value())) {
                buf.push_back(consume());
            }

            // Parte decimal
            if (peek().has_value() && peek().value() == '.') {
                is_float = true;
                buf.push_back(consume());  // Consume '.'

                // Requiere al menos un dígito después del punto
                if (peek().has_value() && std::isdigit(peek().value())) {
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buf.push_back(consume());
                    }
                } else {
                    // Error léxico: punto sin decimales
                    // Puedes manejarlo como quieras, por ejemplo:
                    add_error("Decimal dot with not digits after");
                }
            }

            tokens.push_back({
                .type = is_float ? TokenType::float_lit : TokenType::int_lit,
                .value = buf,
                .line = local_lines
            });
            buf.clear();
        }
        else if (peek().value() == '"') {
            consume();
            while (peek().has_value() && peek().value() != '"') {
                buf.push_back(consume());
            }
            if (!peek().has_value() || peek().value() != '"') {
                add_error("Undeterminated string literal");
                exit(EXIT_FAILURE);
            }
            consume();

            tokens.push_back({.type = TokenType::str_lit, .value = buf, .line = local_lines});
            buf.clear();
        }
		else if (peek().value() == '\'') {
            consume();
            while (peek().has_value() && peek().value() != '\'') {
                buf.push_back(consume());
            }
            if (!peek().has_value() || peek().value() != '\'') {
                add_error("Undeterminated string literal");
                exit(EXIT_FAILURE);
            }
            consume();

            tokens.push_back({.type = TokenType::str_lit, .value = buf, .line = local_lines});
            buf.clear();
        }
        else if (peek().value() == '(') {
            consume();
            tokens.push_back({.type = TokenType::open_paren, .line = local_lines});
            continue;
        }
        else if (peek().value() == ')') {
            consume();
            tokens.push_back({.type = TokenType::close_paren, .line = local_lines});
            continue;
        }
        else if (peek().value() == ';') {
            consume();
            tokens.push_back({.type = TokenType::semi, .line = local_lines});
            continue;
        }
        else if (peek().value() == ':') {
            consume();
            tokens.push_back({.type = TokenType::dp, .line = local_lines});
            continue;
        }
        else if (peek().value() == ',') {
            consume();
            tokens.push_back({.type = TokenType::comma, .line = local_lines});
            continue;
        }
        else if (peek().value() == '.') {
            consume();
            tokens.push_back({.type = TokenType::dot, .line = local_lines});
            continue;
        }
        else if (peek().value() == '+' && peek(1).has_value() && peek(1).value() == '+') {
            consume(); consume();
            tokens.push_back({.type = TokenType::plusplus, .value="++", .line = local_lines});
            continue;
        }
        else if (peek().value() == '-' && peek(1).has_value() && peek(1).value() == '-') {
            consume(); consume();
            tokens.push_back({.type = TokenType::minusminus, .value="--", .line = local_lines});
            continue;
        }
        else if (peek().value() == '+' && peek(1).has_value() && peek(1).value() == '=') {
            consume(); consume();
            tokens.push_back({.type = TokenType::plus_eq, .value = "+=", .line = local_lines});
            continue;
        }
        else if (peek().value() == '-' && peek(1).has_value() && peek(1).value() == '=') {
            consume(); consume();
            tokens.push_back({.type = TokenType::minus_eq, .value = "-=", .line = local_lines});
            continue;
        }
        else if (peek().value() == '*' && peek(1).has_value() && peek(1).value() == '=') {
            consume(); consume();
            tokens.push_back({.type = TokenType::star_eq, .value = "*=", .line = local_lines});
            continue;
        }
        else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '=') {
            consume(); consume();
            tokens.push_back({.type = TokenType::slash_eq, .value = "/=", .line = local_lines});
            continue;
        }
        else if (peek().value() == '=') {
            consume();
            tokens.push_back({.type = TokenType::eq, .line = local_lines});
            continue;
        }
        else if (peek().value() == '+') {
            consume();
            tokens.push_back({.type = TokenType::plus, .value="+", .line = local_lines});
            continue;
        }
        else if (peek().value() == '-') {
            consume();
            tokens.push_back({.type = TokenType::minus, .value="-", .line = local_lines});
            continue;
        }
        else if (peek().value() == '*') {
            consume();
            tokens.push_back({.type = TokenType::star, .value="*", .line = local_lines});
            continue;
        }
        else if (peek().value() == '/') {
            consume();
            tokens.push_back({.type = TokenType::slash, .value="/", .line = local_lines});
            continue;
        }
        else if (peek().value() == '%') {
            consume();
            tokens.push_back({.type = TokenType::percent, .value="%", .line = local_lines});
            continue;
        }
        else if (peek().value() == '#') {
            consume();
            while (peek().has_value() && peek().value() != '#') {
                consume(); // ignora todo dentro del comentario
            }
            if (peek().has_value()) {
                consume(); // consume '#'
            }
            continue;  // skip a agregar token
        }
        else if (peek().value() == '<') {
            consume();
            tokens.push_back({.type = TokenType::l_arrow, .line = local_lines});
            continue;
        }
        else if (peek().value() == '>') {
            consume();
            tokens.push_back({.type = TokenType::r_arrow, .line = local_lines});
            continue;
        }
        else if (peek().value() == '{') {
            consume();
            tokens.push_back({.type = TokenType::l_key, .line = local_lines});
            continue;
        }
        else if (peek().value() == '}') {
            consume();
            tokens.push_back({.type = TokenType::r_key, .line = local_lines});
            continue;
        }
        else if (peek().value() == '[') {
            consume();
            tokens.push_back({.type = TokenType::l_bracket, .line = local_lines});
            continue;
        }
        else if (peek().value() == ']') {
            consume();
            tokens.push_back({.type = TokenType::r_bracket, .line = local_lines});
            continue;
        }
        else if (peek().value() == '\n') {
            consume();
            ++local_lines;
            continue;
        }
        else if (std::isspace(peek().value())) {
            consume();
            continue;
        } 
        else {
            add_error("TOKEN ERROR" + std::to_string(peek().value()), local_lines);
            exit(EXIT_FAILURE);
        }
    }
    m_index = 0;
    return tokens;
}
