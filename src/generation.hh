#pragma once

#include <cstdlib>
#include <llvm/IR/BasicBlock.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <map>
#include <stack>

#include "parser.hh"
#include "global.hh"
#include "error.hh"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

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

extern std::unique_ptr<llvm::Module> TheModule;
extern llvm::LLVMContext TheContext;

class Generator {
private:

public:
	Mode current_mode = Mode::Global;
	std::string current_func = "";
	Mode mod;
	std::string current_mod = "";
	std::stringstream function_buffer, main_buffer;
 
  llvm::IRBuilder<> Builder; 
  std::unique_ptr<llvm::Module> ModModule;

	struct Var {
      llvm::Type* type; 
	    std::string name;
      llvm::Value* var_ptr;
	    bool is_mutable;
      bool is_globl;
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
  std::unordered_map<std::string, std::pair<llvm::Type*, std::vector<llvm::Type*>>> declared_funcs;
	std::unordered_map<std::string, std::vector<Var>> m_fnc_args;
	std::unordered_map<std::string, Type> m_fnc_custom_ret;
	std::vector<std::string> m_string_literals;
	std::vector<float> m_float_literals;
	std::stack<std::pair<llvm::BasicBlock*, std::pair<llvm::BasicBlock*, llvm::BasicBlock*>>> stmt_orde; // 1 - start | 2- end | 3- update (for)

	std::map<std::string, std::vector<std::pair<std::string, Type>>> m_structs;
	std::map<std::string, std::vector<NodeExpr>> m_vars_in_structs;
	std::vector<NodeExpr> m_struct_temp_args; // shitty way

	bool stack_aligned_in_call = false;	

	bool is_header = false; 

	std::string last_struct_template_name = "";

  bool write_stmt = true;
  std::stringstream buf;

  static size_t size_of(Type::Kind type) {
    switch (type) {
      case Type::Kind::Int: return 4;
      case Type::Kind::Str: return 8;
      case Type::Kind::Float: return 8;
      case Type::Kind::Any: return 8;
      case Type::Kind::None: return 1;
      default: return 8;
    }

    return 8;
  }

  static std::string reg_transform(const std::string& reg, size_t bytes) {
  }

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
 
  
inline void emit_op(
    const std::string& src,
    const std::string& dst,
    Op op,                 // Mov, Add, Sub, Mul, Div, Lea
    size_t bytes = 8,
    bool src_float = false,
    bool dst_float = false,
    bool is_ref = false
) {
    std::stringstream output;
    output << "  ";

    // Elegir mnemónico según tipo
    if (src_float || dst_float) {
        switch(op) {
            case Op::Mov: output << "movsd "; break;
            case Op::Add: output << "addsd "; break;
            case Op::Sub: output << "subsd "; break;
            case Op::Mul: output << "mulsd "; break;
            case Op::Div: output << "divsd "; break;
            case Op::Lea: output << "lea "; break;
        }
    } else {
        switch(op) {
            case Op::Mov: output << (bytes == 4 && !is_ref ? "movl " : "movq "); break;
            case Op::Add: output << (bytes == 4 && !is_ref ? "addl " : "addq "); break;
            case Op::Sub: output << (bytes == 4 && !is_ref ? "subl " : "subq "); break;
            case Op::Mul: output << (bytes == 4 && !is_ref ? "imull " : "imul "); break;
            case Op::Div: output << (bytes == 4 && !is_ref ? "idivl " : "idivq "); break;
            case Op::Lea: output << "lea "; break;
        }
    }

    // Helper para formatear operandos
    auto fmt_op = [bytes, is_ref](const std::string& s) -> std::string {
        // Si es memoria (contiene paréntesis), no añadimos %
        if (s.find('(') != std::string::npos) return s;

        // Ajustar registros según bytes
        if (bytes == 4 && !is_ref) {
            if (s == "rax") return "%eax";
            if (s == "rdi") return "%edi";
            if (s == "rsi") return "%esi";
            if (s == "rdx") return "%edx";
            if (s == "rcx") return "%ecx";
            if (s == "rbx") return "%ebx";
            if (s == "rbp") return "%ebp";
            if (s == "rsp") return "%esp";
            if (s == "r8") return "%r8d";
            if (s == "r9") return "%r9d";
            if (s == "r10") return "%r10d";
            if (s == "%r10") return "%r10d";
        } else {
          if (s == "rax") return "%rax";
          if (s == "rdi") return "%rdi";
          if (s == "rsi") return "%rsi";
          if (s == "rdx") return "%rdx";
          if (s == "rcx") return "%rcx";
          if (s == "rbx") return "%rbx";
          if (s == "rbp") return "%rbp";
          if (s == "rsp") return "%rsp";
          if (s == "r8") return "%r8";
          if (s == "r9") return "%r9";
          if (s == "r10") return "%r10";
          if (s == "%r10") return "%r10";
        }
        return s;
    };
    output << fmt_op(src) << ", " << fmt_op(dst);

    if (!write_stmt) {
        buf << output.str() << "\n";
        return;
    }

    if (current_mode == Mode::Function) function_buffer << buf.str();
    else main_buffer << buf.str();
}

