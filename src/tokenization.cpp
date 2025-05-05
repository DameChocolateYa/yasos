#include "tokenization.hpp"

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens {};

    std::string buf = "";

    while(peek().has_value()) {
        if (std::isalpha(peek().value())) {
            buf.push_back(consume());
            while(peek().has_value() && std::isalnum(peek().value())) {
                buf.push_back(consume());
                for (auto& c : buf) {
                    c = std::tolower(c);
                }
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
            else if (buf == "float") {
                tokens.push_back({.type = TokenType::float_type});
                buf.clear();
                continue;
            }
            else if (buf == "add") {
                tokens.push_back({.type = TokenType::plus, .value="+"});
                buf.clear();
                continue;
            }
            else if (buf == "sub") {
                tokens.push_back({.type = TokenType::minus, .value="-"});
                buf.clear();
                continue;
            }
            else if (buf == "mul") {
                tokens.push_back({.type = TokenType::star, .value="*"});
                buf.clear();
                continue;
            }
            else if (buf == "div") {
                tokens.push_back({.type = TokenType::slash, .value="/"});
                buf.clear();
                continue;
            }
            else if (buf == "adding") {
                tokens.push_back({.type = TokenType::plus_eq, .value="+="});
                buf.clear();
                continue;
            }
            else if (buf == "subing") { // substracting
                tokens.push_back({.type = TokenType::minus_eq, .value="-="});
                buf.clear();
                continue;
            }
            else if (buf == "muling") { //multiplicating
                tokens.push_back({.type = TokenType::star_eq, .value="*="});
                buf.clear();
                continue;
            }
            else if (buf == "diving") { // dividing
                tokens.push_back({.type = TokenType::slash_eq, .value="/="});
                buf.clear();
                continue;
            }
            else if (buf == "inc") {
                tokens.push_back({.type = TokenType::plusplus, .value="++"});
                buf.clear();
                continue;
            }
            else if (buf == "dec") {
                tokens.push_back({.type = TokenType::minusminus, .value="--"});
                buf.clear();
                continue;
            }
            else if (buf == "if") {
                tokens.push_back({.type = TokenType::_if, .value="if"});
                buf.clear();
                continue;
            }
            else if (buf == "else") {
                tokens.push_back({.type = TokenType::_else, .value="else"});
                buf.clear();
                continue;
            }
            else if (buf == "elif") {
                tokens.push_back({.type = TokenType::_elif, .value="elif"});
                buf.clear();
                continue;
            }
            else if (buf == "then") {
                tokens.push_back({.type = TokenType::_then});
                buf.clear();
                continue;
            }
            else if (buf == "and") {
                tokens.push_back({.type = TokenType::_and, .value="&&"});
                buf.clear();
                continue;
            }
            else if (buf == "or") {
                tokens.push_back({.type = TokenType::_or, .value="||"});
                buf.clear();
                continue;
            }
            else if (buf == "eq") {
                tokens.push_back({.type = TokenType::eq_eq, .value="=="});
                buf.clear();
                continue;
            }
            else if (buf == "neq") {
                tokens.push_back({.type = TokenType::bang_eq, .value="!="});
                buf.clear();
                continue;
            }
            else if (buf == "gt") {
                tokens.push_back({.type = TokenType::gt, .value=">"});
                buf.clear();
                continue;
            }
            else if (buf == "gte") {
                tokens.push_back({.type = TokenType::gte, .value=">="});
                buf.clear();
                continue;
            }
            else if (buf == "lt") {
                tokens.push_back({.type = TokenType::lt, .value="<"});
                buf.clear();
                continue;
            }
            else if (buf == "lte") {
                tokens.push_back({.type = TokenType::lte, .value="<="});
                buf.clear();
                continue;
            }
            else if (buf == "import") {
                tokens.push_back({.type = TokenType::import});
                buf.clear();
                continue;
            }
            else if (buf == "use") {
                tokens.push_back({.type = TokenType::use});
                buf.clear();
                continue;
            }
            else if (buf == "none" || buf == "None") {
                tokens.push_back({.type = TokenType::none});
                buf.clear();
                continue;
            }
            else if (buf == "NoArg" || buf == "noarg") {
                tokens.push_back({.type = TokenType::no_arg});
                buf.clear();
                continue;
            }
            else if (buf == "cr") {
                tokens.push_back({.type = TokenType::cr});
                buf.clear();
                continue;
            }
            else if (buf == "endfn") {
                tokens.push_back({.type = TokenType::endfn});
                buf.clear();
                continue;
            }
            else if (buf == "ret") {
                tokens.push_back({.type = TokenType::ret});
                buf.clear();
                continue;
            }
            else if (buf == "mkpub") {
                tokens.push_back({.type = TokenType::mkpub});
                buf.clear();
                continue;
            }
            else if (buf == "get") {
                tokens.push_back({.type = TokenType::get});
                buf.clear();
                continue;
            }
            else if (buf == "while") {
                tokens.push_back({.type = TokenType::_while});
                buf.clear();
                continue;
            }
            else if (buf == "stop") {
                tokens.push_back({.type = TokenType::_stop});
                buf.clear();
                continue;
            }
            else if (buf == "continue") {
                tokens.push_back({.type = TokenType::_continue});
                buf.clear();
                continue;
            }
            else if (buf == "do") {
                tokens.push_back({.type = TokenType::_do});
                buf.clear();
                continue;
            }
            else if (buf == "true") {
                tokens.push_back({.type = TokenType::_true});
                buf.clear();
                continue;
            }
            else if (buf == "false") {
                tokens.push_back({.type = TokenType::_false});
                buf.clear();
                continue;
            }
            else if (buf == "unload") {
                tokens.push_back({.type = TokenType::_unload});
                buf.clear();
                continue;
            }
            else {
                tokens.push_back({.type = TokenType::ident, .value = buf});
                buf.clear();
            }
        }
        else if (std::isdigit(peek().value())) {
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
                    std::cerr << "Error: punto decimal sin dígitos después\n";
                }
            }

            tokens.push_back({
                .type = is_float ? TokenType::float_lit : TokenType::int_lit,
                .value = buf
            });
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
