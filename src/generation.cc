#include "generation.hh"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <variant>
#include <cstring>
#include <filesystem>
#include <any>

#include "global.hh"
#include "parser.hh"
#include "error.hh"

namespace fs = std::filesystem;

int main_func_declared = false;

std::vector<std::string> modules;

std::vector<std::string> m_mod;
std::unordered_map<std::string, std::string> m_fnc_mod;
std::unordered_map<std::string, Type> m_fnc_rets;

struct Func {
    std::string name;
    std::vector<ArgType> args;
};

std::unordered_map<std::string, std::function<void(const NodeStmtCall&, Generator*)>> function_handlers;
std::unordered_map<std::string, std::function<void(const NodeExprCall&, Generator*)>> function_ret_handlers;
std::vector<Func> decfuncs;

std::unordered_map<std::string, std::function<void(const NodeExprProperty&, Generator*, int)>> str_ret_property;
std::unordered_map<std::string, std::function<void(const NodeStmtProperty&, Generator*, int)>> str_property;

static bool is_int(const NodeExpr& expr) { return std::holds_alternative<NodeExprIntLit>(expr.var); }
static bool is_str(const NodeExpr& expr) { return std::holds_alternative<NodeExprStrLit>(expr.var); }
static bool is_float(const NodeExpr& expr) { return std::holds_alternative<NodeExprFloatLit>(expr.var); }
static bool is_ident(const NodeExpr& expr) { return std::holds_alternative<NodeExprIdent>(expr.var); }
static bool is_call(const NodeExpr& expr) { return std::holds_alternative<NodeExprCall>(expr.var); }

static Type replace_user_defined_type_with_generic(Type type, Generator* gen) {
  if (type.kind != Type::Kind::UserDefined) return type;

  const std::string& name = type.user_type;

  // Is a struct?
  for (const auto& strct : gen->m_structs) {
    if (strct.first == name) return Type{Type::Kind::Struct};
  }

  return type;
}

static Type get_var_type(NodeExprIdent ident, Generator* gen) {
  const std::string& name = ident.ident.value.value();

  if (gen->m_vars.contains(name)) {
    return gen->m_vars.at(name).type;
  }
  else if (gen->current_mode == Mode::Function) {
      if (!gen->m_fnc_args.contains(gen->current_func)) {
          return Type{Type::Kind::None};
      }
      for (const auto& arg : gen->m_fnc_args.at(gen->current_func)) {
          if (arg.name == name) {
              return arg.type;
          }
      }
      return Type{Type::Kind::None};
  }

  return Type{Type::Kind::None};
}

static Type check_value(const NodeExpr& expr, Generator* gen, bool get_raw_value = false) {
    if (std::holds_alternative<NodeExprIntLit>(expr.var)) return Type{Type::Kind::Int};
    else if (std::holds_alternative<NodeExprStrLit>(expr.var)) return Type{Type::Kind::Str};
    else if (std::holds_alternative<NodeExprFloatLit>(expr.var)) return Type{Type::Kind::Float};
    else if (std::holds_alternative<NodeExprCall>(expr.var)) {
        NodeExprCall expr_call = std::get<NodeExprCall>(expr.var);
        const std::string& name = expr_call.name.value.value();
        if (gen->declared_funcs.contains(name)) {
			for (const auto& fnc : gen->declared_funcs) {
				if (fnc.first == name) return fnc.second.first;
			}
		}
		return Type{Type::Kind::None};
    }
    else if (std::holds_alternative<NodeExprIdent>(expr.var)) {
      if (get_raw_value) return Type{Type::Kind::Var};
        NodeExprIdent ident = std::get<NodeExprIdent>(expr.var);
        const std::string& name = ident.ident.value.value();

        if (gen->m_glob_vars.contains(name)) {
          return gen->m_glob_vars.at(name).type;
        }

        if (gen->m_vars.contains(name)) {
            return gen->m_vars.at(name).type;
        }
        else if (gen->current_mode == Mode::Function) {
            if (!gen->m_fnc_args.contains(gen->current_func)) {
                return Type{Type::Kind::None};
            }
            for (const auto& arg : gen->m_fnc_args.at(gen->current_func)) {
                if (arg.name == name) {
                    return arg.type;
                }
            }
            return Type{Type::Kind::None};
        }
		else if (gen->m_glob_vars.contains(name)) return gen->m_glob_vars.at(name).type;
        else {
            return Type{Type::Kind::None};
        }
    }
    else if (std::holds_alternative<NodeExprBinaryAssign>(expr.var)) {
        NodeExprBinaryAssign expr_bin = std::get<NodeExprBinaryAssign>(expr.var);
        if (gen->current_mode == Mode::Function) {
            if (!gen->m_fnc_args.contains(gen->current_func)) {
                return Type{Type::Kind::None};
            }
            for (const auto& arg : gen->m_fnc_args.at(gen->current_func)) {
                if (arg.name == expr_bin.left_token.value.value()) {
                    return arg.type;
                }
            }

        }

        return gen->m_vars.at(expr_bin.left_token.value.value()).type;
    }
    else if (std::holds_alternative<NodeExprBinary>(expr.var)) {
        NodeExprBinary expr_bin = std::get<NodeExprBinary>(expr.var);
        Type v1, v2;
        v1 = check_value(*expr_bin.lhs, gen);
        v2 = check_value(*expr_bin.rhs, gen);
        if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) return Type{Type::Kind::Float};
        else return v1;
    } else if (std::holds_alternative<NodeExprUnary>(expr.var)) {
        NodeExprUnary expr_bin = std::get<NodeExprUnary>(expr.var);
        Type v = check_value(*expr_bin.expr, gen);
        return v;
    } else if (std::holds_alternative<NodeExprDeref>(expr.var)) {
      NodeExprDeref expr_deref = std::get<NodeExprDeref>(expr.var);
      Type v = check_value(expr_deref.expr->var, gen);
      return v;
    }
    else if (std::holds_alternative<NodeExprUnaryIncDec>(expr.var)) {
        return Type{Type::Kind::Int};
    }
    else if (std::holds_alternative<NodeExprNone>(expr.var)) {
        return Type{Type::Kind::None};
    }
	else if (std::holds_alternative<NodeExprList>(expr.var)) {
		return Type{Type::Kind::List};
	}
	else if (std::holds_alternative<NodeExprListElement>(expr.var)) {
		return Type{Type::Kind::Int};
	} else if (std::holds_alternative<NodeExprStruct>(expr.var)) return Type{Type::Kind::Struct};

    return Type{Type::Kind::None};
}

static void set_var(Generator* gen, NodeExprIdent expr_ident, const std::string& reg, bool is_deref = false) {
  const std::string& name = expr_ident.ident.value.value();
  std::string e = "";

  if (gen->m_glob_vars.contains(name)) {
    Type type = gen->m_glob_vars.at(name).type;
    if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct) 
      gen->write("  mov %" + reg + ", " + name + "(%rip)");
    else if (type.kind == Type::Kind::Float) gen->write("  movsd %" + reg + ", " + name + "(%rip)");
      return;
  }
  if (gen->m_vars.contains(name) && !is_deref) {
    Type type = gen->m_vars.at(name).type;
    size_t offset_bytes = gen->get_var(name, expr_ident.line);
    if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct)
      gen->write("  mov %" + reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
    else if (type.kind == Type::Kind::Float)
      gen->write("  movsd %" + reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
  }
}

static void set_value(Generator* gen, const std::string& reg, NodeExpr target_expr, NodeExpr value_expr = NodeExpr(NodeExprNone {.line = 0})) {
  if (std::holds_alternative<NodeExprIdent>(target_expr.var)) {
    NodeExprIdent expr_ident = std::get<NodeExprIdent>(target_expr.var);
    set_var(gen, expr_ident, reg, false);
  } else if (std::holds_alternative<NodeExprDeref>(target_expr.var)) {
    NodeExprDeref expr_deref = std::get<NodeExprDeref>(target_expr.var);
    if (std::holds_alternative<NodeExprIdent>(expr_deref.expr->var))
      set_var(gen, std::get<NodeExprIdent>(expr_deref.expr->var), reg, true);
  } else if (std::holds_alternative<NodeExprListElement>(target_expr.var)) {
    NodeExprListElement expr_list_ele = std::get<NodeExprListElement>(target_expr.var);
    const std::string& name = expr_list_ele.list_name.value.value(); 

    if (gen->m_glob_vars.contains(name)) {
      Type type = gen->m_glob_vars.at(name).type;
      if (reg != "r10") gen->write("  mov %" + reg + ", %r10");
      if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct) {
        gen->gen_expr(*expr_list_ele.index, false, "rax");
        gen->write("  mov " + name + "(%rip), %rbx");
        gen->write("  mov %r10, (%rbx,%rax,8)");
      } 
      //else if (type.kind == Type::Kind::Float) gen->write("  movsd %" + reg + ", " + name + "(%rip)");
      return;
    }

    if (reg != "r10") gen->write("  mov %" + reg + ", %r10");
    size_t stack_loc = gen->get_var(name, expr_list_ele.line);

    gen->gen_expr(*expr_list_ele.index, false, "rax");

		gen->write("  mov " + std::to_string(stack_loc) + "(%rsp), %rbx");
		gen->write("  mov %r10, (%rbx,%rax,8)");
  } 
}

