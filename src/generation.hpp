#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <map>
#include <stack>

#include "parser.hpp"
#include "global.hpp"

#undef __FILE__
#define __FILE__ "src/generation.hpp"

static std::string escape_string(const std::string& raw);

void initialize_func_map();
void initialize_func_ret_map();
void initialize_str_property_map();
void initialize_str_ret_property_map();

enum ArgRequired {Yes, No};

void check_func_args(const std::vector<NodeExprPtr>& args, const std::unordered_multimap<ArgType, ArgRequired>& required_args);

extern std::unordered_map<std::string, VarType> known_function_types;

extern int main_func_declared;

enum class PrintType : int {
    Int = 0,
    Float = 1,
    Str = 2,
    CR = 3
};

enum class Mode {
    Global,
    Function,
    Mod,
};

extern std::vector<std::string> m_mod;
extern std::unordered_map<std::string, std::string> m_fnc_mod;
extern std::unordered_map<std::string, VarType> m_fnc_rets;

class Generator {
private:

public:
	Mode current_mode = Mode::Global;
	std::string current_func = "";
	Mode mod;
	std::string current_mod = "";
	std::stringstream function_buffer, main_buffer;

	struct Var {
	    size_t stack_loc;
	    VarType type;
	    std::string name;
	    int is_mutable;
	};

	std::string filename;

	const NodeProg m_prog;
	std::stringstream m_output;
	size_t m_stack_size = 0;
	std::map<std::string, Var> m_vars;
	std::vector<std::string> m_vars_order;
	std::unordered_map<std::string, std::vector<Var>> m_fnc_args;	
	//std::unordered_map<std::string, std::vector<Var>> m_fnc_ret;
	std::vector<std::string> m_string_literals;
	std::vector<float> m_float_literals;
	std::vector<std::string> m_structs;
	std::stack<std::string> stmt_orde; 

	inline void write(const std::string& output, int newline = true) {
	    if (current_mode == Mode::Function) function_buffer << output << (newline ? "\n" : "");
	    else main_buffer << output << (newline ? "\n" : "");
	}

	inline void insert_var(const std::string& name, VarType type, int is_mutable) {
	    m_vars.insert({name, Var{.stack_loc = m_stack_size - 1, .type = type, .name = name, .is_mutable = is_mutable}});
	    m_vars_order.push_back(name);
	}

	void push(const std::string& reg, int newline = true) {
	    //write("  push " + reg);
	    write("  sub $16, %rsp");
	    write("  mov %" + reg + ", (%rsp)", newline);
	    ++m_stack_size;
	}

	void push_float(const std::string& reg, int newline = true) {
	    write("  sub $16, %rsp");                          // hacer espacio en la pila
	    write("  movsd %" + reg + ", (%rsp)", newline);     // guardar el valor float
	    ++m_stack_size;
	}

	void pop(const std::string& reg, int newline = true) {
	    //write( "  pop " + reg);
	    write("  mov (%rsp), %" + reg);
	    write("  add $16, %rsp");
	    if (m_stack_size == 0) {
	        std::cerr << "Stack underflow!\n";
	        exit(EXIT_FAILURE);
	    }
	    --m_stack_size;
	}

	void pop_float(const std::string& reg, int newline = true) {
	    write("  movsd (%rsp), %" + reg);
	    write("  add $16, %rsp", newline);
	    if (m_stack_size == 0) {
	        std::cerr << "Float stack underflow!\n";
	        exit(EXIT_FAILURE);
	    }
	    --m_stack_size;
	}

	size_t get_var(const std::string& var_name) {
	    if (!m_vars.contains(var_name)) {
	        std::cerr << "Error trying to get an undeclared variable (" << var_name << ")\n";
	        exit(EXIT_FAILURE);
	    }
	    size_t offset_bytes = (m_stack_size - m_vars.at(var_name).stack_loc - 1) * 16; // this should be the var pos
	    return offset_bytes;
	}

	inline void call(const std::string& name) {
	    write("  call *" + name + "@GOTPCREL(%rip)");
	}

    inline explicit Generator(NodeProg root, std::string filename) : m_prog(std::move(root)), filename(filename) {}

    std::vector<std::string> libraries;
    std::vector<std::string> libpaths;

    void gen_expr(const NodeExpr& expr, bool push_result=true, const std::string& reg = "rax");
    void gen_stmt(const NodeStmt& stmt);
    [[nodiscard]] std::string gen_prog();
};

static std::string escape_string(const std::string& raw) {
    std::string out;
    for (char c : raw) {
        switch (c) {
            case '"':  out += "\\\""; break;
            //case '\\': out += "\\\\"; break;
            default:   out += c;
        }
    }
    return out;
}
