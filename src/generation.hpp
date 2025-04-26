#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include "parser.hpp"
#include "global.hpp"
#include "tokenization.hpp"

#undef __FILE__
#define __FILE__ "src/generation.hpp"

static std::string escape_string(const std::string& raw);

void initialize_func_map();
void initialize_func_ret_map();

enum ArgType {
    NoArg,
    None,
    Integer,
    String,
    Float,
    OptionalInteger,
    OptionalString,
    OptionalFloat,
    VariableValue,
    NxtUndefNum
};

enum ArgRequired {Yes, No};

void check_func_args(const std::vector<NodeExprPtr>& args, const std::unordered_multimap<ArgType, ArgRequired>& required_args);

extern std::unordered_map<std::string, VarType> known_function_types;

enum class PrintType : int {
    Int = 0,
    Float = 1,
    Str = 2,
    CR = 3
};

class Generator {
private:

public:
struct Var {
    size_t stack_loc;
    VarType type;
};

const NodeProg m_prog;
std::stringstream m_output;
size_t m_stack_size = 0;
std::unordered_map<std::string, Var> m_vars;
std::vector<std::string> m_string_literals;
std::vector<float> m_float_literals;

void push(const std::string& reg) {
    m_output << "  push " << reg << "\n";
    ++m_stack_size;
}

void push_float(const std::string& reg) {
    //m_output << "   sub rsp, 8\n";
    m_output << "   movsd [rsp], " << reg << "\n";
    m_output << "   add rsp, 8\n";
    ++m_stack_size;
}

void pop(const std::string& reg) {
    m_output << "  pop " << reg << "\n";
    if (m_stack_size == 0) {
        std::cerr << "Stack underflow!\n";
        exit(EXIT_FAILURE);
    }
    --m_stack_size;
}

void pop_float(const std::string& reg) {
    m_output << "   movsd " << reg << ", [rsp]\n";
    m_output << "   add rsp, 8\n";
    if (m_stack_size == 0) {
        std::cerr << "Stack underflow!\n";
        exit(EXIT_FAILURE);
    }
    --m_stack_size;
}

size_t get_var(const std::string& var_name) {
    if (!m_vars.contains(var_name)) {
        std::cerr << "Error trying to get an undeclared variable\n";
        exit(EXIT_FAILURE);
    }
    size_t offset_bytes = (m_stack_size - m_vars.at(var_name).stack_loc - 1) * 8; // this should be the var pos
    return offset_bytes;
}

    inline explicit Generator(NodeProg root) : m_prog(std::move(root)) {}

    std::vector<std::string> libraries;

    void gen_expr(const NodeExpr& expr);
    void gen_stmt(const NodeStmt& stmt);
    [[nodiscard]] std::string gen_prog();
};

static std::string escape_string(const std::string& raw) {
    std::string out;
    for (char c : raw) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:   out += c;
        }
    }
    return out;
}