std::any get_raw_value(const NodeExpr& expr, Generator* gen) {
  switch (check_value(expr.var, gen, false).kind) {
    case Type::Kind::Str:
      return std::get<NodeExprStrLit>(expr.var).str_lit.value.value();
      break;
    case Type::Kind::Var:
      Type type = get_var_type(std::get<NodeExprIdent>(expr.var), gen);
      break; 
    //default: break;
  }
}

void check_func_args(const std::vector<NodeExprPtr>& args, const std::unordered_multimap<ArgType, ArgRequired>& required_args) {
    int max_args = 0;
    int required_args_count = 0;
    int not_count = false;

    for (const auto& required_arg : required_args) {
        if (required_arg.first == ArgType::NxtUndefNum) not_count = true;
        if (required_arg.second == ArgRequired::Yes) ++required_args_count;
        ++max_args;
    }
    if (args.size() != max_args && !not_count) {
        /*std::cerr << "Error, invalid args\n";
        exit(EXIT_FAILURE);*/
    }

    int current_arg = 0;
    for (const auto& required_arg : required_args) {
        if (required_arg.first == ArgType::Any) continue;
        if (required_arg.second == ArgRequired::No) continue;

        if (required_arg.first == ArgType::NxtUndefNum) return;

        if (is_ident(args[current_arg]->var)) continue;
	if (is_call(args[current_arg]->var)) continue;

        if (required_arg.first == ArgType::Float) {
            if (!is_float(args[current_arg]->var)) {
                add_error("argument " + std::to_string(current_arg + 1) + " should be a float", args[0]->line);
            }
        }
        else if (required_arg.first == ArgType::Integer) {
            if (!is_int(args[current_arg]->var)) {
                add_error("argument " + std::to_string(current_arg + 1) + " should be a integer", args[0]->line);
            }
        }
        else if (required_arg.first == ArgType::String) {
            if (!is_str(args[current_arg]->var)) {
                add_error("argument " + std::to_string(current_arg + 1) + " should be a string", args[0]->line);
            }
        }
        else if (std::holds_alternative<NodeExprCall>(args[current_arg]->var)) {
            // We will handle it later
        }

        ++current_arg;
    }
}

static void call_func(const std::string& fn, std::vector<NodeExprPtr> arg_values, Generator* gen, int line, bool direct_call = false) {
    std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};
    int index = 0;
    int reg_index = 0;
    int float_reg_index = 0;
	std::vector<Type> passed_args;
	size_t stack_start = gen->m_stack_size;

	int ii = 0;
	int fi = 0;
	size_t extra_stack = 0;
	for (const auto& arg : arg_values) {
		Type var_type = check_value(*arg, gen);
    var_type = replace_user_defined_type_with_generic(var_type, gen);

		if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::List) {
			++ii;
			if (ii > 6) ++extra_stack;
		}
		else if (var_type.kind == Type::Kind::Float) {
			++fi;
			if (fi > 8) ++extra_stack;
		}
		else if (var_type.kind == Type::Kind::List) {
			ii += 3;
			if (ii > 6) ++extra_stack;
		}
	}
	if (((gen->m_stack_size + extra_stack) % 2) != 0) {
		//gen->write("  sub $8, %rsp");
		++gen->m_stack_size;
	}		
	std::vector<std::pair<NodeExpr, Type>> stack_args;
    for (const auto& arg : arg_values) {
        Type var_type = check_value(*arg, gen);
        var_type = replace_user_defined_type_with_generic(var_type, gen);

		if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::Struct && reg_index > 5) {
			stack_args.push_back({*arg, var_type});
			continue;
		}
		else if (var_type.kind == Type::Kind::Float && float_reg_index > 7) {
			stack_args.push_back({*arg, Type{Type::Kind::Float}});
			continue;
		}
		else if (var_type.kind == Type::Kind::Struct) {
			std::string template_name;
			std::vector<NodeExprPtr> fields;
			if (std::holds_alternative<NodeExprStruct>(arg->var)) {
				NodeExprStruct expr_struct = std::get<NodeExprStruct>(arg->var);
				template_name = expr_struct.template_name.value.value();
				fields = expr_struct.fields;
			}
			else if (std::holds_alternative<NodeExprIdent>(arg->var)) {
				NodeExprIdent expr_ident = std::get<NodeExprIdent>(arg->var);
				for (const auto& var : gen->m_vars) {
					if (var.first == expr_ident.ident.value.value()) {
						template_name = var.second.struct_template;
						for (const auto& it : gen->m_vars_in_structs.at(expr_ident.ident.value.value())) {
							fields.push_back(std::make_shared<NodeExpr>(it));
						}
					}
				}
			}

			int struct_index = 0;
			for (const auto& struct_arg : gen->m_structs.at(template_name)) {
				if (struct_arg.second.kind != Type::Kind::Float && reg_index > 5) {
					stack_args.push_back({*fields[struct_index], struct_arg.second});	

					++struct_index;
					++reg_index;
					++index;
					continue;
				} else if (struct_arg.second.kind == Type::Kind::Float && float_reg_index > 7) {
					stack_args.push_back({*fields[struct_index], struct_arg.second});
					++struct_index;
					++float_reg_index;
					++index;
					continue;	
				}
				
				if (struct_arg.second.kind != Type::Kind::Float) {
					gen->gen_expr(*fields[struct_index], false, regs[reg_index]);
					++reg_index;
				}
				else if (struct_arg.second.kind == Type::Kind::Float) {
					gen->gen_expr(*fields[struct_index], false, regs[reg_index]); 
					++float_reg_index;
				}
				++index;
				++struct_index;
			}
			continue;
		}

        if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::List) {
          gen->gen_expr(*arg, false, regs[reg_index]);
        }
        else gen->gen_expr(*arg, false, float_regs[float_reg_index]);
        if (var_type.kind == Type::Kind::Float) ++float_reg_index;
        else ++reg_index;
        ++index;
    }

	if (!stack_args.empty()) {
		for (int i = stack_args.size() - 1; i >= 0; --i) {
			const auto& arg = stack_args[i];
			gen->gen_expr(arg.first, true, (arg.second.kind == Type::Kind::Float ? "xmm0" : "rax"));
		}
	}

	bool func_exists = false;
	for (const auto& fnc : declared_funcs) {
		if (direct_call) break;
		if (fnc.first == fn) {
			func_exists = true;
			int index = -1;
			for (const auto& arg : fnc.second.second) {
				++index;
				if (arg.kind == Type::Kind::Any || arg.kind == Type::Kind::Ptr) continue;
        if (arg.kind == Type::Kind::NxtUndefNum) break;
				if (arg.kind != passed_args[index].kind) {
					//add_error("ERROR PASSING ARGS\n", line);
					//return;
				}
			}
			if (!func_exists) {
				add_error("Inexistent func (" + fn + ")", line);
				return;
			}

		}
	}

    gen->write("  mov $" + std::to_string(float_reg_index) + ", %al");
    gen->call(fn);
	while (gen->m_stack_size > stack_start) {
        //gen->write("  add $8, %rsp");
		//--gen->m_stack_size;
    }
}

