#include "parser.hpp"
#include "global.hpp"
#include "error.hpp"
#include "tokenization.hpp"
#include <cstdlib>
#include <execution>
#include <memory>

std::optional<NodeExpr> Parser::parse_primary_expr() {
    if (peek().has_value() && peek().value().type == TokenType::ident &&
        peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {

        Token name = consume();
        int line = name.line;
        consume();

        std::vector<NodeExpr> args;

        while (peek().has_value() && peek().value().type != TokenType::close_paren) {
            if (auto e = parse_expr()) {
                args.push_back(e.value());
            } else {
                add_error("Invalid Expression in function call (expression context)", line);
            }

            if (peek().has_value() && peek().value().type == TokenType::comma) {
                consume();
            } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                add_error("Expected ',' or ')'", line);
            }
        }

        if (peek().has_value() && peek().value().type == TokenType::close_paren) {
            consume();
        } else {
            add_error("Expected ')' at the end of function call", line);
        }

        std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
        for (auto& arg : args) {
            wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
        }
        return NodeExpr(NodeExprCall{name, wrapped_args, line});

    }
    else if (peek().has_value() && peek().value().type == TokenType::get &&
        peek(1).has_value() && peek(1).value().type == TokenType::ident &&
        peek(2).has_value() && peek(2).value().type == TokenType::l_arrow) {
            consume();
            Token name = consume();
            int line = name.line;
            consume();

            std::vector<NodeExpr> arg_values;
            while (peek().has_value() && peek().value().type != TokenType::r_arrow) {
                NodeExpr expr;
                if (auto e = parse_expr()) {
                    expr = e.value();
                }
                arg_values.push_back(expr);

                if (peek().has_value() && peek().value().type == TokenType::comma) {
                    consume();
                    continue;
                }
                else if (peek().has_value() && peek().value().type == TokenType::r_arrow) {
                    break;
                }
                else {
                    //std::cerr << "Expected ';'\n";
                    break;
                }
            }
            if (!peek().has_value() || peek().value().type != TokenType::r_arrow) {
                add_error("Expected '>'", line);
            }
            consume(); // <

            return NodeExpr(NodeExprCallCustomFunc{.name = name, .arg_values = arg_values, .line = line});
        }
    else if (peek().has_value() && peek().value().type == TokenType::open_paren) {
        int line = peek().value().line;
        consume(); // (

        auto inner_expr = parse_expr();
        if (!inner_expr.has_value()) {
            add_error("Invalid expression", line);
        }
        if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
            add_error("Expected ')'", line);
        }
        consume(); // )
        
        return inner_expr;
    }
    else if (peek().has_value() && peek().value().type == TokenType::get &&
            peek(1).has_value() && peek(1).value().type == TokenType::ident
            && peek(2).has_value() && peek(2).value().type == TokenType::dot) {
        consume(); // get
        Token ident = consume();
        int line = ident.line;
        consume(); // .
        Token property = consume();

        int is_func = false;
        std::vector<NodeExprPtr> args;
        if (peek().has_value() && peek().value().type == TokenType::open_paren) {
            consume(); // (
            is_func = true;
            while (peek().has_value() && peek().value().type != TokenType::close_paren) {
                auto e = parse_primary_expr();
                if (!e.has_value()) {
                    add_error("Invalid expression in property args", line);
                }
                args.push_back(std::make_shared<NodeExpr>(e.value()));

                if (peek().has_value() && peek().value().type == TokenType::comma) {
                    consume();
                    continue;
                }
            }
            if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
                add_error("Expected ')'", line);
            }
            consume();
        }

        return NodeExpr(NodeExprProperty{.ident = ident, .property = property, .is_func = is_func, .args = args, .line = line});
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
    else if (peek().has_value() && peek().value().type == TokenType::cr) {
        return NodeExpr(NodeExprCR{consume()});
    }
    else if (peek().has_value() && peek().value().type == TokenType::_true) {
        consume();
        return NodeExpr(NodeExprBoolValue{1});
    }
    else if (peek().has_value() && peek().value().type == TokenType::_false) {
        consume();
        return NodeExpr(NodeExprBoolValue{0});
    }

    return {};
}

