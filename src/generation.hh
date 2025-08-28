#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <map>
#include <stack>

#include "parser.hh"
#include "global.hh"

#undef __FILE__
#define __FILE__ "src/generation.hpp"

static std::string escape_string(const std::string& raw);

void initialize_func_map();
void initialize_func_ret_map();
void initialize_str_property_map();
void initialize_str_ret_property_map();

enum ArgRequired {Yes, No};

void check_func_args(const std::vector<NodeExprPtr>& args, const std::unordered_multimap<ArgType, ArgRequired>& required_args);

extern std::unordered_map<std::string, Type> known_function_types;

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
extern std::unordered_map<std::string, Type> m_fnc_rets;

extern std::unordered_map<std::string, std::pair<Type, std::vector<Type>>> declared_funcs;

typedef enum { TYPE_INT, TYPE_CHAR, TYPE_DOUBLE } TypeTag;

typedef struct {
    TypeTag type;
    union {
        int i;
        char* s;
        double d;
    } value;
} AnyValue;

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
	    Type type;
	    std::string name;
	    int is_mutable;
		  std::string struct_template = "";
	};
  struct GlobVar {
	    NodeExpr expr;
	    Type type;
	    std::string name;
	    int is_mutable;
      bool is_declared = true;
		  std::string struct_template = ""; 
	};

	std::string filename;

	const NodeProg m_prog;
	std::stringstream m_output;
	size_t m_stack_size = 0;
  size_t m_stack_size_rel = 0; // Used only for rsp (NOT RBP)
	std::map<std::string, Var> m_vars;
	std::map<std::string, std::pair<int, int>> m_lists;
	std::unordered_map<std::string, GlobVar> m_glob_vars;
	std::vector<std::string> m_vars_order;
  std::unordered_map<std::string, std::pair<Type, std::vector<Type>>> declared_funcs;
	std::unordered_map<std::string, std::vector<Var>> m_fnc_args;
	std::unordered_map<std::string, Type> m_fnc_custom_ret;
	std::vector<std::string> m_string_literals;
	std::vector<float> m_float_literals;
	std::stack<std::string> stmt_orde;

	std::map<std::string, std::vector<std::pair<std::string, Type>>> m_structs;
	std::map<std::string, std::vector<NodeExpr>> m_vars_in_structs;
	std::vector<NodeExpr> m_struct_temp_args; // shitty way

	bool stack_aligned_in_call = false;	

	bool is_header = false; 

	std::string last_struct_template_name = "";

  bool write_stmt = true;
  std::stringstream buf;

	inline void write(const std::string& output, int newline = true) {
      if (!write_stmt) {
        buf << output << (newline ? "\n" : "");
        return;
      }

	    if (current_mode == Mode::Function) function_buffer << output << (newline ? "\n" : "");
	    else main_buffer << output << (newline ? "\n" : "");
	}

  const std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};
  enum Op {
    Mov,
    Add,
    Lea,
    Mul,
    Sub,
    Div,
  };
  inline void mov(const std::string& reg1, const std::string& reg2, Op op = Op::Mov, bool reg1_pointer = false, bool reg2_pointer = false) {
    std::stringstream buf;
    buf << "  ";
    bool is_float_op = (std::find(float_regs.begin(), float_regs.end(), reg1) != float_regs.end() || std::find(float_regs.begin(), float_regs.end(), reg2) != float_regs.end());
    switch (op) {
      case Op::Mov:
        if (is_float_op) buf << "movsd ";
        else buf << "mov ";
        break;
      case Op::Add:
        if (is_float_op) buf << "addsd ";
        else buf << "add ";
        break;
      case Op::Mul:
        if (is_float_op) buf << "imul ";
        else buf << "mulsd ";
        break;
      case Op::Div:
        break;
      case Op::Lea:
        buf << "lea ";
        break;
    }
    
    if (reg1_pointer) buf << "(%" << reg1 << ")";
    else buf << "%" << reg1;

    buf << ", ";

    if (reg2_pointer) buf << "(%" << reg2 << ")";
    else buf << "%" << reg2;
    buf << "\n";

    if (current_mode == Mode::Function) function_buffer << buf.str();
	  else main_buffer << buf.str();
  }

	inline void insert_var(const std::string& name, Type type, int is_mutable = true, const std::string& struct_template = "", bool is_globl = false, NodeExpr expr = NodeExpr(NodeExprNone {.line = 0})) {
	    if (!is_globl) {
        m_vars.insert({name, Var{.stack_loc = m_stack_size_rel, .type = type, .name = name, .is_mutable = is_mutable, .struct_template = struct_template}});
         m_vars_order.push_back(name);
      } else {
        m_glob_vars.insert({name, GlobVar{.expr = expr, .type = type, .name = name, .is_mutable = is_mutable, .struct_template = struct_template}});
      }
	}

	void push(const std::string& reg, int newline = true) {
	    //write("  push " + reg);
	    //write("  push %" + reg, newline); 
      ++m_stack_size;
      ++m_stack_size_rel;
      write("  mov %" + reg + ", " + std::to_string((m_stack_size_rel - 1) * 8) + "(%rsp)"); 
	}

	void push_float(const std::string& reg, int newline = true) {
	    write("  sub $8, %rsp");
		write("  movsd %" + reg + ", (%rsp)");
		++m_stack_size;
	}

	void pop(const std::string& reg, int newline = true) {
	    //write( "  pop " + reg);
	    //write("  pop %" + reg);
      write("  mov " + std::to_string((m_stack_size_rel - 1) * 8) + "(%rsp), %" + reg);
	    if (m_stack_size == 0) {
	        std::cerr << "Stack underflow!\n";
	        exit(EXIT_FAILURE);
	    }
	    --m_stack_size;
      --m_stack_size_rel;
	}

	void pop_float(const std::string& reg, int newline = true) {
	    write("  movsd (%rsp), %" + reg);
	    write("  add $8, %rsp", newline);
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
	    size_t offset_bytes = (m_vars.at(var_name).stack_loc - 1) * 8; // this should be the var pos
	    return offset_bytes;
	}

	inline void call(const std::string& name) {
	    // Align stack pointer in 16 bytes for System V ABI
	
	    size_t old_stack_size = m_stack_size;
	    if (((m_stack_size) % 2) != 0) {
			write("  sub $8, %rsp");
	        ++m_stack_size;
	    }

	    write("  call " + name + "@PLT");

	    // Reset stack size
	    if (m_stack_size != old_stack_size) {
	        write("  add $8, %rsp");
	        --m_stack_size;
	    }
	}

    inline explicit Generator(NodeProg root, std::string filename) : m_prog(std::move(root)), filename(filename) {}

    std::vector<std::string> libraries;
    std::vector<std::string> libpaths;

    void gen_expr(const NodeExpr& expr, bool push_result=true, const std::string& reg = "rax", bool is_value = true, bool is_func_call = false);
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
