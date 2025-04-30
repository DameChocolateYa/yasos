#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_map>

#include "parser.hpp"
#include "global.hpp"

#undef __FILE__
#define __FILE__ "src/generation.hpp"

static std::string escape_string(const std::string& raw);

void initialize_func_map();
void initialize_func_ret_map();

enum ArgRequired {Yes, No};

void check_func_args(const std::vector<NodeExprPtr>& args, const std::unordered_multimap<ArgType, ArgRequired>& required_args);

extern std::unordered_map<std::string, VarType> known_function_types;

enum class PrintType : int {
    Int = 0,
    Float = 1,
    Str = 2,
    CR = 3
};

enum class Mode {
    Global,
    Function
};

class Generator {
private:

public:
Mode current_mode = Mode::Global;
std::string current_func = "";
std::stringstream function_buffer, main_buffer;

struct Var {
    size_t stack_loc;
    VarType type;
    std::string name;
};

const NodeProg m_prog;
std::stringstream m_output;
size_t m_stack_size = 0;
std::unordered_map<std::string, Var> m_vars;
std::unordered_map<std::string, std::vector<Var>> m_fnc_args;
//std::unordered_map<std::string, std::vector<Var>> m_fnc_ret;
std::vector<std::string> m_string_literals;
std::vector<float> m_float_literals;

inline void write(const std::string& output) {
    if (current_mode == Mode::Function) function_buffer << output << "\n";
    else main_buffer << output << "\n";
}

void push(const std::string& reg) {
    write("  push " + reg);
    ++m_stack_size;
}

void push_float(const std::string& reg) {
    //m_output << "   sub rsp, 8\n";
    write("   movsd [rsp], " + reg);
    write("   add rsp, 8");
    ++m_stack_size;
}

void pop(const std::string& reg) {
    write( "  pop " + reg);
    if (m_stack_size == 0) {
        std::cerr << "Stack underflow!\n";
        exit(EXIT_FAILURE);
    }
    --m_stack_size;
}

void pop_float(const std::string& reg) {
    write( "   movsd " + reg + ", [rsp]");
    write("   add rsp, 8");
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