void Generator::gen_expr(const NodeExpr& expr, bool push_result, const std::string& reg, bool is_value, bool is_func_call) {
    struct ExprVisitor {
        Generator* gen;
        bool push_result;
        std::string reg;
        bool is_value;
		bool is_func_call;

        void operator()(const NodeExprIntLit& expr_int_lit) const {
            gen->write("  mov $" + expr_int_lit.int_lit.value.value() + ", %" + reg);
            if (push_result) gen->push(reg);
        }

        void operator()(const NodeExprBinary& expr_bin) const {
            const std::string& op = expr_bin.op_token.value.value();
            static int and_or_counter = 0;
            std::string and_or_label = std::to_string(and_or_counter);

            Type v1 = check_value(*expr_bin.lhs, gen);
            Type v2 = check_value(*expr_bin.rhs, gen);
            if (v1.kind == Type::Kind::None) {
                add_error("Cannot operate with null values in left expression", expr_bin.line);
            }
            bool reg1_ptr = false, reg2_ptr = false;
            if(std::holds_alternative<NodeExprDeref>(expr_bin.lhs->var)) reg1_ptr = true;
            if(std::holds_alternative<NodeExprDeref>(expr_bin.rhs->var)) reg2_ptr = true;

            if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                gen->gen_expr(*expr_bin.rhs, true, "rbx");
                gen->gen_expr(*expr_bin.lhs, false);
            }
            else if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Float) {
                gen->gen_expr(*expr_bin.lhs, false, "rbx");
                gen->write("cvtsi2sd %rbx, %xmm0");
                gen->gen_expr(*expr_bin.rhs, false, "xmm1");
            }
            else if (v1.kind == Type::Kind::Float && v2.kind == Type::Kind::Int) {
                gen->gen_expr(*expr_bin.rhs, false, "rbx");
                gen->write("cvtsi2sd %rbx, %xmm1");
                gen->gen_expr(*expr_bin.lhs, false, "xmm0");
            }
            else if (v1.kind == Type::Kind::Float && v2.kind == Type::Kind::Float) {
                gen->gen_expr(*expr_bin.rhs, false, "xmm1");
                gen->gen_expr(*expr_bin.lhs, false, "xmm0");
            }
            else if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::None) {
              gen->gen_expr(*expr_bin.lhs, true, "rbx");
            }

            if (op == "+") { 
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                  gen->pop("rbx");
                  gen->write( "  add %rbx, %rax");
                }
                /*else if (v1.kind == Type::Kind::Str) {
                    gen->write("  mov %rax, %rdi");
                    gen->write("  mov %rbx, %rsi");
                    gen->call("strcat");
                }*/
                else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
                    gen->write("  addsd %xmm1, %xmm0");
                }
            }
            else if (op == "-") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                  gen->pop("rbx");
                  gen->write("  sub %rbx, %rax");
                }
                else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) gen->write("  subsd %xmm1, %xmm0");
            }
            else if (op == "*") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                  gen->pop("rbx");
                  gen->write("  imul %rbx, %rax");
                }
                else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) gen->write("  mulsd %xmm1, %xmm0");
            }
            else if (op == "/") {
                gen->write("  xor %rdx, %rdx");
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                  gen->pop("rbx");
                  gen->write("  idiv %rbx");
                }
                else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) gen->write("  divsd %xmm1, %xmm0");
            }
            else if (op == "%") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
                    gen->pop("rbx");
                    gen->write("  xor %rdx, %rdx");
                    gen->write("  idiv %rbx");
                    gen->write("  mov %rdx, %rax");
                }
                else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
                    gen->write("  movsd %xmm0, %xmm2");        // copia a en xmm2
                    gen->write("  divsd %xmm1, %xmm0");         // xmm0 = a / b
                    gen->write("  roundsd $3, %xmm0, %xmm0");   // trunc(a / b)
                    gen->write("  mulsd %xmm1, %xmm0");         // xmm0 = b * trunc(a / b)
                    gen->write("  subsd %xmm0, %xmm2");         // xmm2 = a - b * trunc(a / b)
                    gen->write("  movapd %xmm2, %xmm0");        // resultado final en xmm0
                }
            }
            else if (op == "==") {
				if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  sete %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  sete %al");
					gen->write("  movzx %al, %rax");
				}
            }
            else if (op == "!=") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  setne %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  setne %al");
					gen->write("  movzx %al, %rax");
				} else if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::None) {
            gen->write("  cmp $0, %rbx");
            gen->write("  sete %al");
            gen->write("  movzx %al, %rax");
        }
            }
            else if (op == "<") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  setl %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  setb %al");
					gen->write("  movzx %al, %rax");
				}
            }
            else if (op == "<=") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  setle %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  setbe %al");
					gen->write("  movzx %al, %rax");
				}
            }
            else if (op == ">") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  setg %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  seta %al");
					gen->write("  movzx %al, %rax");
				}
            }
            else if (op == ">=") {
                if (v1.kind == Type::Kind::Int && v2.kind == Type::Kind::Int) {
          gen->pop("rbx");
					gen->write("  cmp %rbx, %rax");
					gen->write("  setae %al");
					gen->write("  movzx %al, %rax");
				}
				else if (v1.kind == Type::Kind::Float || v2.kind == Type::Kind::Float) {
					gen->write("  ucomisd %xmm1, %xmm0");
					gen->write("  setge %al");
					gen->write("  movzx %al, %rax");
				}
            }
            else if (op == "&&") {
                and_or_label = std::to_string(and_or_counter);

                gen->pop("rbx");
                gen->write("  cmp $0, %rax");
                gen->write("  je .false_" + and_or_label);
                gen->write("  cmp $0, %rbx");
                gen->write("  je .false_" + and_or_label);

                gen->write("  mov $1, %rax");
                gen->write("  jmp .end_" + and_or_label);
                gen->write("  .false_" + and_or_label + ":\n  mov $0, %rax");
                gen->write("  .end_" + and_or_label + ":");
                
                ++and_or_counter;
            }
            else if (op == "||") {
              and_or_label = std::to_string(and_or_counter);

              gen->pop("rbx");
              gen->write("  cmp $0, %rax");
              gen->write("  jne .true_" + and_or_label);
              gen->write("  cmp $0, %rbx");
              gen->write("  jne .true_" + and_or_label);

              gen->write("  mov $0, %rax");
              gen->write("  jmp .end_" + and_or_label);
              gen->write("  .true_" + and_or_label + ":\n  mov $1, %rax");
              gen->write("  .end_" + and_or_label + ":");

              ++and_or_counter;
            }
            else {
                add_error("Unsupported binary operator", expr_bin.line);
            }
            if (push_result) {
                if (v1.kind != Type::Kind::Float && v2.kind != Type::Kind::Float) gen->push("rax");
                else gen->push_float(reg);
            } else {
              if (reg != "rax") gen->write("  mov %rax, %" + reg);
            }
        }
        
        void operator()(const NodeExprUnary& expr_unary) const {
          gen->gen_expr(*expr_unary.expr, false);
          
          if (expr_unary.op.type == TokenType::bang) {
            gen->write("  cmp $1, %rax");
            gen->write("  setne %al");
            gen->write("  movzx %al, %rax");
            if (push_result) gen->push("rax");
            else if (reg != "rax") gen->write("  mov %rax, %" + reg);
          }
        }

        void operator()(const NodeExprBinaryAssign& expr_bin_assign) const {
            const std::string& op = expr_bin_assign.op_token.value.value();
            const std::string& var_name = expr_bin_assign.left_token.value.value();

            gen->gen_expr(*expr_bin_assign.right_expr, false);

            gen->write("  mov %rax, %rbx");

            size_t offset_bytes = gen->get_var(var_name, expr_bin_assign.line);
            gen->write("  mov " + std::to_string(offset_bytes) + "(%rsp), %rax");

            Type var_type = gen->m_vars.at(var_name).type;
            if (var_type.kind == Type::Kind::None ||
            std::holds_alternative<NodeExprNone>(expr_bin_assign.right_expr->var)) {
                add_error("Cannot operate with null values", expr_bin_assign.line);
            }
            if (op == "+=") {
                if (var_type.kind == Type::Kind::Int) gen->write("  add %rbx, %rax");
                /*else {
                    gen->write("  mov %rax, %rdi");
                    gen->write("  mov %rbx, %rsi");
                    gen->call("strcat");
                }*/
            }
            else if (op == "-=") {
                gen->write("  sub %rbx, %rax");
            }
            else if (op == "*=") {
                gen->write("  imul %rbx, %rax");
            }
            else if (op == "/=") {
                gen->write("  xor %rdx, %rdx");
                gen->write("  idiv %rbx");
            }
            else {
                add_error("Invalid assignment operator", expr_bin_assign.line);
            }

            gen->write("  mov %" + reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
            //if (push_result) gen->push("rax");
        }
        void operator()(const NodeExprUnaryIncDec& expr_unary_operator) const {
            const std::string& op = expr_unary_operator.op_token.value.value();
            const std::string& var_name = expr_unary_operator.ident.value.value();

            size_t offset_bytes = gen->get_var(var_name, expr_unary_operator.line);
            gen->write("  mov " + std::to_string(offset_bytes) + "(%rsp), %rax");

            if (op == "++") {
                gen->write("  add $1, %rax # Increment in 1 variable (" + var_name + ")");
            }
            else if (op == "--") {
                gen->write("  sub $1, %rax");
            }
            else {
                add_error("Invalid unary operator", expr_unary_operator.line);
            }
            gen->write("  mov %" + reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
            //if (push_result) gen->push("rax");
        };

        void operator()(const NodeExprIdent& expr_ident) const {
			if (gen->m_glob_vars.contains(expr_ident.ident.value.value())) {
			  const std::string& name = expr_ident.ident.value.value();
        GlobVar var = gen->m_glob_vars.at(name);
        if (var.type.kind != Type::Kind::Float) gen->write("  mov " + name + "(%rip), %" + reg);
        else gen->write("  movsd " + name + "(%rip), %xmm0" );
        
        if (push_result) {
          if (var.type.kind != Type::Kind::Float) gen->push(reg);
          else gen->push("xmm0");
        }
        return;
			}	

            if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == expr_ident.ident.value.value()) {
                                if (var.type.kind != Type::Kind::Float) gen->write("  mov -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                else gen->write("  movsd -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                //gen->write("  mov rax, rsi");
                                if (push_result) {
                                    if (var.type.kind != Type::Kind::Float) gen->push(reg);
                                    else gen->push_float(reg);
                                }
                                return;
                            }
                        }
                    }
                }
            }
            const std::string& var_name = expr_ident.ident.value.value();
            size_t offset_bytes = gen->get_var(var_name, expr_ident.line);
            //std::cerr << gen->m_vars.at(var_name).stack_loc << "\n";
            if (gen->m_vars.at(var_name).type.kind != Type::Kind::Float) gen->write("  mov " +  std::to_string(offset_bytes) +" (%rsp), %" + reg);
            else if (gen->m_vars.at(var_name).type.kind == Type::Kind::Float) gen->write("  movsd " + std::to_string(offset_bytes) + "(%rsp), %" + reg);
            //gen->write("  mov rax, rsi");
            if (push_result) {
                if (gen->m_vars.at(var_name).type.kind != Type::Kind::Float) gen->push(reg);
                else gen->push_float(reg);
            }
        }

        void operator()(const NodeExprStrLit& expr_str_lit) const {
          std::string value = expr_str_lit.str_lit.value.value();

          /*bool in_expr = false;
          std::string current_expr = "";
          std::string final_str = "";

          for (char c : value) {
            if (in_expr) current_expr += c; continue;
            if (c == '{') {
              in_expr = true;
              continue;
            } else if (c == '}') {
              in_expr = false; // TODO
              Tokenizer tokenizer(std::move(current_expr));
              std::vector<Token> tokens = tokenizer.tokenize();
              current_expr.clear();

              Parser parser(std::move(tokens));
              std::optional<NodeProg> program = parser.parse_prog();

              if (!program.has_value()) {
                  std::cerr << "Parsing failed on expression in string\n";
              }

              gen->gen_expr(&program.value().expr); 

              continue;
            }
            final_str += c;
          */

            for (int i = 0; i < gen->m_string_literals.size(); ++i) {
                if (gen->m_string_literals[i] == expr_str_lit.str_lit.value.value()) {
                    gen->write("  lea str_" + std::to_string(i) + "(%rip), %" + reg);
                    if (push_result) gen->push(reg);
                    return;
                }
            }

            std::string label = "str_" + std::to_string(gen->m_string_literals.size());
            gen->m_string_literals.push_back(expr_str_lit.str_lit.value.value());
            gen->write("  lea " + label + "(%rip), %" + reg);
            if (push_result) gen->push(reg);
        }

        void operator()(const NodeExprFloatLit& expr_float_lit) const {
            std::string label = "float_" + std::to_string(gen->m_float_literals.size());
            double val = std::stod(expr_float_lit.float_lit.value.value());  // convierte a double
            gen->m_float_literals.push_back(val);
            gen->write("  movsd " + label + "(%rip), %" + reg);

            if (push_result) gen->push_float(reg);
        }

        void operator()(const NodeExprNone& expr_none) const {
            gen->write("  mov $0, %" + reg);
            if (push_result) gen->push(reg);
        }
        void operator()(const NodeExprNoArg& expr_no_arg) const {}
        void operator()(const NodeExprCR& expr_cr) const {}

        void operator()(const NodeExprBoolValue& expr_bool) const {
            gen->write("  mov $" + std::to_string(expr_bool.value) + ", %" + reg);
            if (push_result) gen->push(reg);
        }

        void operator()(const NodeExprCall& expr_call) const {
			    const std::string& fn = expr_call.name.value.value();
          std::vector<NodeExprPtr> arg_values = expr_call.args;
          size_t old_stack_size = gen->m_stack_size;
			    call_func(fn, arg_values, gen, expr_call.line, false);
          gen->m_stack_size = old_stack_size;
          if (reg != "rax") gen->write("  mov %rax, %" + reg);
          if (push_result) gen->push(reg);
		}

        void operator()(const NodeExprProperty& stmt_property) const {
			const std::string& ident = stmt_property.ident.value.value();
			const std::string& property = stmt_property.property.value.value();
			int line = stmt_property.line;

			size_t stack_abs_pos = 0;
			size_t stack_arg_pos;

			std::string struct_templat;
			bool is_arg = false;
			if (gen->current_mode == Mode::Function) {
				for (const auto& fnc : gen->m_fnc_args) {
					if (fnc.first == gen->current_func) {
						for (const auto& arg : gen->m_fnc_args.at(fnc.first)) {
							if (arg.name == ident) {
								is_arg = true;
								struct_templat = arg.struct_template;
								stack_arg_pos = arg.stack_loc;
							}
						}
					}
				}
			}	
			if (!is_arg) {
				if (gen->m_vars.contains(ident)) {
					struct_templat = gen->m_vars.at(ident).struct_template;
				} else {
					add_error("Cannot find variable", line);
					return;
				}
			}
			if (struct_templat == "") {
				add_error("Variable is not a struct type", line);
				return;
			}

			if (!is_arg) {
				for (const auto& struct_template : gen->m_structs) {
					if (struct_template.first != struct_templat) continue;
					size_t stack_rel_pos = 0;
          for (const auto& field : struct_template.second) {
             if (field.first == stmt_property.property.value.value()) {
               break;
             }
             stack_rel_pos += 8;
          }
					stack_abs_pos = gen->get_var(ident, stmt_property.line);
					gen->write("  mov " + std::to_string(stack_abs_pos) + "(%rsp), %" + reg);
          gen->write("  mov " + std::to_string(stack_rel_pos) + "(%" + reg + "), %" + reg);
					if (push_result) gen->push(reg);
				}
			} else {
				int index = 0;
				for (const auto& field : gen->m_structs.at(struct_templat)) {
					if (field.first == property) break;
					++index;
				}	
				gen->write("  mov -" + std::to_string(stack_arg_pos + (index * 8)) + "(%rbp), %" + reg);
				if (push_result) gen->push(reg);

			}
		}	
		
		void operator()(const NodeExprGetPtr& expr_ptr) const {
			if (gen->m_vars.contains(expr_ptr.ident.value.value())) {	
				size_t offset_bytes = gen->get_var(expr_ptr.ident.value.value(), expr_ptr.line);
				gen->write("  lea " + std::to_string(offset_bytes) + "(%rsp), %" + reg);
				if (push_result) gen->push(reg);
			}
			else if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == expr_ptr.ident.value.value()) {
                                if (var.type.kind != Type::Kind::Float) gen->write("  lea -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                else gen->write("  movsd -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                //gen->write("  mov rax, rsi");
                                if (push_result) {
                                    if (var.type.kind != Type::Kind::Float) gen->push(reg);
                                    else gen->push_float(reg);
                                }
                                return;
                            }
                        }
                    }
                }
            }
			else {
				add_error("Unexistent variable\n", expr_ptr.line);
			}	
		}

    void operator()(const NodeExprDeref& expr_ref) const {
      const std::string& expr_reg = check_value(*expr_ref.expr, gen).kind != Type::Kind::Float ? reg : "xmm0"; 
      gen->gen_expr(*expr_ref.expr, false, expr_reg.c_str());

      if (is_value) gen->write("  mov (%" + expr_reg + "), %" + reg);
      if (push_result) gen->push(reg);
    }

		void operator()(const NodeExprList& expr_list) const {
			size_t shadow_space = expr_list.elements.size() * 8;
			gen->write("  mov $" + std::to_string(shadow_space) + ", %rdi");
			gen->call("malloc");
			gen->write("  mov %rax, %rbx");

			for (int i = 0; i <= expr_list.elements.size() - 1; ++i) {
				const auto& arg = expr_list.elements[i];
				Type var_type = check_value(arg, gen);
				if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::List) {
					gen->gen_expr(arg, false, "rax");
					gen->write("  mov %rax, " + std::to_string(i * 8) + "(%rbx)");
				}
				else if (var_type.kind == Type::Kind::Float) {
					gen->gen_expr(arg, false, "xmm0");
					gen->write("  movsd %xmm0, " + std::to_string(i * 8) + "(%rbx)");
				}
			}
			if (reg != "rbx") gen->write("  mov %rbx, %" + reg);
      if (push_result) gen->push(reg);
		}

		void operator()(const NodeExprListElement& expr_list_element) const {
			const std::string& var_name = expr_list_element.list_name.value.value();
			if (check_value(expr_list_element, gen).kind != Type::Kind::Int) {
				add_error("Expected Integer expression as index", expr_list_element.line);
				return;
			}
      bool is_arg = false;

      size_t stack_loc;
      if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == var_name) {
                                stack_loc = var.stack_loc;
                                is_arg = true;
                                //gen->write("  mov rax, rsi");
                            }
                        }
                    }
                }
            }
      if (gen->m_vars.contains(var_name) && !is_arg) stack_loc = gen->get_var(var_name, expr_list_element.line);			
			gen->gen_expr(*expr_list_element.index, false, "rax");

      if (gen->m_glob_vars.contains(var_name)) {
        gen->write("  mov " + var_name + "(%rip), %rbx");
        gen->write("  mov (%rbx,%rax,8), %" + reg);
        if (push_result) gen->push(reg);
        return;
      }

			if (!is_arg) gen->write("  mov " + std::to_string(stack_loc) + "(%rsp), %rbx");
      else gen->write("  mov -" + std::to_string(stack_loc) + "(%rbp), %rbx");
			gen->write("  mov (%rbx,%rax,8), %" + reg);

			if (push_result) gen->push(reg);
		}

		void operator()(const NodeExprStruct& expr_struct) const {
      gen->write("  mov $" + std::to_string(expr_struct.fields.size() * 8) + ", %rdi");
      gen->write("  call malloc@PLT");
      gen->write("  mov %rax, %rbx");

      int index = 0;
      for (const auto& field : expr_struct.fields) {
        gen->gen_expr(field->var, false);
        gen->write("  mov %rax, " + std::to_string(index * 8) + "(%rbx)");
        ++index;
      }
      if (push_result) gen->push("rbx");
      if (reg != "rbx") gen->write("  mov %rbx, %" + reg);

			/*int normal_reg_index = 0;
			int float_reg_index = 0;

			const std::string& template_name = expr_struct.template_name.value.value();
			gen->last_struct_template_name = template_name;
			int line = expr_struct.line;

			if (!gen->m_structs.contains(template_name)) {
				add_error("Inexistent struct template", line);
				return;
			}
			
			int index = 0;
			for (const auto& field : expr_struct.fields) {
				bool field_found = false;
				Type field_type = gen->m_structs.at(template_name)[index].second;
				NodeExpr expr = *field;
				if (check_value(expr, gen).kind != field_type.kind) {
					add_error("Argument type in struct is not the same as the struct template", line);
					return;
				}
				++index;	
			}

			gen->m_struct_temp_args.clear();
			for (const auto& it : expr_struct.fields) {
				if (!is_func_call) gen->gen_expr(*it, true);
				gen->m_struct_temp_args.push_back(*it);
			}*/
		}

    void operator()(const NodeExprNew& stmt_new) const {
      if (stmt_new.type.kind != Type::Kind::Struct) {
        gen->write("  mov $8, %rdi");
        gen->write("  call malloc");

        if (push_result) gen->push("rax");
        else if (reg != "rax") gen->write("  mov %rax, %" + reg);
      }
    }

    };

    ExprVisitor visitor{.gen = this, .push_result = push_result, .reg = reg, .is_value = is_value, .is_func_call = is_func_call};
    std::visit(visitor, expr.var);
}

