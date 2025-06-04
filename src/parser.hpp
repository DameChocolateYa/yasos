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
    Void = -1,
    Int = 0,
    Float = 1,
    Str = 2,
	Other = 3,
};

struct NodeExpr;
using NodeExprPtr = std::shared_ptr<NodeExpr>;

struct NodeExprIntLit {
    Token int_lit;
    int line;
};

struct NodeExprIdent {
    Token ident;
    int line;
};

struct NodeExprStrLit {
    Token str_lit;
    int line;
};

struct NodeExprFloatLit {
    Token float_lit;
    int line;
};

struct NodeExprNone {
    Token none;
    int line;
};

struct NodeExprNoArg {
    Token no_arg;
    int line;
};

struct NodeExprBinary {
    NodeExprPtr lhs;
    Token op_token;
    NodeExprPtr rhs;
    int line;
};

struct NodeExprBinaryAssign {
    Token left_token;
    Token op_token;
    NodeExprPtr right_expr;
    int line;
};

struct NodeExprUnaryIncDec {
    Token ident;
    Token op_token;
    int line;
};

struct NodeExprCall {
    Token name;
    std::vector<NodeExprPtr> args;
    int line;
};

struct NodeExprCR {
    Token token;
    int line;
};

struct CustomFuncArgs {
    std::string name;
    ArgType arg_type;
    int line;
};

struct NodeExprBoolValue {
    int value;
    int line;
};

struct NodeExprProperty {
    Token ident;
    Token property;

    int is_func = false;
    std::vector<NodeExprPtr> args; // optional
    
    int line;
};

struct NodeExprGetPtr {
	Token ident;

	int line;
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
        NodeExprBoolValue,
        NodeExprProperty,
		NodeExprGetPtr
    > var;

    NodeExpr() = default;

    template<typename T>
    NodeExpr(T val) : var(std::move(val)) {}

    int line;
};

struct NodeExit {
    NodeExpr expr;
    int line;
};

struct NodeStmtVar {
    Token ident;
    NodeExpr expr;
    int is_mutable;
    int line;
};

struct NodeStmtVarRe {
    Token ident;
    VarType type;
    NodeExpr expr;
    int line;
};

struct NodeStmtCall {
    Token name;
    std::vector<NodeExprPtr> args;
    int line;
};

struct NodeStmtUse {
    std::vector<Token> use;
    int line;
};

struct NodeStmtImport {
    Token to_import;
    int line;
};

struct NodeStmtDeclmod {
    Token module_name;
    int line;
};

struct NodeStmtEndmod {
    int line;
};

struct NodeStmtUmod {
    Token module_name;
    int line;
};

struct NodeStmtUbeepmod {
    Token module_name;
    int line;
};

struct NodeStmtMkpub {
    std::vector<Token> functions;
    int line;
};

struct NodeStmt;

struct NodeStmtIf {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
    std::optional<std::variant<std::shared_ptr<NodeStmtIf>, std::vector<NodeStmt>>> else_branch;
    int line;
};

struct NodeStmtWhile {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
    std::vector<NodeStmt> bfw; // before while
    std::vector<NodeStmt> afi; // after iteration
    int line;
};

struct NodeStmtPrint {
    NodeExpr str;
    std::vector<NodeExpr> args;
    int line;
};

struct NodeStmtDefFunc {
    Token name;
    std::vector<CustomFuncArgs> args;
    VarType return_type;
	std::vector<NodeStmt> code_branch;
    int line;
};

struct NodeStmtEndfn {
    // We dont need anything here...
    int line;
};

struct NodeStmtRet { // Return a value
    NodeExpr value;
    int line;
};

struct NodeStmtUnload {
    std::vector<Token> vars;
    int line;
};

struct NodeStmtStop {
    // We dont need anything here...
    int line;
};

struct NodeStmtContinue {
    // We dont need anything here...
    int line;
};

struct NodeStmtProperty {
    Token ident;
    Token property;
    
    int is_func = false;
    std::vector<NodeExprPtr> args; // optional
    
    int line;
};

struct NodeStmtLlibrary {
    Token name;
    int line;
};
struct NodeStmtLibpath {
    Token path;
    int line;
};

struct NodeStmtSetPtr {
	Token ident;
	NodeExpr expr;

	int line;
};

struct NodeStmtGlobl {
	Token ident;
	int line;
};

struct NodeStmt {
    std::variant<NodeStmtVar, NodeStmtVarRe, NodeStmtCall, NodeStmtImport, NodeStmtUse, NodeStmtIf, NodeStmtWhile, NodeStmtPrint, NodeStmtDefFunc, NodeStmtEndfn, NodeStmtRet, NodeStmtMkpub, NodeStmtUnload, NodeStmtStop, NodeStmtContinue, NodeStmtProperty, NodeStmtDeclmod, NodeStmtEndmod, NodeStmtUmod, NodeStmtUbeepmod, NodeStmtLlibrary, NodeStmtLibpath, NodeStmtSetPtr, NodeStmtGlobl> var;
    int line;
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
