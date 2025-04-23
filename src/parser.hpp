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
#include "global.hpp"

#undef __FILE__
#define __FILE__ "src/parser.hpp"

enum class VarType {
    Str, Int, Float
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

struct NodeExprFloatLit {
    Token float_lit;
};

struct NodeExprNone {
    Token none;
};

struct NodeExprNoArg {
    Token no_arg;
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
        NodeExprFloatLit,
        NodeExprCall,
        NodeExprBinary,
        NodeExprBinaryAssign,
        NodeExprUnaryIncDec,
        NodeExprNoArg,
        NodeExprNone
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

struct NodeStmtImport {
    Token to_import;
};

struct NodeStmt;

struct NodeStmtIf {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
    std::optional<std::variant<std::shared_ptr<NodeStmtIf>, std::vector<NodeStmt>>> else_branch;
};

struct NodeStmt {
    std::variant<NodeStmtVar, NodeStmtCall, NodeStmtImport, NodeStmtUse, NodeStmtIf> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
};

class Parser {
    private:

        [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const {
            if (m_index + offset >= m_tokens.size()) {
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

        std::optional<NodeExpr> parse_primary_expr() {
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
                        terminate(EXIT_FAILURE);
                    }

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                    } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                        std::cerr << "Expected ',' or ')'\n";
                        terminate(EXIT_FAILURE);
                    }
                }

                if (peek().has_value() && peek().value().type == TokenType::close_paren) {
                    consume();
                } else {
                    std::cerr << "Expected ')' at the end of function call\n";
                    terminate(EXIT_FAILURE);
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
            else if (peek().has_value() && peek().value().type == TokenType::float_lit) {
                return NodeExpr(NodeExprFloatLit{consume()});
            }
            else if (peek().has_value() && peek().value().type == TokenType::none) {
                return NodeExpr(NodeExprNone{consume()});
            }
            else if (peek().has_value() && peek().value().type == TokenType::no_arg) {
                return NodeExpr(NodeExprNoArg{consume()});
            }
            return {};
        }

        std::optional<NodeExpr> parse_expr() {
            auto lhs = parse_primary_expr();
            if (!lhs.has_value()) return {};

            while (true) {
                auto maybe_op = peek();
                if (!maybe_op.has_value()) break;

                Token op = maybe_op.value();
                if (op.type == TokenType::plus || op.type == TokenType::minus || op.type == TokenType::star || op.type == TokenType::slash ||
                    op.type == TokenType::eq_eq || op.type == TokenType::lt || op.type == TokenType::lte ||
                    op.type == TokenType::gt || op.type == TokenType::gte || op.type == TokenType::_and || op.type == TokenType::_or ||
                    op.type == TokenType::bang_eq) {
                    consume(); // the operator
                    auto rhs = parse_primary_expr();

                    if (!rhs.has_value()) {
                        std::cerr << "Invalid right expression\n";
                        terminate(EXIT_FAILURE);
                    }

                    lhs = NodeExpr(NodeExprBinary{
                        std::make_shared<NodeExpr>(*lhs),
                        op,
                        std::make_shared<NodeExpr>(*rhs)
                    });
                    LOG(__FILE__, "Added Binary Expression Node: type '" + std::get<NodeExprBinary>(lhs.value().var).op_token.value.value() + "'");
                }
                else if (op.type == TokenType::plus_eq || op.type == TokenType::minus_eq || op.type == TokenType::star_eq || op.type == TokenType::slash_eq) {
                    consume();
                    auto rhs = parse_primary_expr();
                    if (!rhs.has_value() || !lhs.has_value()) {
                        std::cerr << "Invalid expression\n";
                        terminate(EXIT_FAILURE);
                    }

                    if (!std::holds_alternative<NodeExprIdent>(lhs->var)) {
                        std::cerr << "Expected an identifier\n";
                        terminate(EXIT_FAILURE);
                    }

                    Token ident_token = std::get<NodeExprIdent>(lhs->var).ident;

                    lhs = NodeExpr(NodeExprBinaryAssign{
                        .left_token = ident_token,
                        .op_token = op,
                        .right_expr = std::make_shared<NodeExpr>(*rhs)
                    });
                }
                else {
                    break;
                }
            }
            return lhs;
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

                auto expr = parse_expr();
                if (!expr.has_value()) {
                    std::cerr << "Invalid Expression\n";
                    terminate(EXIT_FAILURE);
                }
                stmt_var.expr = expr.value();

                if (peek().has_value() && peek().value().type == TokenType::semi) {
                    consume();
                } else {
                    std::cerr << "Expected a ';'\n";
                    terminate(EXIT_FAILURE);
                }

                return NodeStmt{.var = stmt_var};
            }

            else if (peek().has_value() && peek().value().type == TokenType::ident &&
                     peek(1).has_value() && peek(1).value().type == TokenType::eq)
            {
                Token ident = consume();
                consume();

                auto expr = parse_expr();

                if (!expr.has_value()) {
                    std::cerr << "Invalid Expression in reassignment\n";
                    terminate(EXIT_FAILURE);
                }

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected a ';'\n";
                    terminate(EXIT_FAILURE);
                }
                consume();

                NodeStmtVar reassignment {
                    .ident = ident,
                    .type = Token{.type = TokenType::ident},
                    .expr = expr.value()
                };
                return NodeStmt{.var = reassignment};
            }

            else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && (
                peek(1).value().type == TokenType::plus_eq ||
                peek(1).value().type == TokenType::minus_eq ||
                peek(1).value().type == TokenType::star_eq ||
                peek(1).value().type == TokenType::slash_eq
            )) {
                Token ident = consume(); // Identifier   ...var1...
                Token op = consume(); // Operator (+=, -=, *=, /=)

                auto expr = parse_expr();
                if (!expr.has_value()) {
                    std::cerr << "Invalid expression in assignment\n";
                    terminate(EXIT_FAILURE);
                }

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'";
                    terminate(EXIT_FAILURE);
                }
                consume(); // ;