void Generator::gen_stmt(const NodeStmt& stmt) {
    struct StmtVisitor {
        Generator* gen;

        void operator()(const NodeStmtImport& stmt_import) const {
			const std::string& name = stmt_import.to_import.value.value();
			const std::string& path = "/usr/include/ysinclude/" + name + ".ys";
            std::ifstream input_file(path);
            if (!input_file.is_open()) {
                std::cerr << "Error: Could not open " << name << "\n";
            }

            std::stringstream full_source;
            std::string line;
            current_source_file = path;

            while (std::getline(input_file, line)) {
                full_source << line << '\n';
                ++current_line;
            }
            std::string source_code = full_source.str();

            Tokenizer tokenizer(std::move(source_code));
            std::vector<Token> tokens = tokenizer.tokenize();

            Parser parser(std::move(tokens));
            std::optional<NodeProg> program = parser.parse_prog();

            if (!program.has_value()) {
                std::cerr << "Parsing failed in file: " << name << "\n";
            }

            std::string base_name = fs::path(path).stem().string();
            //std::string gen_name = (index == 0) ? "main" : base_name;
            std::string gen_name = base_name;

            Generator generator(program.value(), gen_name);
            std::string asm_code = generator.gen_prog();
            for (const auto& glob_var : generator.m_glob_vars) {
              auto modified_var = glob_var;
              modified_var.second.is_declared = false;
              gen->m_glob_vars.insert(modified_var);
            }
            for (const auto& declared_func : generator.declared_funcs) {
              gen->declared_funcs.insert(declared_func);
            }
        }

        void operator()(const NodeStmtUse& stmt_use) const {
            for (int i = 0; i < stmt_use.use.size(); ++i) {
                gen->write("  .globl " + stmt_use.use[i].value.value()); // write don't work here
                gen->m_output << ".extern " << stmt_use.use[i].value.value() << "\n";
            }
        }

        void operator()(const NodeStmtMkpub mkpub) const {
            if (gen->current_mode == Mode::Function) {
                add_error("Making public a function inside of another function is not allowed", mkpub.line);
            }
            for (const auto& func : mkpub.functions) {
                bool func_exists = false;
                for (const auto& mapped_func : gen->m_fnc_args) {
                    if (mapped_func.first == func.value.value()) {
                        func_exists = true;
                    }
                }
                if (!func_exists) {
                    /*for (const auto& e : gen->m_fnc_args) {
                        std::cerr << e.first << "\n";
                    }*/
                    add_error("Making public an inexistent function (" + func.value.value() + ")", mkpub.line);
                }
                gen->write("  .globl " + func.value.value());
                gen->write("  .type " + func.value.value() + ", @function");
                if (func.value.value() == "main") continue;
                gen->m_output << ".globl " << func.value.value() << "\n";
                gen->m_output << ".type " << func.value.value() << ", " << "@function" << "\n";
            }
        }

        void operator()(const NodeStmtVar& stmt_var) const {
            if (gen->current_mode == Mode::Global) {
                //add_error("Variables only can be declared inside of a function", stmt_var.line);
            }	

            const std::string& name = stmt_var.ident.value.value(); 
            Type value_type = check_value(stmt_var.expr, gen);
            if (stmt_var.type.kind != Type::Kind::None) value_type = stmt_var.type;
            std::string custom_type_name = value_type.user_type;
            value_type = replace_user_defined_type_with_generic(value_type, gen);
            if (value_type.kind != Type::Kind::Float && value_type.kind != Type::Kind::List /*&& value_type.kind != Type::Kind::Struct*/) gen->gen_expr(stmt_var.expr, false);
            else if (value_type.kind == Type::Kind::Float) gen->gen_expr(stmt_var.expr, false, "xmm0");
			/*else if (value_type.kind == Type::Kind::Struct) {
				gen->gen_expr(stmt_var.expr, false);
				gen->m_vars_in_structs.insert({name, gen->m_struct_temp_args});
			}*/
			else {
				gen->gen_expr(stmt_var.expr, true, "rax"); // Special case
				gen->push("rax");
				gen->insert_var(stmt_var.ident.value.value(), Type{Type::Kind::List}, stmt_var.is_mutable);
				gen->m_lists.insert({name, {}});
			}
            
            if (value_type.kind != Type::Kind::Float && value_type.kind != Type::Kind::List /*&& value_type.kind != Type::Kind::Struct*/) gen->push("rax", false);
            else if (value_type.kind == Type::Kind::Float) gen->push_float("xmm0", false);

            if (!stmt_var.has_initial_value) {
				      gen->write("  mov $0, %rax");
				      gen->push("rax");
				      gen->insert_var(stmt_var.ident.value.value(), Type{Type::Kind::Int}, stmt_var.is_mutable);
				      //return;
			      }

            //gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var_type, .name = name}});
            if (gen->current_mode != Mode::Global) {
              gen->insert_var(name, value_type, stmt_var.is_mutable, (value_type.kind == Type::Kind::Struct ? custom_type_name : ""), false);
            } else {
              gen->insert_var(name, value_type, stmt_var.is_mutable, (value_type.kind == Type::Kind::Struct ? custom_type_name : ""), true, stmt_var.expr);
            }
        }
         
        void operator()(const NodeStmtAssign& stmt) const { 
          // 2. Resolver tipo del RHS
          Type rhs_type = check_value(stmt.value, gen);
          std::string rhs_reg = rhs_type.kind != Type::Kind::Float ? "rbx" : "xmm1";
          gen->gen_expr(stmt.value, true, rhs_reg, true);

          // 1. Resolver el target (LHS)
          Type lhs_type = check_value(stmt.target, gen);
          std::string lhs_reg = lhs_type.kind != Type::Kind::Float ? "rax" : "xmm0";
          gen->gen_expr(stmt.target, false, lhs_reg, false);

          gen->pop("rbx");

          bool is_deref = false;
          std::string e = "";
          if (std::holds_alternative<NodeExprDeref>(stmt.target.var)) is_deref = true;

          // 3. Según el operador
          switch (stmt.op_tok.type) {
            case TokenType::eq:
              if (lhs_type.kind != Type::Kind::Float)
                gen->write("  mov %" + rhs_reg + ", " + (is_deref ? "(%" : "%") + lhs_reg + (is_deref ? ")" : ""));
              else
                gen->write("  movsd %" + rhs_reg + ", %" + lhs_reg);
            break;

            case TokenType::plus_eq:
              if (lhs_type.kind != Type::Kind::Float)
                gen->write("  add %" + rhs_reg + ", %" + lhs_reg);
              else
                gen->write("  addsd %" + rhs_reg + ", %" + lhs_reg);
            break;

            case TokenType::minus_eq:
              if (lhs_type.kind != Type::Kind::Float)
                gen->write("  sub %" + rhs_reg + ", %" + lhs_reg);
              else
                gen->write("  subsd %" + rhs_reg + ", %" + lhs_reg);
            break;

            case TokenType::star_eq:
              if (lhs_type.kind != Type::Kind::Float)
                gen->write("  imul %" + rhs_reg + ", %" + lhs_reg);
              else
                gen->write("  mulsd %" + rhs_reg + ", %" + lhs_reg);
            break;

          case TokenType::slash_eq:
            if (lhs_type.kind != Type::Kind::Float) {
              gen->write("  cqo"); // extender rax a rdx:rax
              gen->write("  idiv %" + rhs_reg);
            } else {
              gen->write("  divsd %" + rhs_reg + ", %" + lhs_reg);
            }
          break;
        }
        set_value(gen, lhs_reg, stmt.target, stmt.value);

    // 4. Guardar el valor de vuelta en la variable
       /* if (!stmt.is_declaration) {
          auto name = get_ident_name(stmt.target);
          size_t offset_bytes = gen->get_var(name);
          if (lhs_type.kind != Type::Kind::Float)
            gen->write("  mov %" + lhs_reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
          else
            gen->write("  movsd %" + lhs_reg + ", " + std::to_string(offset_bytes) + "(%rsp)");
        }*/
    }

        void operator()(const NodeStmtVarRe& stmt_var) const {
            if (gen->current_mode == Mode::Global) {
                //add_error("Variables only can be redefined inside of a function", stmt_var.line);
            }

            const std::string& name = stmt_var.ident.value.value(); 

            Type value_type = check_value(stmt_var.expr, gen);
            if (value_type.kind != Type::Kind::Float) gen->gen_expr(stmt_var.expr, false);
            else gen->gen_expr(stmt_var.expr, false, "xmm0");

			if (gen->m_glob_vars.contains(stmt_var.ident.value.value())) {
			  GlobVar var = gen->m_glob_vars.at(name);
        if (!var.is_mutable) {
          add_error("Cannot modify a constant (" + name + ")", stmt_var.line);
        }
				gen->m_glob_vars.at(name).type = value_type;
        if (value_type.kind != Type::Kind::Float) gen->write("  mov %rax, " + name + "(%rip) # Editing var value (" + name + ")");
				else gen->write("  movsd %xmm0, " + name  + "(%rip) # Editing var value (" + name + ")");
        return;
      }	

            if (gen->m_vars.contains(name)) {
                Var var = gen->m_vars.at(name);
                if (!var.is_mutable) {
                    add_error("Cannot modify a constant (" + name + ")", stmt_var.line);
                }
                /*if (gen->m_vars.at(name).type != value_type) {
                    //std::cerr << static_cast<int>(value_type) << "\n";
                    add_error("Expected same type in reassigment", stmt_var.line);
                }*/

                size_t offset_bytes = gen->get_var(name, stmt_var.line);
				gen->m_vars.at(name).type = value_type;
                if (value_type.kind != Type::Kind::Float) gen->write("  mov %rax, " + std::to_string(offset_bytes)  + "(%rsp) # Editing var value (" + name + ")");
				else gen->write("  movsd %xmm0, " + std::to_string(offset_bytes)  + "(%rsp) # Editing var value (" + name + ")");
                return;
            }

        }

        void operator()(const NodeStmtIf& stmt_if) const {
            static int label_counter = 0;
            int current = label_counter++;
            int elif_counter = 0;

            std::string else_label = "else_" + std::to_string(current);
            std::string end_label = "endif_" + std::to_string(current);
            std::string elif_base_label = "elif_" + std::to_string(current) + "_";

            gen->gen_expr(stmt_if.condition, false);
            gen->write( "  cmp $0, %rax");
             if (stmt_if.elif_conditions.empty()) gen->write("  je " + else_label);
             else gen->write("  je " + elif_base_label + "0");

            size_t var_n = gen->m_vars.size();
            for (const auto& stmt : stmt_if.then_branch) {
                gen->gen_stmt(stmt);
            }
            while (gen->m_vars.size() > var_n) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }

            gen->write("  jmp " + end_label);

            int index = 0;
            for (const auto& elif : stmt_if.elif_conditions) {
              gen->write(elif_base_label + std::to_string(elif_counter) + ":");
              gen->gen_expr(elif, false);
              gen->write("  cmp $0, %rax");
              if (stmt_if.elif_conditions.size() > elif_counter + 1) gen->write("  je " + elif_base_label + std::to_string(elif_counter + 1));
              else gen->write("  je " + else_label);

              size_t var_n = gen->m_vars.size();
              for (const auto& stmt : stmt_if.elif_branches[index]) {
                gen->gen_stmt(stmt);
              }
              while (gen->m_vars.size() > var_n) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
              }

              gen->write("  jmp " + end_label);

              ++index;
              ++elif_counter;
            }

            gen->write("  jmp " + end_label);

            gen->write(else_label + ":");
            var_n = gen->m_vars.size();
            for (const auto& stmt : stmt_if.else_branch) {
              gen->gen_stmt(stmt);
            }

            gen->write(end_label + ":");
            while (gen->m_vars.size() > var_n) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }
        }

        void operator()(const NodeStmtWhile& stmt_while) const {
            static int label_counter = 0;
            int current = ++label_counter;

            std::string label = "while_" + std::to_string(current) + "_";
            std::string start_label = label + "start";
            std::string end_label = label + "end";
            std::string cleanup_label = label + "cleanup";
            gen->stmt_orde.push(label);
            size_t var_n = gen->m_vars.size();
			size_t stack_start = gen->m_stack_size;

            for (const auto& stmt : stmt_while.bfw) {
                //gen->gen_stmt(stmt);
            } 

            gen->write(start_label + ":");

            gen->gen_expr(stmt_while.condition, false);
            gen->write("  cmp $0, %rax");
            gen->write("  je " + end_label);

            for (const auto& stmt :  stmt_while.then_branch) {
                gen->gen_stmt(stmt);
            }

            while (gen->m_vars.size() > var_n) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }
 
            gen->write("  jmp " + start_label);
            gen->write(end_label + ":"); 
            /*while (gen->m_vars.size() > stack_start) {
                gen->pop("rax");
            }*/
			/*if (gen->m_stack_size > stack_start) {
				gen->write("  add $" + std::to_string((gen->m_stack_size - stack_start) * 8) + ", %rsp");
				gen->m_stack_size -= (gen->m_stack_size - stack_start);
			}*/
      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto& var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }
      gen->stmt_orde.pop();
		}
    void operator()(const NodeStmtLoop& stmt_loop) const {
      static int label_counter = 0;
      int current = ++label_counter;

      std::string label = "loop_" + std::to_string(current) + "_";
      std::string start_label = label + "start";
      std::string end_label = label + "end";
      std::string cleanup_label = label + "cleanup";
      gen->stmt_orde.push(label);
      size_t var_n = gen->m_vars.size();
			size_t stack_start = gen->m_stack_size;
      gen->write(start_label + ":"); 

      for (const auto& stmt :  stmt_loop.then_branch) {	
          gen->gen_stmt(stmt);
      }
 
      gen->write("  jmp " + start_label);
      gen->write(end_label + ":"); 

      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto& var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->stmt_orde.pop();
		}
    
    void operator()(const NodeStmtFor& stmt_for) const {
      size_t stack_start_before_loop = gen->m_stack_size;
      size_t var_n_before_loop = gen->m_vars.size();
      for (const auto& stmt : stmt_for.init) {
        gen->gen_stmt(stmt);
      }
      static int label_counter = 0;
      int current = ++label_counter;

      std::string label = "for_" + std::to_string(current) + "_";
      std::string start_label = label + "start";
      std::string end_label = label + "end";
      std::string cleanup_label = label + "cleanup";
      gen->stmt_orde.push(label);
      size_t var_n = gen->m_vars.size();
			size_t stack_start = gen->m_stack_size;
      gen->write(start_label + ":");

      gen->gen_expr(stmt_for.condition, false);
      gen->write("  cmp $0, %rax");
      gen->write("  je " + end_label);

      for (const auto& stmt : stmt_for.code_branch) {
        gen->gen_stmt(stmt);
      }
      for (const auto& stmt : stmt_for.update) {
        gen->gen_stmt(stmt);
      }

      gen->write("  jmp " + start_label);
      gen->write(end_label + ":"); 
      while (gen->m_vars.size() > var_n_before_loop) {
        int i = gen->m_vars_order.size() - 1;
        const auto& var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->stmt_orde.pop();  
    }

        void operator()(const NodeStmtStop& stmt_stop) const {
            if (gen->stmt_orde.empty()) {
                add_error("Can not stop an inexistent loop", stmt_stop.line);
            }
            std::string end_label = gen->stmt_orde.top() + "end";
            gen->write("  jmp " + end_label);
            //gen->stmt_orde.pop();
        }

        void operator()(const NodeStmtContinue& stmt_continue) const {
            if (gen->stmt_orde.empty()) {
                add_error("Can not use 'continue' in an inexistent loop", stmt_continue.line);
            }
            std::string cleanup_label = gen->stmt_orde.top() + "start";
            gen->write("  jmp " + cleanup_label); // cleanup will jump to start label after clean stack generated in statment
        }
 
        const std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
        const std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};

        void operator()(const NodeStmtDefFunc& stmt_def_func) const {	
            int index = 1;
            int reg_index = 0;
            int float_reg_index = 0;
			int stack_index = 0;
      size_t var_n = gen->m_vars.size();

            std::vector<Var> args;
			std::vector<Type> args_required;
			Type ret_type = stmt_def_func.return_type;
      ret_type = replace_user_defined_type_with_generic(ret_type, gen);

			for (const auto& arg : stmt_def_func.args) {
				args_required.push_back(replace_user_defined_type_with_generic(arg.arg_type, gen));
			}

			if (stmt_def_func.name.value.value() == "main" && main_func_declared != -5) {
        main_func_declared = true;
      }
			else {
				const std::string& name = stmt_def_func.name.value.value();
				gen->declared_funcs.insert({name, {ret_type, args_required}});
			}
			
			if (stmt_def_func.is_pub) {
				gen->m_output << ".globl " + stmt_def_func.name.value.value() << "\n";
			}
			if (stmt_def_func.is_extern) return;
			
			if (gen->current_mode == Mode::Function) {
				add_error("Function declaration inside antoher function is not allowed", stmt_def_func.line);
            }
            size_t stack_start = gen->m_stack_size;
            gen->m_stack_size = 0;
            gen->current_mode = Mode::Function;
            gen->current_func = stmt_def_func.name.value.value();
            gen->write(stmt_def_func.name.value.value() + ":");
            gen->write("  push %rbp");
            ++gen->m_stack_size;
			//++gen->m_stack_size;
            gen->write("  mov %rsp, %rbp"); 
      std::string old_buf = gen->buf.str();
      gen->buf.str("");
      gen->write_stmt = false;
			int line = stmt_def_func.line;

			int ii = 0;
			int fi = 0;
			size_t extra_stack = 2;
			size_t extra_stack_index = 2;
			for (const auto& arg : stmt_def_func.args) {
				Type var_type = arg.arg_type;
        var_type = replace_user_defined_type_with_generic(var_type, gen);

				if (var_type.kind != Type::Kind::Float) {
					++ii;
					if (ii > 6) ++extra_stack;
				}
				else if (var_type.kind == Type::Kind::Float) {
					++fi;
					if (fi > 8) ++extra_stack;
				}
			}
			
			size_t shadow_space = (((gen->m_stack_size + stmt_def_func.args.size()) % 2) == 0 ? (stmt_def_func.args.size() * 8) : (stmt_def_func.args.size() * 8 + 8));
      gen->m_stack_size += shadow_space / 8;
			//gen->write("  sub $" + std::to_string(shadow_space) + ", %rsp");
            for (int i = 0; i < stmt_def_func.args.size(); ++i) {
                auto arg = stmt_def_func.args[i];
                Type var_type = replace_user_defined_type_with_generic(arg.arg_type, gen);
                args.push_back({.stack_loc = static_cast<size_t>(index) * 8, .type = var_type, .name = arg.name, .struct_template = (var_type.kind == Type::Kind::Struct) ? stmt_def_func.absolute_type_name_args[i] : ""});
				
				if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::Struct && reg_index > 5) {
					gen->write("  mov " + std::to_string((extra_stack_index + 1) * 8) + "(%rbp), %rax");
					gen->write("  mov %rax, -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");
					++index;
					++stack_index;
					++extra_stack_index;
					continue;
				}
				else if (var_type.kind == Type::Kind::Float && float_reg_index > 7) {
					gen->write("  movsd " + std::to_string((extra_stack + 1) * 8) + "(%rbp), %xmm0");
					gen->write("  movsd %xmm0, -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");
					++index;
					++stack_index;
					--extra_stack;
					continue;
				} else if (var_type.kind == Type::Kind::Struct) {
					std::string template_name = stmt_def_func.absolute_type_name_args[i];
					if (!gen->m_structs.contains(template_name)) {
						add_error("Unknown template name", line);
						return;
				} else if (var_type.kind == Type::Kind::List) {
          //
        }
					for (const auto& arg : gen->m_structs.at(template_name)) {
						Type type = arg.second;
						if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct && reg_index > 5) {
							gen->write("  mov " + std::to_string((extra_stack_index) * 8) + "(%rbp), %rax");
							gen->write("  mov %rax, -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");
							++stack_index;
							++extra_stack_index;
							++index;
							continue;
						}
						else if (type.kind == Type::Kind::Float && float_reg_index > 7) {
							gen->write("  movsd " + std::to_string((extra_stack) * 8) + "(%rbp), %xmm0");
							gen->write("  movsd %xmm0, -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");
							++stack_index;
							--extra_stack;
							++index;
							continue;
						} else if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct) {
							gen->write("  mov %" + regs[reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)"); 
							++reg_index;
							++index;
						} else if (type.kind == Type::Kind::Float) {
							gen->write("  movsd %" + float_regs[float_reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");  
							++float_reg_index;
							++index;
						}
					}
					continue;
				}

                if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::Struct) {
                    gen->write("  mov %" + regs[reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)"); 
                    ++reg_index;
                    ++index;
                }
                else if (var_type.kind == Type::Kind::Float) {
                    gen->write("  movsd %" + float_regs[float_reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 8) + "(%rbp)");
                    ++float_reg_index;
                    ++index;
				}
			}
            gen->m_fnc_args.insert({stmt_def_func.name.value.value(), args});
            m_fnc_rets.insert({stmt_def_func.name.value.value(), stmt_def_func.return_type});	 
 
      gen->m_stack_size_rel = 0;
			for (const auto& stmt : stmt_def_func.code_branch) {
				gen->gen_stmt(stmt);
			}
      size_t needed_stack_size = gen->m_stack_size - stack_start;
      gen->m_stack_size = stack_start;
      if (((needed_stack_size * 8) % 2) != 0) needed_stack_size += 1;
      gen->write_stmt = true;
      gen->write("  sub $" + std::to_string(needed_stack_size * 8) + ", %rsp");
      gen->write(gen->buf.str(), false);
      gen->buf.str("");
      //gen->buf << old_buf;
			//std::cerr << std::to_string(gen->m_stack_size) << "\n";
			gen->write("  mov %rbp, %rsp");
            gen->write("  pop %rbp");
			--gen->m_stack_size;
            gen->write("  ret");
            gen->current_mode = Mode::Global;
            gen->current_func = "";

      while (gen->m_vars.size() > var_n) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }

			gen->write("  sub $" + std::to_string(gen->m_stack_size - stack_start) + ", %rsp");
			while (gen->m_stack_size > stack_start) {
				--gen->m_stack_size;
            }	
        }

        void operator()(const NodeStmtEndfn& stmt_end_fn) const { // NOTE: OBSOLETE AND NOT USED FUNCTION
            gen->write("  leave");
            gen->write("  ret");
            gen->current_mode = Mode::Global;
            gen->current_func = "";
        }

        void operator()(const NodeStmtRet& stmt_ret) const {
            NodeExpr value = stmt_ret.value;
            Type type = replace_user_defined_type_with_generic(check_value(value, gen), gen);
            if (type.kind != Type::Kind::Float && type.kind != Type::Kind::Struct) gen->gen_expr(value, false, "rax");
            else if (type.kind == Type::Kind::Float) gen->gen_expr(value, false, "xmm0");
            else if (type.kind == Type::Kind::Struct) {
              if (std::holds_alternative<NodeExprStruct>(value.var)) {
                NodeExprStruct expr_struct = std::get<NodeExprStruct>(value.var);
                int index = 0;
                for (const auto& field : expr_struct.fields) {
                  gen->gen_expr(*field);
                  gen->write("  mov %rax, " + std::to_string(index * 8) + "(%rbx)");
                  ++index;
                }
                gen->write("  mov %rbx, %rax");
              } else if (std::holds_alternative<NodeExprIdent>(value.var)) {
                NodeExprIdent expr_ident = std::get<NodeExprIdent>(value.var);
                size_t offset = gen->get_var(expr_ident.ident.value.value(), expr_ident.line);
                gen->write("  mov " + std::to_string(offset) + "(%rsp), %rax");
              }
            }
			gen->m_fnc_custom_ret.insert({gen->current_func, check_value(value, gen)});

            gen->write("  leave"); 
            gen->write("  ret");
            //gen->current_mode = Mode::Global;
            //gen->current_func = "";
        }

        void operator()(const NodeStmtUnload& stmt_unload) const {
            for (const auto& element : stmt_unload.vars) {
                std::string name = element.value.value();
                auto it = gen->m_vars.find(name);
                if (it == gen->m_vars.end()) {
                    add_error("Tried to unload an inexistent variable", stmt_unload.line);
                }
                size_t offset = it->second.stack_loc;
                gen->write("  add $" + std::to_string(offset) + ", %rsp");
                gen->pop("rax");
                gen->write("  sub $" + std::to_string(offset) + ", %rsp");
                gen->m_vars.erase(it);  // borrar sin invalidar iterador de bucle
                auto& names = gen->m_vars_order;
                names.erase(
                    std::remove(names.begin(), names.end(), name),
                    names.end()
                ); 
            }
        }

        void operator()(const NodeStmtCall& stmt_call) const {
            const std::string& fn = stmt_call.name.value.value();
            std::vector<NodeExprPtr> arg_values = stmt_call.args;
			call_func(fn, arg_values, gen, stmt_call.line, false);
        }

        void operator()(const NodeStmtProperty& stmt_property) const {
			    const std::string& ident = stmt_property.ident.value.value();
			    const std::string& property = stmt_property.property.value.value();
			    int line = stmt_property.line;

			    gen->gen_expr(stmt_property.expr, false);

			    if (gen->m_vars.contains(ident)) {
				    if (gen->m_vars.at(ident).struct_template == "") {
					    add_error("The variable whose property was tried to access, it is not a struct", line);
					    return;
				    }
				
				    size_t stack_abs_pos;
				    for (const auto& struct_template : gen->m_structs) {
					    if (struct_template.first != gen->m_vars.at(ident).struct_template) continue;
					    size_t stack_rel_pos = (struct_template.second.size() - 2) * 8;
					    for (const auto& prop : struct_template.second) {
						    if (prop.first != property) stack_rel_pos -= 8;	
						    else break;
					    }
					    stack_abs_pos = ((gen->m_stack_size - gen->m_vars.at(ident).stack_loc) * 8) + stack_rel_pos;
					    gen->write("  mov %rax, " + std::to_string(stack_abs_pos) + "(%rsp)");
				    }
			    }
        }

        void operator()(const NodeStmtDeclmod& stmt_declmod) const {
            const std::string& ident = stmt_declmod.module_name.value.value();
            gen->mod = Mode::Mod;
            gen->current_mod = ident;
            m_mod.push_back(ident);
        }
        void operator()(const NodeStmtEndmod& stmt_endmod) const {
            gen->current_mod = "";
            gen->mod = Mode::Global;
        }
        void operator()(const NodeStmtUmod& stmt_umod) const {
            const std::string& name = stmt_umod.module_name.value.value();
            int mod_exists = false;
            for (int i = 0; i < m_mod.size(); ++i) {
                if (m_mod[i] == name) mod_exists = true;
            }
            if (!mod_exists) {
                add_error("Module '" + name + "' not found", stmt_umod.line);
            }

            for (const auto& fnc : m_fnc_mod) {
                if (fnc.first == name) gen->write("  .extern " + fnc.second);
            }
        }
        void operator()(const NodeStmtUbeepmod& stmt_umod) const {
            const std::string& name = stmt_umod.module_name.value.value();
            const std::vector<std::string> beepmodules = {
                "string",
                "std"
            };

            for (const std::string& module : beepmodules) {
                if (name == module) {
                    const std::string& loc = "/usr/include/beepc/" + name + ".o";
                    modules.push_back(loc);
                    return;
                }
            }
            add_error("Unknown beep module", stmt_umod.line);
        }

        void operator()(const NodeStmtLlibrary& stmt_llibrary) const {
            gen->libraries.push_back(stmt_llibrary.name.value.value());
        }
        void operator()(const NodeStmtLibpath& stmt_libpath) const {
            gen->libpaths.push_back(stmt_libpath.path.value.value());
        }

		void operator()(const NodeStmtSetPtr& stmt_ptr) const {
			Type value_type = check_value(stmt_ptr.expr, gen);
			if (value_type.kind != Type::Kind::Float) gen->gen_expr(stmt_ptr.expr, false, "rax");
			else gen->gen_expr(stmt_ptr.expr, false, "xmm0");

			if (gen->m_vars.contains(stmt_ptr.ident.value.value())) {	
				size_t offset_bytes = gen->get_var(stmt_ptr.ident.value.value(), stmt_ptr.line);
				if (value_type.kind != Type::Kind::Float) {
					gen->write("  mov " + std::to_string(offset_bytes) + "(%rsp), %rbx");
					gen->write("  mov %rax, (%rbx)");
				}
			}
			else if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == stmt_ptr.ident.value.value()) {
                                if (var.type.kind != Type::Kind::Float) {
									gen->write("  mov -" + std::to_string(var.stack_loc) + "(%rbp), %rbx");
									gen->write("  mov %rax, (%rbx)");
								} 
								return;
                            }
                        }
                    }
                }
            }
			else {
				add_error("Unexistent variable\n", stmt_ptr.line);
			}	
		}

		void operator()(const NodeStmtGlobl& stmt_globl) const {
			const std::string& name = stmt_globl.ident.value.value();

			if (!gen->m_vars.contains(name)) {
				add_error("Inexistent variable", stmt_globl.line);
				return;
			}
			Var var = gen->m_vars.at(name);
			//gen->m_glob_vars.insert({name, var});

			const std::string& var_name = name;
            size_t offset_bytes = gen->get_var(var_name, stmt_globl.line);
            if (gen->m_vars.at(var_name).type.kind != Type::Kind::Float) gen->write("  mov " +  std::to_string(offset_bytes) +" (%rsp), %rax");
            else gen->write("  movsd " + std::to_string(offset_bytes) + "(%rsp), %rax");


			gen->write("  mov %rax, " + name + "(%rip)");
		}

        void operator()(const NodeStmtHeader& stmt_header) const {
            gen->is_header = true;
        }
        void operator()(const NodeStmtUhead& stmt_uhead) const {
            const std::string& name = stmt_uhead.path.value.value();
            std::ifstream input_file(name);
            if (!input_file.is_open()) {
                std::cerr << "Error: Could not open " << name << "\n";
            }

            std::stringstream full_source;
            std::string line;
            current_source_file = name;

            while (std::getline(input_file, line)) {
                full_source << line << '\n';
                ++current_line;
            }
            std::string source_code = full_source.str();

            Tokenizer tokenizer(std::move(source_code));
            std::vector<Token> tokens = tokenizer.tokenize();

            Parser parser(std::move(tokens));
            std::optional<NodeProg> program = parser.parse_prog();

            if (!program.has_value()) {
                std::cerr << "Parsing failed in file: " << name << "\n";
            }

            std::string base_name = fs::path(name).stem().string();
            //std::string gen_name = (index == 0) ? "main" : base_name;
            std::string gen_name = base_name;

            Generator generator(program.value(), gen_name);
            std::string asm_code = generator.gen_prog();
            for (const auto& glob_var : generator.m_glob_vars) {
              auto modified_var = glob_var;
              modified_var.second.is_declared = false;
              gen->m_glob_vars.insert(modified_var);
            }
            for (const auto& declared_func : generator.declared_funcs) {
              gen->declared_funcs.insert(declared_func);
            }

        }

		void operator()(const NodeStmtLeave& stmt_leave) const {
			gen->write("  mov %rbp, %rsp");
            gen->pop("rbp");
            gen->write("  ret");
		}

		void operator()(const NodeStmtListElement& stmt_list_element) const {
		}

		void operator()(const NodeStmtStruct& stmt_struct) const {
			std::string name = stmt_struct.name.value.value();
			gen->m_structs.insert({name, stmt_struct.fields});
		}
    };

    StmtVisitor visitor{.gen = this};
    std::visit(visitor, stmt.var);
}

