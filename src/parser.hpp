#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <optional>
#include <variant>
#include <memory>
#include <iostream>

#include "tokenization.hpp"

enum class VarType {
    Str, Int
};

struct NodeExpr;
using NodeExprPtr = std::shared_ptr<NodeExpr>;

struct NodeExprIntLit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExprStrLit {
    Token str_lit;
};

struct NodeExprBinary {
    NodeExprPtr lhs;
    Token op_token;
    NodeExprPtr rhs;
};

struct NodeExprBinaryAssign {
    Token left_token;
    Token op_token;
    NodeExprPtr right_expr;
};

struct NodeExprUnaryIncDec {
    Token ident;
    Token op_token;
};

struct NodeExprCall {
    Token name;
    std::vector<NodeExprPtr> args;
};

struct NodeExpr {
    std::variant<
        NodeExprIntLit,
        NodeExprIdent,
        NodeExprStrLit,
        NodeExprCall,
        NodeExprBinary,
        NodeExprBinaryAssign,
        NodeExprUnaryIncDec
    > var;

    NodeExpr() = default;

    template<typename T>
    NodeExpr(T val) : var(std::move(val)) {}
};

struct NodeExit {
    NodeExpr expr;
};

struct NodeStmtVar {
    Token ident;
    Token type;
    NodeExpr expr;
};

struct NodeStmtCall {
    Token name;
    std::vector<NodeExprPtr> args;
};

struct NodeStmtUse {
    std::vector<Token> use;
};

struct NodeStmtInclude {
    Token include;
};

struct NodeStmt {
    std::variant<NodeStmtVar, NodeStmtCall, NodeStmtInclude, NodeStmtUse> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
};

class Parser {
    private:

        [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const {
            if (m_index + offset >= m_tokens.size()) {
                std::cout << "returning an empty peek\n";
                return {};
            }
            else {
                return m_tokens.at(m_index + offset);
            }
        }

        Token consume() {
            if (m_index >= m_tokens.size()) {
                std::cerr << "TRIED TO CONSUME TOKEN OUT OF BOUNDS\n";
                exit(EXIT_FAILURE);
            }
            return m_tokens.at(m_index++);
        }

        size_t m_index = 0;
        const std::vector<Token> m_tokens;

    public:
        inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

        std::optional<NodeExpr> parse_expr() {
            if (peek().has_value() && peek().value().type == TokenType::ident &&
                peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {

                Token name = consume();
                consume();

                std::vector<NodeExpr> args;

                while (peek().has_value() && peek().value().type != TokenType::close_paren) {
                    if (auto e = parse_expr()) {
                        args.push_back(e.value());
                    } else {
                        std::cerr << "Invalid Expression in function call (expression context)\n";
                        exit(EXIT_FAILURE);
                    }

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                    } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                        std::cerr << "Expected ',' or ')'\n";
                        exit(EXIT_FAILURE);
                    }
                }

                if (peek().has_value() && peek().value().type == TokenType::close_paren) {
                    consume();
                } else {
                    std::cerr << "Expected ')' at the end of function call\n";
                    exit(EXIT_FAILURE);
                }

                std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
                for (auto& arg : args) {
                    wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
                }
                return NodeExpr(NodeExprCall{name, wrapped_args});

            }

            else if (peek().has_value() && peek().value().type == TokenType::int_lit) {
                return NodeExpr(NodeExprIntLit{consume()});
            }
            else if (peek().has_value() && peek().value().type == TokenType::ident) {
                return NodeExpr(NodeExprIdent{consume()});
            }
            else if (peek().has_value() && peek().value().type == TokenType::str_lit) {
                return NodeExpr(NodeExprStrLit{consume()});
            }

            else if (peek().has_value() &&
                     (peek().value().type == TokenType::ident ||
                      peek().value().type == TokenType::int_lit ||
                      peek().value().type == TokenType::str_lit) &&
                     peek(1).has_value() && peek(1).value().type == TokenType::plus)
 {
                Token ident_token = peek().value();
                Token lookahead = peek(1).value();
                std::cout << (lookahead.type == TokenType::plus) << "\n";
                std::cout << peek(1).value().value.value_or("NULL") << "\n";

                if (lookahead.type == TokenType::plus ||
                    lookahead.type == TokenType::minus ||
                    lookahead.type == TokenType::star ||
                    lookahead.type == TokenType::slash) {

                    auto lhs = NodeExpr(NodeExprIdent{consume()});
                    Token op_token = consume();
                    auto rhs = parse_expr();

                    if (!rhs.has_value()) {
                        std::cerr << "Invalid expression after operator\n";
                        exit(EXIT_FAILURE);
                    }

                    return NodeExpr(NodeExprBinary{
                        std::make_shared<NodeExpr>(lhs),
                        op_token,
                        std::make_shared<NodeExpr>(rhs.value())
                    });
                }

                else if (lookahead.type == TokenType::plus_eq ||
                         lookahead.type == TokenType::minus_eq ||
                         lookahead.type == TokenType::star_eq ||
                         lookahead.type == TokenType::slash_eq) {

                    Token left_token = consume();
                    Token op_token = consume();
                    auto right_expr = parse_expr();

                    if (!right_expr.has_value()) {
                        std::cerr << "Invalid expression after operator\n";
                        exit(EXIT_FAILURE);
                    }

                    return NodeExpr(NodeExprBinaryAssign{
                        left_token,
                        op_token,
                        std::make_shared<NodeExpr>(right_expr.value())
                    });
                }

                else if (lookahead.type == TokenType::plusplus ||
                         lookahead.type == TokenType::minusminus) {

                    Token ident = consume();
                    Token op_token = consume();

                    return NodeExpr(NodeExprUnaryIncDec{
                        ident,
                        op_token
                    });
                }
                else {
                    return {};
                }
            }

            else {
                return {};
            }
        }