	inline void insert_var(const std::string& name, llvm::Type* type, llvm::Value* var_ptr, int is_mutable = true, bool is_globl = false, std::string struct_template = "") {
      m_vars.insert({name, Var{.type = type, .name = name, .var_ptr = var_ptr, .is_mutable = is_mutable, .is_globl = is_globl, .struct_template = struct_template}});
      m_vars_order.push_back(name); 
	}

  std::vector<std::vector<size_t>> empty_offsets = {};
  std::vector<size_t> last_offset;

  
  size_t push(const std::string& reg, size_t bytes = 4, bool is_ref = false, int newline = true) {
    for (auto it = empty_offsets.begin(); it != empty_offsets.end(); ++it) {
      if (it->size() >= bytes && it->front() % 8 == 0) {
        size_t pos = it->front();   // primer offset libre
        //write("  movl %" + reg + ", " + std::to_string(pos) + "(%rsp)");
        //write("  mov %" + reg + ", " + std::to_string(pos) + "(%rsp)");
        emit_op(reg, std::to_string(pos) + "(%rsp)", Op::Mov, bytes, false, false, is_ref);
        it->erase(it->begin(), it->begin() + bytes); // consumir hueco
        if (it->empty()) empty_offsets.erase(it);
        last_offset.push_back(pos);
        return pos;
      }
    }

    if ((m_stack_size_rel % 8) != 0) {
      empty_offsets.push_back({});
      while ((m_stack_size_rel % 8) != 0) {
        empty_offsets.back().push_back({m_stack_size_rel});
        ++m_stack_size_rel;
        ++m_stack_size;
      }
    }

    // 3. Asignar nuevo espacio
    //write("  mov %" + reg + ", " + std::to_string(m_stack_size_rel) + "(%rsp)");
    emit_op(reg, std::to_string(m_stack_size_rel) + "(%rsp)", Op::Mov, bytes, false, false, is_ref);
    size_t pos = m_stack_size_rel;
    m_stack_size += bytes;
    m_stack_size_rel += bytes;
    last_offset.push_back(pos);

    return pos; // for insert var
}

	void push_float(const std::string& reg, int newline = true) {
	    write("  sub $8, %rsp");
		write("  movsd %" + reg + ", (%rsp)");
		++m_stack_size;
	}

	void pop(const std::string& reg, size_t bytes = 4, int newline = true) {
    m_stack_size_rel -= bytes;
    m_stack_size -= bytes;
    emit_op(std::to_string(last_offset.back()) + "(%rsp)", reg, Op::Mov, bytes);
    last_offset.pop_back();
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

	size_t get_var(const std::string& var_name, size_t line) {
	    if (!m_vars.contains(var_name)) {
	        add_error("Error trying to get an undeclared variable (" + var_name + ")", line);
	        exit(EXIT_FAILURE);
	    }
	    //size_t offset_bytes = (m_vars.at(var_name).stack_loc); // this should be the var pos
	    return 0;
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

    inline explicit Generator(NodeProg root, std::string filename, std::unique_ptr<llvm::Module> module) : m_prog(std::move(root)), filename(filename), Builder(TheContext), ModModule(std::move(module)){}

    std::vector<std::string> libraries;
    std::vector<std::string> libpaths;

    llvm::Value* gen_expr(const NodeExpr& expr, bool push_result=true, const std::string& reg = "rax", bool is_value = true, bool is_func_call = false);
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
