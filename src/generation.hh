#pragma once

#include <cstdlib>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <set>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <map>
#include <stack>
#include <any>

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

extern std::set<std::string> m_preprocessor;

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
      llvm::Type* type; // This can be a pointer to i32 for example
      llvm::Type* base_type; // And this the base, for example i32 (primitive data)
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

	std::map<llvm::StructType*, std::map<std::string, Type>> m_struct_templates;
  std::map<std::string, llvm::StructType*> m_structtypes;
  std::map<llvm::Value*, llvm::StructType*> m_structs;
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

	inline void insert_var(const std::string& name, llvm::Type* type, llvm::Type* base_type, llvm::Value* var_ptr, int is_mutable = true, bool is_globl = false, std::string struct_template = "") {
      m_vars.insert({name, Var{.type = type, .base_type = base_type, .name = name, .var_ptr = var_ptr, .is_mutable = is_mutable, .is_globl = is_globl, .struct_template = struct_template}});
      m_vars_order.push_back(name); 
	} 

    inline explicit Generator(NodeProg root, std::string filename, std::unique_ptr<llvm::Module> module) : m_prog(std::move(root)), filename(filename), Builder(TheContext), ModModule(std::move(module)){}

    std::vector<std::string> libraries;
    std::vector<std::string> libpaths;

    llvm::Value* gen_expr(const NodeExpr& expr, bool as_lvalue = false, bool for_store = false);
    void gen_stmt(const NodeStmt& stmt);
    void gen_prog();
};


std::string escape_string(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char next = s[i + 1];
            if (next == 'n') {
                result += '\n';
                ++i;
            } else if (next == 't') {
                result += '\t';
                ++i;
            } else if (next == '\\') {
                result += '\\';
                ++i;
            } else {
                result += '\\';
            }
        } else {
            result += s[i];
        }
    }
    return result;
}