[[nodiscard]] std::string Generator::gen_prog() {
	declared_funcs.clear();
	m_output << ".extern malloc\n"; // Necessary to many things

    //m_output << "main:\n";
    for (const NodeStmt& stmt : m_prog.stmts) {
        gen_stmt(stmt); 
    }
    if (main_func_declared) {
        main_func_declared = -5;
        m_output << ".globl main\n";
    }
 
    m_output << function_buffer.str();
    //m_output << "main:\n";
    //m_output << filename << ":\n";
    //m_output << main_buffer.str();
    
    m_output << ".section .data\n";
	  if (!m_glob_vars.empty()) {
		for (const auto& var : m_glob_vars) {
      if (!var.second.is_declared) continue;
      m_output << ".globl " << var.first << "\n";
      NodeExpr expr = var.second.expr;
      if (std::holds_alternative<NodeExprIntLit>(expr.var)) {
        std::string val = std::get<NodeExprIntLit>(expr.var).int_lit.value.value();
        m_output << var.first << ": .quad " << val << "\n";
      } else if (std::holds_alternative<NodeExprStrLit>(expr.var)) {
        std::string val = std::get<NodeExprStrLit>(expr.var).str_lit.value.value();
        bool stop = false;
        for (int i = 0; i < m_string_literals.size(); ++i) {
          if (m_string_literals[i] == val) {
            m_output << var.first << ": .quad str_" << std::to_string(i) << "\n";
            stop = true;
            break;
          }
        }

        std::string label = "str_" + std::to_string(m_string_literals.size());
        m_string_literals.push_back(val);
        m_output << var.first << ": .quad " << label << "\n";
      } else if (std::holds_alternative<NodeExprFloatLit>(expr.var)) {
        std::string val = std::get<NodeExprFloatLit>(expr.var).float_lit.value.value();
        m_output << var.first << ": .double " << val << "\n";
      } else {
        add_error("Expected constant value as initial valua for global variable", 0);
      }
		}
	}

    m_output << ".section .rodata\n";
    if (!m_string_literals.empty()) {
        for (size_t i = 0; i < m_string_literals.size(); ++i) {
            m_output << "str_" << i << ":";
            m_output << " .string \"" << escape_string(m_string_literals[i]) << "\"\n";
        }
    }
    if (!m_float_literals.empty()) {
        for (size_t i = 0; i < m_float_literals.size(); ++i) {
            m_output << "float_" << i << ":";
            m_output << " .double " << m_float_literals[i] << "\n";
        }
    }	

    return m_output.str();
}