std::optional<NodeExpr> Parser::parse_expr() {
    auto lhs = parse_primary_expr();
    if (!lhs.has_value()) return {};

    while (true) {
        auto maybe_op = peek();
        if (!maybe_op.has_value()) break;

        Token op = maybe_op.value();
        int line = op.line;
        if (op.type == TokenType::plus || op.type == TokenType::minus || op.type == TokenType::star || op.type == TokenType::slash ||
            op.type == TokenType::percent || op.type == TokenType::eq_eq || op.type == TokenType::lt || op.type == TokenType::lte ||
            op.type == TokenType::gt || op.type == TokenType::gte || op.type == TokenType::_and || op.type == TokenType::_or ||
            op.type == TokenType::bang_eq) {
            consume(); // the operator
            auto rhs = parse_primary_expr();

            if (!rhs.has_value()) {
                add_error("Invalid right expression", line);
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
                add_error("Invalid expression", line);
            }

            if (!std::holds_alternative<NodeExprIdent>(lhs->var)) {
                add_error("Expected an identifier", line);
            }

            Token ident_token = std::get<NodeExprIdent>(lhs->var).ident;

            lhs = NodeExpr(NodeExprBinaryAssign{
                .left_token = ident_token,
                .op_token = op,
                .right_expr = std::make_shared<NodeExpr>(*rhs),
                .line = line
            });
        }
        else {
            break;
        }
    }
    return lhs;
}

