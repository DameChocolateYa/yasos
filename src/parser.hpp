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

#undef __FILE__
#define __FILE__ "src/parser.hpp"

enum class VarType {
    Int = 0,
    Float = 1,
    Str = 2
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

struct NodeExprCR {
    Token token;
};

struct CustomFuncArgs {
    std::string name;
    ArgType arg_type;
};

struct NodeExprCallCustomFunc {
    Token name;
    std::vector<Token> arg_names;
    std::vector<NodeExpr> arg_values;
};

struct NodeExprBoolValue {
    int value;
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
        NodeExprNone,
        NodeExprCR,
        NodeExprCallCustomFunc,
        NodeExprBoolValue
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

struct NodeStmtMkpub {
    std::vector<Token> functions;
};

struct NodeStmt;

struct NodeStmtIf {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
    std::optional<std::variant<std::shared_ptr<NodeStmtIf>, std::vector<NodeStmt>>> else_branch;
};

struct NodeStmtWhile {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
};

struct NodeStmtPrint {
    Token str_lit;
    std::vector<NodeExpr> args;
};

struct NodeStmtDefFunc {
    Token name;
    std::vector<CustomFuncArgs> args;
};

struct NodeStmtEndfn {
    // We dont need anything here...
};

struct NodeStmtRet { // Return a value
    NodeExpr value;
};

struct NodeStmtCallCustomFunc {
    Token name;
    //std::vector<NodeExpr> args;
    std::vector<Token> arg_names;
    std::vector<NodeExpr> arg_values;
};

struct NodeStmtUnload {
    std::vector<Token> vars;
};

struct NodeStmtStop {
    // We dont need anything here...
};

struct NodeStmt {
    std::variant<NodeStmtVar, NodeStmtCall, NodeStmtImport, NodeStmtUse, NodeStmtIf, NodeStmtWhile, NodeStmtPrint, NodeStmtDefFunc, NodeStmtEndfn, NodeStmtRet, NodeStmtCallCustomFunc, NodeStmtMkpub, NodeStmtUnload, NodeStmtStop> var;
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
        std::optional<NodeExpr> parse_primary_expr();
        std::optional<NodeExpr> parse_expr();
        std::optional<NodeStmt> parse_stmt();
        std::optional<NodeProg> parse_prog();
};