        std::optional<NodeStmt> parse_stmt() {
            if (peek().has_value() && peek().value().type == TokenType::var &&
                peek(1).has_value() && peek(1).value().type == TokenType::ident &&
                peek(2).has_value() && peek(2).value().type == TokenType::dp &&
                peek(3).has_value() &&
                peek(4).has_value() && peek(4).value().type == TokenType::eq)
            {
                consume();
                Token ident = consume();
                consume();
                Token type_token = consume();
                consume();

                NodeStmtVar stmt_var;
                stmt_var.ident = ident;
                stmt_var.type = type_token;

                std::cout << "assign\n";
                auto expr = parse_expr();
                if (!expr.has_value()) {
                    std::cerr << "Invalid Expression\n";
                    exit(EXIT_FAILURE);
                }
                stmt_var.expr = expr.value();

                if (peek().has_value() && peek().value().type == TokenType::semi) {
                    consume();
                } else {
                    std::cerr << "Expected a ';'\n";
                    exit(EXIT_FAILURE);
                }

                return NodeStmt{.var = stmt_var};
            }

            else if (peek().has_value() && peek().value().type == TokenType::ident &&
                     peek(1).has_value() && peek(1).value().type == TokenType::eq)
            {
                std::cout << "reassign\n";
                Token ident = consume();
                consume();

                std::cout << "reassign\n";
                auto expr = parse_expr();

                if (!expr.has_value()) {
                    std::cerr << "Invalid Expression in reassignment\n";
                    exit(EXIT_FAILURE);
                }

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected a ';'\n";
                    std::cerr << peek().value().value.value_or("TUKS") << "\n";
                    exit(EXIT_FAILURE);
                }
                consume();

                NodeStmtVar reassignment {
                    .ident = ident,
                    .type = Token{.type = TokenType::ident},
                    .expr = expr.value()
                };

                return NodeStmt{.var = reassignment};
            }

            else if (peek().has_value() && peek().value().type == TokenType::ident &&
                     peek(1).has_value() && peek(1).value().type == TokenType::open_paren)
            {
                Token name = consume();
                consume();

                std::vector<NodeExpr> args;

                while (peek().has_value() && peek().value().type != TokenType::close_paren) {
                    auto arg = parse_expr();
                    if (!arg.has_value()) {
                        std::cerr << "Invalid Expression in function call\n";
                        exit(EXIT_FAILURE);
                    }
                    args.push_back(arg.value());

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                    } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                        std::cerr << "Expected ',' or ')'\n";
                        exit(EXIT_FAILURE);
                    }
                }

                consume();

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'\n";
                    exit(EXIT_FAILURE);
                }
                consume();

                std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
                for (auto& arg : args) {
                    wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
                }
                return NodeStmt(NodeStmtCall{name, wrapped_args});

                //return NodeStmt(.var = NodeStmtCall{name, args}};
            }

            else if (peek().has_value() && peek().value().type == TokenType::include &&
                     peek(1).has_value() && peek(1).value().type == TokenType::l_arrow)
            {
                consume();
                consume();
                NodeStmtInclude stmt_include;

                stmt_include.include = consume();

                if (!peek().has_value() || peek().value().type != TokenType::r_arrow) {
                    std::cerr << "Expected '>'\n";
                    exit(EXIT_FAILURE);
                }
                consume();

                return NodeStmt{.var = stmt_include};
            }

            else if (peek().has_value() && peek().value().type == TokenType::use &&
                     peek(1).has_value() && peek(1).value().type == TokenType::l_key)
            {
                consume();
                consume();

                NodeStmtUse use;

                while (peek().has_value() && peek().value().type != TokenType::r_key) {
                    use.use.push_back(consume());

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                    } else if (peek().has_value() && peek().value().type != TokenType::r_key) {
                        std::cerr << "Expected ',' or '}'\n";
                        exit(EXIT_FAILURE);
                    }
                }

                consume();

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'\n";
                    exit(EXIT_FAILURE);
                }
                consume();

                return NodeStmt{.var = use};
            }

            else {
                return {};
            }
        }


        std::optional<NodeProg> parse_prog() {
            NodeProg prog;
            while(peek().has_value()) {
                if (auto stmt = parse_stmt()) {
                    prog.stmts.push_back(stmt.value());
                }
                else {
                    std::cerr << "Invalid Statement\n";
                    exit(EXIT_FAILURE);
                }
            }
            return prog;
        }
};