std::optional<NodeStmt> Parser::parse_stmt() {
    if (peek().has_value() && (peek().value().type == TokenType::var || peek().value().type == TokenType::cnst) &&
        peek(1).has_value() && peek(1).value().type == TokenType::ident &&
        peek(2).has_value() && peek(2).value().type == TokenType::dp &&
        peek(3).has_value() &&
        peek(4).has_value() && peek(4).value().type == TokenType::eq)
    {
        int mut = peek().value().type == TokenType::var ? true : false;
        consume();
        Token ident = consume();
        int line = ident.line;
        consume();
        Token type_token = consume();
        consume();

        VarType type;
        switch (type_token.type) {
            case TokenType::int_type:
                type = VarType::Int;
                break;
            case TokenType::str_type:
                type = VarType::Str;
                break;
            case TokenType::float_type:
                type = VarType::Float;
                break;
            default:
                add_error("Invalid type", line);
        }

        NodeStmtVar stmt_var;
        stmt_var.ident = ident;
        stmt_var.type = type;

        auto expr = parse_expr();
        if (!expr.has_value()) {
            add_error("Invalid Expression", line);
        }
        stmt_var.expr = expr.value();

        stmt_var.is_mutable = mut;
        stmt_var.line = line;

        if (peek().has_value() && peek().value().type == TokenType::semi) {
            consume();
        } else {
            add_error("Expected ';'", line);
        }

        return NodeStmt{.var = stmt_var};
    }

    else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && peek(1).value().type == TokenType::eq && peek(2).has_value() && peek(2).value().type == TokenType::open_paren) {
        Token name = consume();
        int line = name.line;
        consume(); // =
        consume(); // (
        std::vector<CustomFuncArgs> args;

        while (peek().has_value() && peek().value().type != TokenType::close_paren) {
            if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && peek(1).value().type == TokenType::dp &&
                peek(2).has_value() && (peek(2).value().type == TokenType::str_type || peek(2).value().type == TokenType::int_type || peek(2).value().type == TokenType::float_type)) {
                    std::string arg_name = consume().value.value();
                    consume();
                    Token arg_type_tok = consume();
                    ArgType arg_type;
                    if (arg_type_tok.type == TokenType::str_type) arg_type = ArgType::String;
                    else if (arg_type_tok.type == TokenType::int_type) arg_type = ArgType::Integer;
                    else if (arg_type_tok.type == TokenType::float_type) arg_type = ArgType::Float;

                    args.push_back({.name = arg_name, .arg_type = arg_type});

                    if (peek().has_value() && peek().value().type == TokenType::comma) {
                        consume();
                        continue;
                    }
                    else if (peek().has_value() && peek().value().type == TokenType::close_paren) {
                        break;
                    }
                    else {
                        add_error("Expected ')'", line);
                    }
            }
            else {
                add_error("Malformed expression in function declaration", line);
            }
        }
        consume(); // )

        VarType return_type = VarType::Void;
        if (peek().has_value() && peek().value().type == TokenType::minus &&
            peek(1).has_value() && peek(1).value().type == TokenType::r_arrow) {
            int line = peek().value().line;
            consume();
            consume();

            if (!peek().has_value()) {
                add_error("Expected a type for return specification", line);
            }
            if (peek().value().type == TokenType::str_type) return_type = VarType::Str;
            else if (peek().value().type == TokenType::int_type) return_type = VarType::Int;
            else if (peek().value().type == TokenType::float_type) return_type = VarType::Float;
            else {
                add_error("Unknown return type in specification", line);
            }
            consume();
        }

        if (!peek().has_value() || peek().value().type != TokenType::dp) { // :
            add_error("Expected ':'", peek().value().line);
        }
        consume(); // :
        return NodeStmt{NodeStmtDefFunc{.name = name, .args = args, .return_type = return_type, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && peek(1).value().type == TokenType::l_arrow) {
        Token name = consume();
        int line = name.line;
        consume(); // >

        std::vector<NodeExpr> arg_values;
        while (peek().has_value() && peek().value().type != TokenType::r_arrow) {
            NodeExpr expr;
            if (auto e = parse_expr()) {
                expr = e.value();
            }

            arg_values.push_back(expr);

            if (peek().has_value() && peek().value().type == TokenType::comma) {
                consume();
                continue;
            }
            else if (peek().has_value() && peek().value().type == TokenType::r_arrow) {
                break;
            }
        }
        if (!peek().has_value() || peek().value().type != TokenType::r_arrow) {
            add_error("Expected '>'", line);
        }
        consume(); // <

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        return NodeStmt{.var = NodeStmtCallCustomFunc{.name = name, .arg_values = arg_values, .line = line}};
    }

    else if (peek().has_value() && peek().value().type == TokenType::ident &&
                peek(1).has_value() && peek(1).value().type == TokenType::eq)
    {
        Token ident = consume();
        int line = ident.line;
        consume();

        auto expr = parse_expr();

        if (!expr.has_value()) {
            add_error("Invalid Expression in reassignment", line);
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        NodeStmtVarRe reassignment {
            .ident = ident,
            .expr = expr.value(),
            .line = line
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
        int line = ident.line;
        Token op = consume(); // Operator (+=, -=, *=, /=)

        auto expr = parse_expr();
        if (!expr.has_value()) {
            add_error("Invalid expression in assignment", line);
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        NodeStmtVarRe reassignment {
            .ident = ident,
            .expr = NodeExpr(NodeExprBinaryAssign{
                .left_token = ident,
                .op_token = op,
                .right_expr = std::make_shared<NodeExpr>(expr.value()),
                .line = line
            })
        };
        return NodeStmt{.var = reassignment};
    }
    else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && (
        peek(1).value().type == TokenType::plusplus || peek(1).value().type == TokenType::minusminus
    )) {
        Token ident = consume(); // Identifier   ...var1...
        int line = ident.line;
        Token op = consume(); // Operator (+=, -=, *=, /=)

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        NodeStmtVar reassignment {
            .ident = ident,
            .expr = NodeExpr(NodeExprUnaryIncDec{
                .ident = ident,
                .op_token = op,
                .line = line
            })
        };
        return NodeStmt{.var = reassignment};
    }
    else if (peek().has_value() && (peek().value().type == TokenType::plusplus || peek().value().type == TokenType::minusminus) && peek(1).has_value() && peek(1).value().type == TokenType::ident) {
        Token op = consume();
        int line = op.line;
        Token ident = consume();

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", peek().value().line);
        }
        consume(); // ;

        NodeStmtVarRe reassignment {
            .ident = ident,
            .expr = NodeExpr(NodeExprUnaryIncDec{
                .ident = ident,
                .op_token = op,
                .line = line
            })
        };
        return NodeStmt{.var = reassignment};
    }

    else if (peek().has_value() && peek().value().type == TokenType::ident &&
                peek(1).has_value() && peek(1).value().type == TokenType::open_paren)
    {
        Token name = consume();
        int line = name.line;
        consume();

        std::vector<NodeExpr> args;

        while (peek().has_value() && peek().value().type != TokenType::close_paren) {
            auto arg = parse_expr();
            if (!arg.has_value()) {
                add_error("Invalid Expression in function call", line);
            }
            args.push_back(arg.value());

            if (peek().has_value() && peek().value().type == TokenType::comma) {
                consume();
            } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
                add_error("Expected ',' or ')'", line);
            }
        }

        consume();

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
        for (auto& arg : args) {
            wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
        }
        return NodeStmt(NodeStmtCall{name, wrapped_args, line});

        //return NodeStmt(.var = NodeStmtCall{name, args}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::str_lit) {
        int line = peek().value().line;
        NodeExpr str;
        
        auto tok = peek();
        if (auto expr = parse_expr()) {
            str = expr.value();
        }

        std::vector<NodeExpr> args;
        while (auto tok = peek()) {
            if (tok->type != TokenType::comma) break;

            consume();

            if (auto expr = parse_expr()) {
                args.push_back(std::move(*expr));
            }
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt(NodeStmtPrint{str, args, line});
    }

    else if (peek().has_value() && peek().value().type == TokenType::_if) {
        int line = peek().value().line;
        consume(); // If

        if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
            add_error("Expected a '(' after 'if'", line);
        }
        consume(); // (

        auto condition = parse_expr();
        if (!condition.has_value()) {
            add_error("Invalid expression", line);
        }

        if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
            add_error("Expected ')' after conditional", line);
        }
        consume(); // )

        if (!peek().has_value() || peek().value().type != TokenType::_then) {
            add_error("Expected a 'then' after 'if'", line);
        }
        consume(); // then

        if (!peek().has_value() || peek().value().type != TokenType::l_key) {
            add_error("Expected '{' to start conditional", line);
        }
        consume(); // {

        std::vector<NodeStmt> then_branch;
        while (peek().has_value() && peek().value().type != TokenType::r_key) {
            auto stmt = parse_stmt();
            if (!stmt.has_value()) {
                add_error("Invalid statment in 'if' block", line);
            }
            then_branch.push_back(stmt.value());
        }

        if (!peek().has_value() || peek().value().type != TokenType::r_key) {
            add_error("Expected '}' to end 'if' block", line);
        }
        consume(); // }

        std::optional<std::variant<std::shared_ptr<NodeStmtIf>, std::vector<NodeStmt>>> else_branch;

        if (peek().has_value()) {
            if (peek()->type == TokenType::_elif) {
                consume();
                auto maybe_elif = parse_stmt();
                if (!maybe_elif.has_value() || !std::holds_alternative<NodeStmtIf>(maybe_elif->var)) {
                    add_error("Invalid 'elif' statment", line);
                }

                else_branch = std::make_shared<NodeStmtIf>(std::get<NodeStmtIf>(maybe_elif->var));
            }
            else if (peek()->type == TokenType::_else) {
                consume(); // consume 'else'

                if (!peek().has_value() || peek()->type != TokenType::l_key) {
                    add_error("Expected '{' to start 'else' block", line);
                }
                consume(); // consume '{'

                std::vector<NodeStmt> else_block;
                while (peek().has_value() && peek()->type != TokenType::r_key) {
                    auto stmt = parse_stmt();
                    if (!stmt.has_value()) {
                        add_error("Invalid statement in 'else' block", line);
                    }
                    else_block.push_back(stmt.value());
                }

                if (!peek().has_value() || peek()->type != TokenType::r_key) {
                    add_error("Expected '}' to end 'else' block");
                }
                consume(); // consume '}'

                else_branch = else_block;
            }
        }
        NodeStmtIf if_stmt {
            .condition = condition.value(),
            .then_branch = then_branch,
            .else_branch = else_branch,
            .line = line
        };
        return NodeStmt{.var=if_stmt};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_while) {
        int line = peek().value().line;
        consume(); // while
        std::vector<NodeStmt> bfw;
        std::vector<NodeStmt> afi;
        /*if (peek().has_value() && peek().value().type == TokenType::l_bracket) {
            consume(); // [
            while (peek().has_value() && peek().value().type != TokenType::r_bracket) {
                auto stmt = parse_stmt();
                if (!stmt.has_value()) {
                    std::cerr << "Invalid statment in 'while' inline\n";
                    terminate(EXIT_FAILURE);
                }
                bfw.push_back(stmt.value());
            }
            if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
                std::cerr << "Expected ']'\n";
                terminate(EXIT_FAILURE);
            }
            consume(); // ]
        }*/
        if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
            add_error("Expected '('", line);
        }
        consume(); // (

        auto e = parse_expr();
        if (!e.has_value()) {
            add_error("Invalid expression", line);
        }
        NodeExpr condition = e.value();

        if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
            add_error("Expected ')' after while conditional", line);
        }
        consume(); // )

        /*if (peek().has_value() && peek().value().type == TokenType::l_bracket) {
            consume(); // [
            while (peek().has_value() && peek().value().type != TokenType::r_bracket) {
                auto stmt = parse_stmt();
                if (!stmt.has_value()) {
                    std::cerr << "Invalid statment in 'while' inline\n";
                    terminate(EXIT_FAILURE);
                }
                afi.push_back(stmt.value());
            }
            if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
                std::cerr << "Expected ']'\n";
                terminate(EXIT_FAILURE);
            }
            consume(); // ]
        }*/

        if (!peek().has_value() || peek().value().type != TokenType::_do) {
            add_error("Expected 'do' after while conditional", line);
        }
        consume(); // do

        if (!peek().has_value() || peek().value().type != TokenType::l_key) {
            add_error("Expected '{' after while conditional");
        }
        consume(); // {

        std::vector<NodeStmt> then_branch;
        while (peek().has_value() && peek().value().type != TokenType::r_key) {
            auto stmt = parse_stmt();
            if (!stmt.has_value()) {
                add_error("Invalid statment in 'while' block", line);
            }
            then_branch.push_back(stmt.value());
        }

        if (!peek().has_value() || peek().value().type != TokenType::r_key) {
            add_error("Expected '}' after while loop", line);
        }
        consume(); // {

        return NodeStmt{.var = NodeStmtWhile{.condition = condition, .then_branch = then_branch, .bfw = bfw, .afi = afi, .line = line}};
    }

    else if (peek().has_value() && peek().value().type == TokenType::import &&
                peek(1).has_value() && peek(1).value().type == TokenType::l_arrow)
    {
        int line = peek().value().line;
        consume();
        consume();
        NodeStmtImport stmt_import;

        stmt_import.to_import = consume();
        stmt_import.line = line;

        if (!peek().has_value() || peek().value().type != TokenType::r_arrow) {
            add_error("Expected '>'", line);
        }
        consume();

        return NodeStmt{.var = stmt_import};
    }

    else if (peek().has_value() && peek().value().type == TokenType::use &&
                peek(1).has_value() && peek(1).value().type == TokenType::l_key)
    {
        int line = peek().value().line;
        consume();
        consume();

        NodeStmtUse use;

        while (peek().has_value() && peek().value().type != TokenType::r_key) {
            use.use.push_back(consume());

            if (peek().has_value() && peek().value().type == TokenType::comma) {
                consume();
            } else if (peek().has_value() && peek().value().type != TokenType::r_key) {
                add_error("Expected ',' or '}'", line);
            }
        }
        use.line = line;

        consume();

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt{.var = use};
    }
    else if (peek().has_value() && peek().value().type == TokenType::mkpub) {
        int line = peek().value().line;
        NodeStmtMkpub mkpub;
        mkpub.line = line;
        consume();
        if (peek().has_value() && peek().value().type == TokenType::l_key) {
            consume(); // {
            while (peek().has_value() && peek().value().type != TokenType::r_key) {
                mkpub.functions.push_back(consume()); // func name

                if (peek().has_value() && peek().value().type == TokenType::comma) {
                    consume(); // ,
                }
                else if (peek().has_value() && peek().value().type != TokenType::r_key) {
                    add_error("Expected ',' or '}'", line);
                }

            }
            consume();
        }
        else if (peek().has_value() && peek().value().type == TokenType::ident) {
            mkpub.functions.push_back(consume());
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }

        consume();

        return NodeStmt{.var = mkpub};
    }
    else if (peek().has_value() && peek().value().type == TokenType::ident && peek(1).has_value() && peek(1).value().type == TokenType::dp) {
        Token name = consume();
        int line = peek().value().line;
        consume();
        return NodeStmt{.var = NodeStmtDefFunc{.name = name, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::endfn) {
        int line = peek().value().line;
        consume();
        return NodeStmt{.var = NodeStmtEndfn{.line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::ret) {
        int line = peek().value().line;
        consume(); // ret keyword
        auto e = parse_expr();
        if (!e.has_value()) {
            add_error("Invalid expression in return statment", line);
        }
        NodeExpr expr = e.value();

        if (!peek().has_value() && peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt{.var = NodeStmtRet{.value = expr, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_unload) {
        int line = peek().value().line;
        consume(); // unload
        std::vector<Token> vars;

        if (peek().has_value() && peek().value().type == TokenType::l_key) {
            consume(); // {
            while (peek().has_value() && peek().value().type != TokenType::r_key) {
                if (!peek().has_value() || peek().value().type != TokenType::ident) {
                    add_error("Expected a variable in list to unload", line);
                }
                vars.push_back(consume());

                if (peek().has_value() && peek().value().type == TokenType::comma) {
                    consume();
                    continue;
                }
                else break;
            }
            if (!peek().has_value() || peek().value().type != TokenType::r_key) {
                add_error("Expected '}' or ',' after identifier", line);
            }
            consume();
        }
        else {
            if (!peek().has_value() || peek().value().type != TokenType::ident) {
                add_error("Expected variable to unload", line);
            }
            vars.push_back(consume());
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        return NodeStmt{.var = NodeStmtUnload{.vars = vars, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_stop) {
        int line = peek().value().line;
        consume(); // stop

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        return NodeStmt{.var = NodeStmtStop{.line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_continue) {
        int line = peek().value().line;
        consume(); // continue

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume(); // ;

        return NodeStmt{.var = NodeStmtContinue{.line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::get &&
            peek(2).has_value() && peek(2).value().type == TokenType::ident
            && peek(3).has_value() && peek(2).value().type == TokenType::dot) {
        consume(); // get
        Token ident = consume();
        int line = ident.line;
        consume(); // .
        Token property = consume();

        int is_func = false;
        std::vector<NodeExprPtr> args;
        if (peek().has_value() && peek().value().type == TokenType::open_paren) {
            is_func = true;
            while (peek().has_value() && peek().value().type != TokenType::close_paren) {
                auto e = parse_expr();
                if (!e.has_value()) {
                    add_error("Invalid Expression", line);
                }
                args.push_back(std::make_shared<NodeExpr>(e.value()));

                if (peek().has_value() && peek().value().type == TokenType::comma) {
                    consume();
                    continue;
                }
            }
            if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
                add_error("Expected ')'", line);
            }
            consume();
        }

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt{.var = NodeStmtProperty{.ident = ident, .property = property, .is_func = is_func, .args = args, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_declmod) {
        consume(); // declmod
        int line = peek().value().line;

        if (!peek().has_value() || peek().value().type != TokenType::ident) {
            add_error("Expected name of module", line);
        }
        Token module_name = consume();
        
        return NodeStmt{.var = NodeStmtDeclmod{.module_name = module_name, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_endmod) {
        int line = peek().value().line;
        consume(); // endmod 
        
        return NodeStmt{.var = NodeStmtEndmod{.line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_umod) {
        consume(); //_umod
        int line = peek().value().line;
        if (!peek().has_value() || peek().value().type != TokenType::ident) {
            add_error("Expected module to import", line);
        }
        Token module_name = consume();

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt{.var = NodeStmtUmod{.module_name = module_name, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_ubeepmod) {
        int line = peek().value().line;
        consume();
        if (!peek().has_value() || peek().value().type != TokenType::ident) {
            add_error("Expected global module to import", line);
        }
        Token module_name = consume();

        if (!peek().has_value() || peek().value().type != TokenType::semi) {
            add_error("Expected ';'", line);
        }
        consume();

        return NodeStmt{.var = NodeStmtUbeepmod{.module_name = module_name, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_llibrary) {
        int line = peek().value().line;
        consume();

        /*if (!peek().has_value() || peek().value().type != TokenType::l_arrow) {
            add_error("Expected '<'", line);
        }
        consume();*/

        if (!peek().has_value() || peek().value().type != TokenType::str_lit) {
            add_error("Expected library path", line);
        }
        Token path = consume();

        return NodeStmt{.var = NodeStmtLlibrary{.name = path, .line = line}};
    }
    else if (peek().has_value() && peek().value().type == TokenType::_libpath) {
        int line = peek().value().line;
        consume();

        /*if (!peek().has_value() || peek().value().type != TokenType::l_arrow) {
            add_error("Expected '<'", line);
        }
        consume();*/

        if (!peek().has_value() || peek().value().type != TokenType::str_lit) {
            add_error("Expected library path", line);
        }
        Token path = consume();

        return NodeStmt{.var = NodeStmtLibpath{.path = path, .line = line}};
    }
    else {
        int line = peek().value().line;
        consume();
        add_error("Invalid Statment", line);
        return {};
    }
}


std::optional<NodeProg> Parser::parse_prog() {
    NodeProg prog;
    while(peek().has_value()) {
        if (auto stmt = parse_stmt()) {
            prog.stmts.push_back(stmt.value());
        }
    }
    return prog;
}
