#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>
#include <optional>
#include <variant>
#include <memory>
#include <iostream>

#include "lexer.hh"

#undef __FILE__
#define __FILE__ "src/parser.hh"

struct Type {
  enum class Kind {
    Int,
    Float,
    Str,
    Char,
    None,
    List,
    Struct,
    Var,
	  Any,
	  Ptr,
	  NoArg,
	  NxtUndefNum,
    UserDefined
  };

  Kind kind; 
  bool is_ref = false; // if type has & before type ident
  std::string user_type = "";
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

struct NodeExprCharLit {
  Token char_lit;
  int line;
};

struct NodeExprFloatLit {
    Token float_lit;
    int line;
};

struct NodeExprNone {
    int line;
};

struct NodeExprNullptr {
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

struct NodeExprUnary {
    Token op;
    std::shared_ptr<NodeExpr> expr;
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
    Type arg_type;
    int line;
};

struct NodeExprBoolValue {
    int value;
    int line;
};

struct NodeExprProperty {
    std::shared_ptr<NodeExpr> base;
    Token property;

    int is_func = false;
    std::vector<NodeExprPtr> args; // optional
    
    int line;
};

struct NodeExprGetPtr {
	Token ident;

	int line;
};

struct NodeExprDeref {
  NodeExprPtr expr;
  int line;
};

struct NodeExprList {
	std::vector<NodeExpr> elements;

	int line;
};

struct NodeExprListElement {
	NodeExprPtr list_expr;
	NodeExprPtr index;
	int line;
};

struct NodeExprStruct {
	Token template_name;
	std::vector<NodeExprPtr> fields;
	int line;
};

struct NodeExprNew {
  Type type;
  int line;
};

struct NodeExprIsDef {
  Token name;
  int line;
};

struct NodeExprIsNotDef {
  Token name;
  int line;
};

struct NodeExprSizeOf {
  Type type;
  int line;
};

struct NodeExprCast {
  NodeExprPtr expr;
  Type target_type;
  int line;
};

struct NodeExpr {
    std::variant<
        NodeExprIntLit,
        NodeExprIdent,
        NodeExprStrLit,
        NodeExprCharLit,
        NodeExprFloatLit,
        NodeExprCall,
        NodeExprBinary,
        NodeExprBinaryAssign,
        NodeExprUnary,
        NodeExprUnaryIncDec,
        NodeExprNoArg,
        NodeExprNone,
        NodeExprNullptr,
        NodeExprCR,
        NodeExprBoolValue,
        NodeExprProperty,
		NodeExprGetPtr,
    NodeExprDeref,
		NodeExprList,
		NodeExprListElement,
		NodeExprStruct,
    NodeExprNew,
    NodeExprIsDef,
    NodeExprIsNotDef,
    NodeExprSizeOf,
    NodeExprCast
    > var;

    NodeExpr() = default;
    virtual ~NodeExpr() = default;

    template<typename T>
    NodeExpr(T val) : var(std::move(val)) {} 

    int line;
};

struct NodeExit {
    NodeExpr expr;
    int line;
};

struct NodeStmtAsmUserWrite {
  std::vector<std::string> format_instructions;
  std::vector<std::vector<NodeExpr>> exprs;
  int line;
};

struct NodeStmtAssign {
  NodeExpr target;
  Token op_tok;
  NodeExpr value;
  int line;
};

struct NodeStmtVar {
    Token ident;
    Type type = Type{Type::Kind::None};
    NodeExpr expr;
	  int has_initial_value = true;
    int is_mutable;
    int line;
};

struct NodeStmtVarRe {
    Token ident;
    Type type;
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
    Token mod_name;
    std::vector<std::string> to_import;
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
    std::vector<NodeExpr> elif_conditions;
    std::vector<std::vector<NodeStmt>> elif_branches;
    std::vector<NodeStmt> else_branch;
    int line;
};

struct NodeStmtWhile {
    NodeExpr condition;
    std::vector<NodeStmt> then_branch;
    std::vector<NodeStmt> bfw; // before while
    std::vector<NodeStmt> afi; // after iteration
    int line;
};

struct NodeStmtLoop {
  std::vector<NodeStmt> then_branch;
  int line;
};

struct NodeStmtFor {
  std::vector<NodeStmt> init;
  NodeExpr condition;
  std::vector<NodeStmt> update;
  std::vector<NodeStmt> code_branch;
  int line;
};

struct NodeStmtDefFunc {
    Token name;
    std::vector<CustomFuncArgs> args;
    Type return_type;
	std::vector<NodeStmt> code_branch;
	bool is_pub = false;
	bool is_extern = false;
	std::vector<std::string> absolute_type_name_args;
  bool is_vargs;
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
    NodeExpr expr; 
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

struct NodeStmtHeader {
    int line;
};

struct NodeStmtUhead {
    Token mod_name;
    std::vector<std::string> to_import;
    int line;
};

struct NodeStmtLeave {
	int line;
};

struct NodeStmtListElement {
	Token list_name;
	NodeExpr index;
	NodeExpr expr;
	int line;
};

struct NodeStmtStruct {
	Token name;
	std::vector<std::pair<std::string, Type>> fields;
	int line;
};

enum DefineType {
  Cond,
  Value,
  Func,
};

struct NodeStmtDefine {
  Token name;
  int line;
};

struct NodeStmtUndef {
  Token name;
  int line;
};

struct NodeStmtPreprocessorCond {
  Token condition;
  std::vector<NodeStmt> then_branch;
  std::vector<Token> elif_conditions;
  std::vector<std::vector<NodeStmt>> elif_branches;
  std::vector<NodeStmt> else_branch;
  int line;
};

struct NodeStmtPreError {
  std::string err_msg;
  int line;
};
struct NodeStmtPreWarning {
  std::string warn_msg;
  int line;
};

struct NodeStmtPrint {
  Token fmt;
  std::vector<NodeExpr> args;
  int line;
};

struct NodeStmt {
    std::variant<NodeStmtAsmUserWrite, NodeStmtAssign, NodeStmtVar, NodeStmtVarRe, NodeStmtCall, NodeStmtImport, NodeStmtUse, NodeStmtIf, NodeStmtWhile, NodeStmtLoop, NodeStmtFor, NodeStmtDefFunc, NodeStmtEndfn, NodeStmtRet, NodeStmtMkpub, NodeStmtUnload, NodeStmtStop, NodeStmtContinue, NodeStmtProperty, NodeStmtDeclmod, NodeStmtEndmod, NodeStmtUmod, NodeStmtUbeepmod, NodeStmtLlibrary, NodeStmtLibpath, NodeStmtSetPtr, NodeStmtGlobl, NodeStmtHeader, NodeStmtUhead, NodeStmtLeave, NodeStmtListElement, NodeStmtStruct, NodeStmtDefine, NodeStmtUndef, NodeStmtPreprocessorCond, NodeStmtPreError, NodeStmtPreWarning, NodeStmtPrint> var;
    int line;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
    NodeExpr expr;
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
        std::optional<NodeExpr> parse_property_chain(std::optional<NodeExpr> base_expr = std::nullopt); 
        std::optional<NodeExpr> parse_primary_expr();
        std::optional<NodeExpr> parse_expr(int min_precedence = 0);
        std::optional<NodeStmt> parse_stmt();
        std::optional<NodeProg> parse_prog(bool parse_only_one_expr = false);
};