                NodeStmtVar reassignment {
                    .ident = ident,
                    .type = Token{.type = TokenType::ident},
                    .expr = NodeExpr(NodeExprBinaryAssign{
                        .left_token = ident,
                        .op_token = op,
                        .right_expr = std::make_shared<NodeExpr>(expr.value())
                    })
                };
                return NodeStmt{.var = reassignment};
            }
            else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && (
                peek(1).value().type == TokenType::plusplus || peek(1).value().type == TokenType::minusminus
            )) {
                Token ident = consume(); // Identifier   ...var1...
                Token op = consume(); // Operator (+=, -=, *=, /=)

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'";
                    terminate(EXIT_FAILURE);
                }
                consume(); // ;

                NodeStmtVar reassignment {
                    .ident = ident,
                    .type = Token{.type = TokenType::ident},
                    .expr = NodeExpr(NodeExprUnaryIncDec{
                        .ident = ident,
                        .op_token = op,
                    })
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
                        terminate(EXIT_FAILURE);
                    }
                    args.push_back(arg.value());

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                    } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                        std::cerr << "Expected ',' or ')'\n";
                        terminate(EXIT_FAILURE);
                    }
                }

                consume();

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'\n";
                    terminate(EXIT_FAILURE);
                }
                consume();

                std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
                for (auto& arg : args) {
                    wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
                }
                return NodeStmt(NodeStmtCall{name, wrapped_args});

                //return NodeStmt(.var = NodeStmtCall{name, args}};
            }

            else if (peek().has_value() && peek().value().type == TokenType::_if) {
                consume(); // If

                if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
                    std::cerr << "Expected a '(' after 'if'\n";
                    terminate(EXIT_FAILURE);
                }
                consume(); // (

                auto condition = parse_expr();
                if (!condition.has_value()) {
                    std::cerr << "Invalid expression\n";
                    terminate(EXIT_FAILURE);
                }

                if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
                    std::cout << static_cast<int>(peek().value().type) << "\n";
                    std::cerr << "Expected ')' after conditional\n";
                    terminate(EXIT_FAILURE);
                }
                consume(); // )

                if (!peek().has_value() || peek().value().type != TokenType::l_key) {
                    std::cerr << "Expected '{' to start conditional\n";
                    terminate(EXIT_FAILURE);
                }
                consume(); // {

                std::vector<NodeStmt> then_branch;
                while (peek().has_value() && peek().value().type != TokenType::r_key) {
                    auto stmt = parse_stmt();
                    if (!stmt.has_value()) {
                        std::cerr << "Invalid statment in 'if' block\n";
                        terminate(EXIT_FAILURE);
                    }
                    then_branch.push_back(stmt.value());
                }

                if (!peek().has_value() || peek().value().type != TokenType::r_key) {
                    std::cerr << "Expected '}' to end 'if' block\n";
                    terminate(EXIT_FAILURE);
                }
                consume(); // }

                std::optional<std::variant<std::shared_ptr<NodeStmtIf>, std::vector<NodeStmt>>> else_branch;

                if (peek().has_value()) {
                    if (peek()->type == TokenType::_elif) {
                        auto maybe_elif = parse_stmt();
                        if (!maybe_elif.has_value() || !std::holds_alternative<NodeStmtIf>(maybe_elif->var)) {
                            std::cerr << "Invalid 'elif' statment\n";
                            terminate(EXIT_FAILURE);
                        }

                        else_branch = std::make_shared<NodeStmtIf>(std::get<NodeStmtIf>(maybe_elif->var));
                    }
                    else if (peek()->type == TokenType::_else) {
                        consume(); // consume 'else'

                        if (!peek().has_value() || peek()->type != TokenType::l_key) {
                            std::cerr << "Expected '{' to start 'else' block\n";
                            terminate(EXIT_FAILURE);
                        }
                        consume(); // consume '{'

                        std::vector<NodeStmt> else_block;
                        while (peek().has_value() && peek()->type != TokenType::r_key) {
                            auto stmt = parse_stmt();
                            if (!stmt.has_value()) {
                                std::cerr << "Invalid statement in 'else' block\n";
                                terminate(EXIT_FAILURE);
                            }
                            else_block.push_back(stmt.value());
                        }

                        if (!peek().has_value() || peek()->type != TokenType::r_key) {
                            std::cerr << "Expected '}' to end 'else' block\n";
                            terminate(EXIT_FAILURE);
                        }
                        consume(); // consume '}'

                        else_branch = else_block;
                    }
                }
                NodeStmtIf if_stmt {
                    .condition = condition.value(),
                    .then_branch = then_branch,
                    .else_branch = else_branch
                };
                return NodeStmt{.var=if_stmt};
            }

            else if (peek().has_value() && peek().value().type == TokenType::import &&
                     peek(1).has_value() && peek(1).value().type == TokenType::l_arrow)
            {
                consume();
                consume();
                NodeStmtImport stmt_import;

                stmt_import.to_import = consume();

                if (!peek().has_value() || peek().value().type != TokenType::r_arrow) {
                    std::cerr << "Expected '>'\n";
                    terminate(EXIT_FAILURE);
                }
                consume();

                return NodeStmt{.var = stmt_import};
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
                        terminate(EXIT_FAILURE);
                    }
                }

                consume();

                if (!peek().has_value() || peek().value().type != TokenType::semi) {
                    std::cerr << "Expected ';'\n";
                    terminate(EXIT_FAILURE);
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
                    terminate(EXIT_FAILURE);
                }
            }
            return prog;
        }
};
