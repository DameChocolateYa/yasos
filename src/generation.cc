#include "generation.hh"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <variant>
#include <cstring>
#include <filesystem>
#include <any>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include "global.hh"
#include "parser.hh"
#include "error.hh"
#include "tokenization.hh"

namespace fs = std::filesystem;

int main_func_declared = false;

std::vector<std::string> modules;

std::vector<std::string> m_mod;
std::unordered_map<std::string, std::string> m_fnc_mod;
std::unordered_map<std::string, Type> m_fnc_rets;

llvm::LLVMContext TheContext;
std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("MainModule", TheContext);

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


llvm::Type* map_type_to_llvm(const Type& t) {
    using Kind = Type::Kind;

    switch (t.kind) {
        case Kind::Int:
            return llvm::Type::getInt32Ty(TheContext);   // i32
        /*case Kind::Bool:
            return llvm::Type::getInt1Ty(TheContext);    // i1*/
        case Kind::Float:
            return llvm::Type::getDoubleTy(TheContext);   // float
        case Kind::Str:
            return llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext));
        case Kind::None:
            return llvm::Type::getInt32Ty(TheContext);   // i32eturn nullptr;  // todavía no definido
        case Kind::List:
            // Para listas, usualmente puntero a tipo base
            return llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(TheContext)); // ejemplo int*
        case Kind::Struct:
            // Structs dinámicos
            return llvm::StructType::create(TheContext, t.user_type);
        case Kind::Any:
            return llvm::PointerType::getVoidTy(TheContext);
        default:
            return nullptr;
    }
}


bool is_string_literal(llvm::Value* val) {
  if (auto* gv = llvm::dyn_cast<llvm::GlobalVariable>(val)) {
    if (gv->hasInitializer()) {
      auto* init = gv->getInitializer();
      if (auto* cdata = llvm::dyn_cast<llvm::ConstantDataArray>(init)) {
        return cdata->isString();
      }
    }
  }
  return false;
}

static Type replace_user_defined_type_with_generic(Type type, Generator* gen) {
  if (type.kind != Type::Kind::UserDefined) return type;

  const std::string& name = type.user_type;

  // Is a struct?
  for (const auto& strct : gen->m_structs) {
    if (strct.first == name) return Type{Type::Kind::Struct};
  }

  return type;
}

static llvm::Type* get_var_type(const std::string& name, Generator* gen) {
  if (gen->m_vars.contains(name)) {
    return gen->m_vars.at(name).type;
  }
  else if (gen->current_mode == Mode::Function) {
      if (!gen->m_fnc_args.contains(gen->current_func)) {
      }
      for (const auto& arg : gen->m_fnc_args.at(gen->current_func)) {
          if (arg.name == name) {
              return arg.type;
          }
      }
  }
}

/*static llvm::Type* check_value(const NodeExpr& expr, Generator* gen, bool get_raw_value = false) {
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
    NodeExprListElement list_ele = std::get<NodeExprListElement>(expr.var);
    Type type = get_var_type(list_ele.list_name.value.value(), gen);
		return type;
	} else if (std::holds_alternative<NodeExprStruct>(expr.var)) return Type{Type::Kind::Struct};

    return Type{Type::Kind::None};
}*/


static void set_value(Generator* gen, llvm::Value* target, llvm::Value* value) {
}

static std::optional<llvm::Value*> call_func(const std::string& fn, std::vector<NodeExprPtr> arg_values, Generator* gen, int line, bool direct_call = false) {
  std::vector<llvm::Value*> values;

  for (const auto& arg_val : arg_values) {
    values.push_back(gen->gen_expr(*arg_val));
  }

  llvm::Function* func = gen->ModModule->getFunction(fn);
  if (!func) {
    add_error("Inexistent function (" + fn + ")", line);
    return std::nullopt;
  }

  llvm::Value* call = gen->Builder.CreateCall(func, values, fn + "_res");
  if (!func->getReturnType()->isVoidTy()) {
    return call;
  }

  return std::nullopt;
}

llvm::Value* Generator::gen_expr(const NodeExpr& expr, bool push_result, const std::string& reg, bool is_value, bool is_func_call) {
    struct ExprVisitor {
        Generator* gen;
        bool push_result;
        std::string reg;
        bool is_value;
		bool is_func_call;

    llvm::Value* operator()(const NodeExprIntLit& expr_int_lit) const {
            //gen->write("  mov $" + expr_int_lit.int_lit.value.value() + ", %" + reg);
            //gen->emit_op("$" + expr_int_lit.int_lit.value.value(), reg, Op::Mov, 4);
            //if (push_result) gen->push(reg, 4);
            /*if (push_result) {
              gen->push("$" + expr_int_lit.int_lit.value.value(), 4);
              return;
            }
            gen->emit_op("$" + expr_int_lit.int_lit.value.value(), reg, Op::Mov, 4);*/
          return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(TheContext),
            std::stoi(expr_int_lit.int_lit.value.value_or("0"))
          ); 
        }

    llvm::Value* operator()(const NodeExprBinary& expr_bin) const {
      llvm::Value* lhs_val = gen->gen_expr(*expr_bin.lhs);
      llvm::Value* rhs_val = gen->gen_expr(*expr_bin.rhs);

      llvm::Type* v1 = lhs_val->getType();
      llvm::Type* v2 = rhs_val->getType();

      if (v1->isDoubleTy() || v2->isDoubleTy()) {
        if (v1->isDoubleTy()) {
          lhs_val = gen->Builder.CreateFPExt(lhs_val, llvm::Type::getDoubleTy(TheContext), "f_lhs");
          v1 = lhs_val->getType();
        }
        if (v2->isDoubleTy()) {
          rhs_val = gen->Builder.CreateFPExt(rhs_val, llvm::Type::getDoubleTy(TheContext), "f_rhs");
          v2 = rhs_val->getType();
        }
      }

      llvm::Value* result;
      switch (expr_bin.op_token.type) {
        case TokenType::plus:
          if (v1->isDoubleTy() || v2->isDoubleTy()) result = gen->Builder.CreateFAdd(lhs_val, rhs_val, "faddtmp");
          else if (is_string_literal(lhs_val) && is_string_literal(rhs_val)) {

            std::vector<llvm::Type*> param_types = {llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext)), llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext))};

            llvm::Function* func = gen->ModModule->getFunction("strcat");
            if (func) {
              result = gen->Builder.CreateCall(func, {lhs_val, rhs_val}, "strcattmp");
              break;
            }

            llvm::FunctionType* func_type = llvm::FunctionType::get(
              llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext)),
              param_types,
              false
            );
        
            llvm::Function* strcat_func = llvm::Function::Create(
              func_type,
              llvm::Function::ExternalLinkage,
              "strcat",
              gen->ModModule.get()
            );  
            result = gen->Builder.CreateCall(strcat_func, {lhs_val, rhs_val}, "strcat");
          }
          else result = gen->Builder.CreateAdd(lhs_val, rhs_val, "add");
          break;
        case TokenType::minus:
          if (v1->isDoubleTy() || v2->isDoubleTy()) result = gen->Builder.CreateFSub(lhs_val, rhs_val, "fsub");
          else result = gen->Builder.CreateSub(lhs_val, rhs_val, "sub");
          break;
        case TokenType::star:
          if (v1->isDoubleTy() || v2->isDoubleTy()) result = gen->Builder.CreateFMul(lhs_val, rhs_val, "fmul");
          else result = gen->Builder.CreateMul(lhs_val, rhs_val, "mul");
          break;
        case TokenType::slash:
          if (v1->isDoubleTy() || v2->isDoubleTy()) result = gen->Builder.CreateFDiv(lhs_val, rhs_val, "fdiv");
          else result = gen->Builder.CreateSDiv(lhs_val, rhs_val, "div");
          break;
        case TokenType::percent:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            llvm::Value* div = gen->Builder.CreateFDiv(lhs_val, rhs_val, "div");
            llvm::Value* trunc = gen->Builder.CreateFPToSI(div, llvm::Type::getInt32Ty(TheContext));
            llvm::Value* mul = gen->Builder.CreateFMul(rhs_val, gen->Builder.CreateSIToFP(trunc, v1, "mul"));
            result = gen->Builder.CreateFSub(lhs_val, mul, "fmod");
          }
          else result = gen->Builder.CreateSRem(lhs_val, rhs_val, "mod");
          break;
        case TokenType::eq_eq:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpUEQ(lhs_val, rhs_val, "ueq");
          } else {
            result = gen->Builder.CreateICmpEQ(lhs_val, rhs_val, "seq");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "cmp");
          break;
        case TokenType::bang_eq:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpUNE(lhs_val, rhs_val, "une");
          } else {
            result = gen->Builder.CreateICmpNE(lhs_val, rhs_val, "ine");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "neq");
          break;
        case TokenType::l_arrow:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpULT(lhs_val, rhs_val, "ult");
          } else {
            result = gen->Builder.CreateICmpSLT(lhs_val, rhs_val, "slt");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "lt");
          break;
        case TokenType::lte:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpULE(lhs_val, rhs_val, "ule");
          } else {
            result = gen->Builder.CreateICmpSLE(lhs_val, rhs_val, "sle");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "le");
          break;
        case TokenType::r_arrow:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpUGT(lhs_val, rhs_val, "fgt");
          } else {
            result = gen->Builder.CreateICmpSGT(lhs_val, rhs_val, "sgt");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "gt");
          break;
        case TokenType::gte:
          if (v1->isDoubleTy() || v2->isDoubleTy()) {
            result = gen->Builder.CreateFCmpUGE(lhs_val, rhs_val, "fge");
          } else {
            result = gen->Builder.CreateICmpSGE(lhs_val, rhs_val, "sge");
          }
          result = gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "ge");
          break;
        case TokenType::_and:  
        case TokenType::_or: { 
          
          llvm::Function* function = gen->Builder.GetInsertBlock()->getParent();
          llvm::BasicBlock* entry_block = gen->Builder.GetInsertBlock(); // antes de cambiar a rhsBlock
          llvm::BasicBlock* rhs_block = llvm::BasicBlock::Create(TheContext, "rhs", function);
          llvm::BasicBlock* end_block = llvm::BasicBlock::Create(TheContext, "end", function);

          llvm::Value* lhs_bool = gen->Builder.CreateICmpNE(lhs_val, llvm::ConstantInt::get(lhs_val->getType(), 0), "and_or_lhs");
          if (expr_bin.op_token.type == TokenType::_and) gen->Builder.CreateCondBr(lhs_bool, rhs_block, end_block);
          else gen->Builder.CreateCondBr(lhs_bool, end_block, rhs_block);

          gen->Builder.SetInsertPoint(rhs_block);
          llvm::Value* rhs_bool = gen->Builder.CreateZExt(rhs_val, llvm::Type::getInt32Ty(TheContext), "and_or_rhs");
          gen->Builder.CreateBr(end_block);

          gen->Builder.SetInsertPoint(end_block);
          llvm::PHINode* phi = gen->Builder.CreatePHI(llvm::Type::getInt32Ty(TheContext), 2, "phi");
          if (expr_bin.op_token.type == TokenType::_and) {
            phi->addIncoming(llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0), entry_block);
            phi->addIncoming(rhs_bool, rhs_block);
          } else {
            phi->addIncoming(llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1), entry_block);
            phi->addIncoming(rhs_bool, rhs_block);
          }
          result = phi;
          break;
        }
        default:
          add_error("Binary Operator (" + expr_bin.op_token.value.value_or("NULL_TOK") + ") not supported", expr_bin.line);
          break;
      }

      return result;
    }
        
    llvm::Value* operator()(const NodeExprUnary& expr_unary) const {
      llvm::Value* val = gen->gen_expr(*expr_unary.expr);
      llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
      llvm::Value* cmp = gen->Builder.CreateICmpEQ(val, zero);
      return gen->Builder.CreateZExt(cmp, llvm::Type::getInt32Ty(TheContext));
    }

    llvm::Value* operator()(const NodeExprBinaryAssign& expr_bin_assign) const {
            /*const std::string& op = expr_bin_assign.op_token.value.value();
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
                else {
                    gen->write("  mov %rax, %rdi");
                    gen->write("  mov %rbx, %rsi");
                    gen->call("strcat");
                }
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
            //if (push_result) gen->push("rax");*/
        }
    llvm::Value* operator()(const NodeExprUnaryIncDec& expr_unary_operator) const {
            /*const std::string& op = expr_unary_operator.op_token.value.value();
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
            //if (push_result) gen->push("rax");*/
        };

        llvm::Value* operator()(const NodeExprIdent& expr_ident) const {
          const std::string& name = expr_ident.ident.value.value();
          if (gen->m_vars.contains(name)) {
            if (gen->m_vars.at(name).is_globl) {
              llvm::GlobalVariable* var = gen->ModModule->getGlobalVariable(name);
              return gen->Builder.CreateLoad(var->getValueType(), var, name);
            } else {
              llvm::Value* var = gen->m_vars.at(name).var_ptr;
              return gen->Builder.CreateLoad(gen->m_vars.at(name).type, var, name);
            }
          }

          return nullptr;
        }

        llvm::Value* operator()(const NodeExprStrLit& expr_str_lit) const {
          std::string value = expr_str_lit.str_lit.value.value();
          llvm::Value* str_ptr = gen->Builder.CreateGlobalStringPtr(value, "str");

          return str_ptr;
        }

        llvm::Value* operator()(const NodeExprFloatLit& expr_float_lit) const {
          return llvm::ConstantFP::get(
            llvm::Type::getDoubleTy(TheContext),
            std::stof(expr_float_lit.float_lit.value.value())
          );
        }

        llvm::Value* operator()(const NodeExprNone& expr_none) const {
          return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0); 
        }
        llvm::Value* operator()(const NodeExprNoArg& expr_no_arg) const {}
        llvm::Value* operator()(const NodeExprCR& expr_cr) const {}

        llvm::Value* operator()(const NodeExprBoolValue& expr_bool) const {
            gen->write("  mov $" + std::to_string(expr_bool.value) + ", %" + reg);
            if (push_result) gen->push(reg);
        }

        llvm::Value* operator()(const NodeExprCall& expr_call) const {
			    const std::string& fn = expr_call.name.value.value();
          std::vector<NodeExprPtr> arg_values = expr_call.args;
          std::optional<llvm::Value*> ret_val = call_func(fn, arg_values, gen, expr_call.line, false);
          if (!ret_val.has_value()) {

          }
          return *ret_val;
		}

        llvm::Value* operator()(const NodeExprProperty& stmt_property) const {
		    }	
		
        llvm::Value* operator()(const NodeExprGetPtr& expr_ptr) const {
			/*if (gen->m_vars.contains(expr_ptr.ident.value.value())) {	
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
			}*/	
		}

        llvm::Value* operator()(const NodeExprDeref& expr_ref) const {
      /*const std::string& expr_reg = check_value(*expr_ref.expr, gen).kind != Type::Kind::Float ? reg : "xmm0"; 
      gen->gen_expr(*expr_ref.expr, false, expr_reg.c_str());

      if (is_value) gen->emit_op("(%" + expr_reg + ")", reg, Op::Mov, 8, false, false, true);
      if (push_result) gen->push(reg, 8, true);*/
    }

        llvm::Value* operator()(const NodeExprList& expr_list) const {
      /*size_t ele_size = size_of(check_value(expr_list.elements[0], gen).kind);
			size_t shadow_space = expr_list.elements.size() * ele_size;
      gen->emit_op("$" + std::to_string(shadow_space), "rdi", Op::Mov, 4);
			gen->call("malloc");
      gen->emit_op("rax", "rbx", Op::Mov, 8, false, false, true);

      Type var_type;
			for (int i = 0; i <= expr_list.elements.size() - 1; ++i) {
				const auto& arg = expr_list.elements[i];
				var_type = check_value(arg, gen);
				if (var_type.kind != Type::Kind::Float && var_type.kind != Type::Kind::List) {
					gen->gen_expr(arg, false, "rax");
          gen->emit_op("rax", std::to_string(i * ele_size) + "(%rbx)", Op::Mov, size_of(var_type.kind));
				}
				else if (var_type.kind == Type::Kind::Float) {
					gen->gen_expr(arg, false, "xmm0");
          gen->emit_op("xmm0", std::to_string(i * ele_size) + "(%rbx)", Op::Mov, size_of(var_type.kind), true, true);
				}
			}
			if (reg != "rbx") gen->emit_op("rbx", reg, Op::Mov, size_of(var_type.kind), var_type.kind == Type::Kind::Float);
      if (push_result) gen->push(reg, size_of(var_type.kind));*/
		}

        llvm::Value* operator()(const NodeExprListElement& expr_list_element) const {
			/*const std::string& var_name = expr_list_element.list_name.value.value();
			if (check_value(*expr_list_element.index, gen).kind != Type::Kind::Int) {
				add_error("Expected Integer expression as index", expr_list_element.line);
				return;
			}
      bool is_arg = false;
      size_t ele_size = 0;

      size_t stack_loc;
      Type type;
      if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == var_name) {
                                stack_loc = var.stack_loc;
                                is_arg = true;
                                ele_size = size_of(var.type.kind);
                                type = var.type;
                                //gen->write("  mov rax, rsi");
                            }
                        }
                    }
                }
            }
      if (gen->m_vars.contains(var_name) && !is_arg) {
        stack_loc = gen->get_var(var_name, expr_list_element.line);
        ele_size = size_of(gen->m_vars.at(var_name).type.kind);
        type = gen->m_vars.at(var_name).type;
      }
			gen->gen_expr(*expr_list_element.index, false, "rax");

      if (gen->m_glob_vars.contains(var_name)) {
        type = gen->m_glob_vars.at(var_name).type;
        ele_size = size_of(type.kind);
        gen->emit_op(var_name + "(%rip)", "rbx", Op::Mov, 8);
        gen->emit_op("(%rbx,%rax," + std::to_string(ele_size) + ")", reg, Op::Mov, ele_size, type.kind == Type::Kind::Float, false, type.is_ref);
        if (push_result) gen->push(reg, ele_size);
        return;
      }

      if (!is_arg) gen->emit_op(std::to_string(stack_loc) + "(%rsp)", "rbx", Op::Mov, 8);
      else gen->emit_op(std::to_string(stack_loc) + "(%rsp)", "rbx", Op::Mov, 8);

      gen->emit_op("(%rbx,%rax," + std::to_string(ele_size) + ")", reg, Op::Mov, ele_size, type.kind == Type::Kind::Float, false, false);

			if (push_result) gen->push(reg, ele_size);*/
		}

        llvm::Value* operator()(const NodeExprStruct& expr_struct) const {
		}

        llvm::Value* operator()(const NodeExprNew& stmt_new) const {
      /*if (stmt_new.type.kind != Type::Kind::Struct) {
        gen->emit_op("$" + std::to_string(size_of(stmt_new.type.kind)), "rdi", Op::Mov, 4);
        gen->write("  call malloc");

        if (push_result) gen->push("rax", 8); // Pointers always need 8 bytes
        else if (reg != "rax") gen->emit_op("rax", reg, Op::Mov, 8);
      }*/
    }

    };

    ExprVisitor visitor{.gen = this, .push_result = push_result, .reg = reg, .is_value = is_value, .is_func_call = is_func_call};
    llvm::Value* value = std::visit(visitor, expr.var);

    return value;
}

void Generator::gen_stmt(const NodeStmt& stmt) {
    struct StmtVisitor {
        Generator* gen;

        void operator()(const NodeStmtAsmUserWrite& stmt_asm) const {
          /*int index = 0;
          for (const auto& instruction : stmt_asm.format_instructions) {
            std::string output = ""; 

            for (char c : instruction) {
              int expr_index = 0;

              if (c == '!') {
                if (expr_index >= stmt_asm.exprs[index].size()) {
                  add_error("Expressions amount is greater than expected", stmt_asm.line);
                  return;
                }
                NodeExpr expr = stmt_asm.exprs[index][expr_index];
                ++expr_index;

                if (std::holds_alternative<NodeExprIntLit>(expr.var)) {
                  output += "$" + get_raw_value(expr, gen);
                } else if (std::holds_alternative<NodeExprIdent>(expr.var)) {
                  output += get_raw_value(expr, gen);
                }
              } else {
                output += c;
              }
            }
            ++index;

            gen->write("  " + output + " # INCRUSTED");
          }*/
        }

        void operator()(const NodeStmtImport& stmt_import) const {
			const std::string& name = stmt_import.mod_name.value.value();
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

            auto temp_module = std::make_unique<llvm::Module>("temp", TheContext);
            Generator generator(program.value(), name, std::move(temp_module)); // pasa módulo temporal
            generator.gen_prog();

            llvm::SmallVector<llvm::ReturnInst*, 8> Returns;

            for (auto& fn_name : stmt_import.to_import) {
              if (auto* f = generator.ModModule->getFunction(fn_name)) {
                llvm::ValueToValueMapTy VMap;
                llvm::Function* new_f = llvm::Function::Create(
                f->getFunctionType(),
                llvm::GlobalValue::ExternalLinkage,
                f->getName(),
                gen->ModModule.get()  // módulo destino 
                );

                auto new_arg_it = new_f->arg_begin();
                  for (auto &Arg : f->args()) {
                    VMap[&Arg] = &*new_arg_it;
                    ++new_arg_it;
                  }

                llvm::CloneFunctionInto(new_f, f, VMap, llvm::CloneFunctionChangeType::LocalChangesOnly, Returns);
                new_f->copyAttributesFrom(f);
              }
            }

            llvm::ValueToValueMapTy VMap;
            for (auto& var_name : stmt_import.to_import) {
              if (auto* g = generator.ModModule->getGlobalVariable(var_name)) {
                
                llvm::GlobalVariable* new_g = new llvm::GlobalVariable(
                *gen->ModModule,
                g->getValueType(),
                g->isConstant(),
                g->getLinkage(),
                nullptr,
                g->getName() 
                );

                VMap[g] = new_g;
                
                if (g->hasInitializer()) {
                  llvm::Constant* new_init = llvm::MapValue(g->getInitializer(), VMap);
                  new_g->setInitializer(new_init);
                } 
                
                new_g->setAlignment(g->getAlign());
                new_g->setThreadLocalMode(g->getThreadLocalMode());
                new_g->setExternallyInitialized(g->isExternallyInitialized());
              }
            } 

            std::vector<std::string> to_import = stmt_import.to_import;
            for (const auto& glob_var : generator.m_vars) {
              if (std::find(to_import.begin(), to_import.end(), glob_var.first) == to_import.end()) continue;
              auto modified_var = glob_var;
              //modified_var.second.is_declared = false;
              gen->m_vars.insert(modified_var);
            }
            for (const auto& declared_func : generator.declared_funcs) {
              if (std::find(to_import.begin(), to_import.end(), declared_func.first) == to_import.end()) continue;
              gen->declared_funcs.insert(declared_func);
            }
            for (const auto& fnc_custom_ret : generator.m_fnc_custom_ret) {
              if (std::find(to_import.begin(), to_import.end(), fnc_custom_ret.first) == to_import.end()) continue;
              gen->m_fnc_custom_ret.insert(fnc_custom_ret);
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
          llvm::Type* llvm_type = map_type_to_llvm(stmt_var.type);
          const std::string& name = stmt_var.ident.value.value(); 

          llvm::Value* var_ptr = nullptr;
          llvm::Value* init_val = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(TheContext), 0 
          );
          if (stmt_var.has_initial_value) {
            init_val = gen->gen_expr(stmt_var.expr);
          }

          llvm::Constant* initializer = nullptr;

          if (stmt_var.has_initial_value) {
            if (llvm::Constant* c = llvm::dyn_cast<llvm::Constant>(init_val)) {
              initializer = c;
            } else {
              initializer = llvm::Constant::getNullValue(llvm_type);
            }
          } else {
            initializer = llvm::Constant::getNullValue(llvm_type);
          }

          if (gen->current_mode == Mode::Global) {
            var_ptr = new llvm::GlobalVariable(
              *gen->ModModule,
              llvm_type,
              !stmt_var.is_mutable,
              llvm::GlobalValue::ExternalLinkage,
              initializer, 
              stmt_var.ident.value.value()
            );
          } else {
            var_ptr = gen->Builder.CreateAlloca(llvm_type, nullptr, name);
          }

          if (gen->current_mode != Mode::Global) gen->Builder.CreateStore(init_val, var_ptr);
          gen->insert_var(stmt_var.ident.value.value(), llvm_type, var_ptr, stmt_var.is_mutable, gen->current_mode == Mode::Global);
        }
         
        void operator()(const NodeStmtAssign& stmt) const { // -> value, target, op_token
          llvm::Value* target_ptr = nullptr;

          if (std::holds_alternative<NodeExprIdent>(stmt.target.var)) {
            NodeExprIdent expr_ident = std::get<NodeExprIdent>(stmt.target.var);
            const std::string& name = expr_ident.ident.value.value();
            if (!gen->m_vars.contains(name)) {
              throw std::runtime_error("Variable '" + name + "' no declarada");
            }
            target_ptr = gen->m_vars.at(name).var_ptr;
          } else {
            throw std::runtime_error("Solo asignaciones a identificadores soportadas por ahora");
          }

          llvm::Value* value = gen->gen_expr(stmt.value);
          if (stmt.op_tok.type == TokenType::plus_eq || stmt.op_tok.type == TokenType::minus_eq || stmt.op_tok.type == TokenType::star_eq || stmt.op_tok.type == TokenType::slash_eq) {
            TokenType tok;
            if (stmt.op_tok.type == TokenType::plus_eq) tok = TokenType::plus;
            else if (stmt.op_tok.type == TokenType::minus_eq) tok = TokenType::minus;
            else if (stmt.op_tok.type == TokenType::star_eq) tok = TokenType::star;
            else tok = TokenType::slash;

            NodeExpr expr_bin = NodeExpr(NodeExprBinary{
              .lhs = std::make_shared<NodeExpr>(stmt.target),
              .op_token = Token {.type = tok, .value = "", .line = stmt.line},
              .rhs = std::make_shared<NodeExpr>(stmt.value),
              .line = stmt.line
            });

            value = gen->gen_expr(expr_bin);
          }

          gen->Builder.CreateStore(value, target_ptr);  
        }

        void operator()(const NodeStmtVarRe& stmt_var) const {
            /*if (gen->current_mode == Mode::Global) {
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
                if (gen->m_vars.at(name).type != value_type) {
                    //std::cerr << static_cast<int>(value_type) << "\n";
                    add_error("Expected same type in reassigment", stmt_var.line);
                }

                size_t offset_bytes = gen->get_var(name, stmt_var.line);
				gen->m_vars.at(name).type = value_type;
                if (value_type.kind != Type::Kind::Float) gen->write("  mov %rax, " + std::to_string(offset_bytes)  + "(%rsp) # Editing var value (" + name + ")");
				else gen->write("  movsd %xmm0, " + std::to_string(offset_bytes)  + "(%rsp) # Editing var value (" + name + ")");
                return;
            }*/
        }

        void operator()(const NodeStmtIf& stmt_if) const {
          static int label_counter = 0;
            
          llvm::Function* function = gen->Builder.GetInsertBlock()->getParent();

          int current = label_counter++;
          llvm::BasicBlock* endBlock = llvm::BasicBlock::Create(TheContext, "endif_" + std::to_string(current), function);
          llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(TheContext, "else_" + std::to_string(current), function);

          llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(TheContext, "if_" + std::to_string(current), function);

          std::vector<llvm::BasicBlock*> elifBlocks;
          for (size_t i = 0; i < stmt_if.elif_conditions.size(); ++i) {
            elifBlocks.push_back(llvm::BasicBlock::Create(TheContext, "elif_" + std::to_string(current) + "_" + std::to_string(i), function)            );
          }

          llvm::Value* condVal = gen->gen_expr(stmt_if.condition);
          llvm::Value* condBool = gen->Builder.CreateICmpNE(condVal,
          llvm::ConstantInt::get(condVal->getType(), 0), "if_cond");

          if (!stmt_if.elif_conditions.empty()) {
            gen->Builder.CreateCondBr(condBool, ifBlock, elifBlocks[0]);
          } else {
            gen->Builder.CreateCondBr(condBool, ifBlock, elseBlock);
          }

          gen->Builder.SetInsertPoint(ifBlock);
          for (const auto& stmt : stmt_if.then_branch) {
            gen->gen_stmt(stmt);
          }
          gen->Builder.CreateBr(endBlock);

          for (size_t i = 0; i < stmt_if.elif_conditions.size(); ++i) {
            gen->Builder.SetInsertPoint(elifBlocks[i]);
            llvm::Value* elifCond = gen->gen_expr(stmt_if.elif_conditions[i]);
            llvm::Value* elifBool = gen->Builder.CreateICmpNE(elifCond,
            llvm::ConstantInt::get(elifCond->getType(), 0), "elif_cond");

            llvm::BasicBlock* nextBlock = (i + 1 < stmt_if.elif_conditions.size()) ? elifBlocks[i + 1] : elseBlock;

            gen->Builder.CreateCondBr(elifBool, llvm::BasicBlock::Create(TheContext, "elif_body_" + std::to_string(current) + "_" + std::to_string(i), function), nextBlock);

            gen->Builder.SetInsertPoint(llvm::cast<llvm::BasicBlock>(gen->Builder.GetInsertBlock()->getTerminator()->getSuccessor(0)));
            for (const auto& stmt : stmt_if.elif_branches[i]) {
              gen->gen_stmt(stmt);
            }
            gen->Builder.CreateBr(endBlock);
          }

          gen->Builder.SetInsertPoint(elseBlock);
          for (const auto& stmt : stmt_if.else_branch) {
            gen->gen_stmt(stmt);
          }
          gen->Builder.CreateBr(endBlock);

          gen->Builder.SetInsertPoint(endBlock);
        }

    void operator()(const NodeStmtWhile& stmt_while) const {
      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function* function = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock* startBlock = llvm::BasicBlock::Create(TheContext, "while_" + std::to_string(current) + "_start", function);
      llvm::BasicBlock* bodyBlock  = llvm::BasicBlock::Create(TheContext, "while_"  + std::to_string(current) + "_body", function);
      llvm::BasicBlock* endBlock   = llvm::BasicBlock::Create(TheContext, "while_"   + std::to_string(current) + "_end", function);
      gen->stmt_orde.push({startBlock, {endBlock, nullptr}});

      gen->Builder.CreateBr(startBlock);

      gen->Builder.SetInsertPoint(startBlock);
      llvm::Value* condVal = gen->gen_expr(stmt_while.condition);
      llvm::Value* condBool = gen->Builder.CreateICmpNE(
        condVal,
        llvm::ConstantInt::get(condVal->getType(), 0)
      );
      gen->Builder.CreateCondBr(condBool, bodyBlock, endBlock);

      gen->Builder.SetInsertPoint(bodyBlock);

      size_t var_n = gen->m_vars.size();
      for (const auto& stmt : stmt_while.then_branch) {
          gen->gen_stmt(stmt);
      }
      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto& var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->Builder.CreateBr(startBlock);

      gen->Builder.SetInsertPoint(endBlock);
      gen->stmt_orde.pop();
    }
    void operator()(const NodeStmtLoop& stmt_loop) const {
      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function* function = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(TheContext, "loop_" + std::to_string(current) + "_start", function);
      llvm::BasicBlock* endBlock  = llvm::BasicBlock::Create(TheContext, "loop_" + std::to_string(current) + "_end", function);
      gen->stmt_orde.push({loopBlock, {endBlock, nullptr}});

      gen->Builder.CreateBr(loopBlock);

      gen->Builder.SetInsertPoint(loopBlock);

      size_t var_n = gen->m_vars.size();
      for (const auto& stmt : stmt_loop.then_branch) {
        gen->gen_stmt(stmt);
      }

      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto& var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->Builder.CreateBr(loopBlock);

      gen->Builder.SetInsertPoint(endBlock);
      gen->stmt_orde.pop();
		}
    
     
    void operator()(const NodeStmtFor& stmt_for) const {
      size_t var_n_before_loop = gen->m_vars.size();

      for (const auto& stmt : stmt_for.init)
        gen->gen_stmt(stmt);

      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function* func = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock* start_bb = llvm::BasicBlock::Create(TheContext, "for_start_" + std::to_string(current), func);
      llvm::BasicBlock* body_bb  = llvm::BasicBlock::Create(TheContext, "for_body_"  + std::to_string(current), func);
      llvm::BasicBlock* update_bb = llvm::BasicBlock::Create(TheContext, "for_update_" + std::to_string(current), func);
      llvm::BasicBlock* end_bb   = llvm::BasicBlock::Create(TheContext, "for_end_"   + std::to_string(current), func);

      gen->stmt_orde.push({start_bb, {end_bb, update_bb}});

      gen->Builder.CreateBr(start_bb);

      gen->Builder.SetInsertPoint(start_bb);
      llvm::Value* cond_val = gen->gen_expr(stmt_for.condition, false);
      llvm::Value* cond_bool = gen->Builder.CreateICmpNE(
      cond_val, llvm::ConstantInt::get(cond_val->getType(), 0), "for_cond");
      gen->Builder.CreateCondBr(cond_bool, body_bb, end_bb);

      gen->Builder.SetInsertPoint(body_bb);
      for (const auto& stmt : stmt_for.code_branch)
        gen->gen_stmt(stmt);
      gen->Builder.CreateBr(update_bb);

      gen->Builder.SetInsertPoint(update_bb);
      for (const auto& stmt : stmt_for.update)
        gen->gen_stmt(stmt);
      gen->Builder.CreateBr(start_bb);

      gen->Builder.SetInsertPoint(end_bb);

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
            llvm::BasicBlock* loop_end = gen->stmt_orde.top().second.first;
            gen->Builder.CreateBr(loop_end);

            llvm::Function* func = gen->Builder.GetInsertBlock()->getParent();
            llvm::BasicBlock* after_stop = llvm::BasicBlock::Create(TheContext, "after_stop");
            gen->Builder.SetInsertPoint(after_stop);
        }

        void operator()(const NodeStmtContinue& stmt_continue) const {
            if (gen->stmt_orde.empty()) {
                add_error("Can not use 'continue' in an inexistent loop", stmt_continue.line);
            }

            if (gen->stmt_orde.top().second.second != nullptr) { // is for
              llvm::BasicBlock* for_update = gen->stmt_orde.top().second.second;
              gen->Builder.CreateBr(for_update);
            } else {
              llvm::BasicBlock* loop_start = gen->stmt_orde.top().first;
              gen->Builder.CreateBr(loop_start);
            }

            llvm::Function* func = gen->Builder.GetInsertBlock()->getParent();
            llvm::BasicBlock* after_continue = llvm::BasicBlock::Create(TheContext, "after_continue");
            gen->Builder.SetInsertPoint(after_continue);
        }
 
        const std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
        const std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};

      void operator()(const NodeStmtDefFunc& stmt_def_func) const {	
        size_t var_n = gen->m_vars.size();
        llvm::Type* ret_type = map_type_to_llvm(stmt_def_func.return_type);

        std::vector<llvm::Type*> param_types;
        for (const auto& c_arg : stmt_def_func.args) {
          param_types.push_back(map_type_to_llvm(c_arg.arg_type));
        }

        llvm::FunctionType* func_type = llvm::FunctionType::get(
          ret_type,
          param_types,
          stmt_def_func.is_vargs
        );
        
        llvm::GlobalValue::LinkageTypes linkage = stmt_def_func.is_extern
        ? llvm::Function::ExternalLinkage
        : llvm::Function::InternalLinkage;

        linkage = stmt_def_func.is_pub
        ? llvm::Function::ExternalLinkage
        : linkage;

        if (stmt_def_func.name.value.value() == "main") linkage = llvm::Function::ExternalLinkage;

        llvm::Function* func = llvm::Function::Create(
          func_type,
          linkage,
          stmt_def_func.name.value.value(),
          gen->ModModule.get()
        ); 

        gen->declared_funcs.insert({stmt_def_func.name.value.value(), {ret_type, param_types}});

        if (stmt_def_func.is_extern) return;

        llvm::BasicBlock* entry = llvm::BasicBlock::Create(TheContext, stmt_def_func.name.value.value(), func);
        gen->Builder.SetInsertPoint(entry);
        gen->current_mode = Mode::Function;

        auto argIt = func->arg_begin();
        for (const auto& c_arg : stmt_def_func.args) {
          argIt->setName(c_arg.name);

          llvm::AllocaInst* alloca_inst = gen->Builder.CreateAlloca(argIt->getType(), nullptr, c_arg.name + "_local");

          gen->Builder.CreateStore(&*argIt, alloca_inst);
          gen->insert_var(c_arg.name, map_type_to_llvm(c_arg.arg_type), alloca_inst, false, false);
          
          ++argIt;
        }

        for (const auto& stmt : stmt_def_func.code_branch) {
          gen->gen_stmt(stmt);
        }

        if (ret_type->getTypeID() != llvm::Type::VoidTyID) {
          gen->Builder.CreateRet(llvm::Constant::getNullValue(ret_type));
        } 

        gen->Builder.ClearInsertionPoint();
        gen->current_mode = Mode::Global;

        while (gen->m_vars.size() > var_n) {
          int i = gen->m_vars_order.size() - 1;
          const auto& var_name = gen->m_vars_order[i];
          gen->m_vars.erase(var_name);
          gen->m_vars_order.pop_back();
        }  
      }

        void operator()(const NodeStmtEndfn& stmt_end_fn) const { // NOTE: OBSOLETE AND NOT USED FUNCTION
            gen->write("  leave");
            gen->write("  ret");
            gen->current_mode = Mode::Global;
            gen->current_func = "";
        }

        void operator()(const NodeStmtRet& stmt_ret) const {
          llvm::Value* ret_val = gen->gen_expr(stmt_ret.value);
          gen->Builder.CreateRet(ret_val);
        }

        void operator()(const NodeStmtUnload& stmt_unload) const {
            /*for (const auto& element : stmt_unload.vars) {
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
            }*/
        }

        void operator()(const NodeStmtCall& stmt_call) const {
            const std::string& fn = stmt_call.name.value.value();
            std::vector<NodeExprPtr> arg_values = stmt_call.args;
			      call_func(fn, arg_values, gen, stmt_call.line, false);
        }

        void operator()(const NodeStmtProperty& stmt_property) const {
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

		}

		void operator()(const NodeStmtGlobl& stmt_globl) const {
			/*const std::string& name = stmt_globl.ident.value.value();

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


			gen->write("  mov %rax, " + name + "(%rip)");*/
		}

        void operator()(const NodeStmtHeader& stmt_header) const {
            gen->is_header = true;
        }
        void operator()(const NodeStmtUhead& stmt_uhead) const {
            /*const std::string& name = stmt_uhead.path.value.value();
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
            for (const auto& fnc_custom_ret : generator.m_fnc_custom_ret) {
              gen->m_fnc_custom_ret.insert(fnc_custom_ret);
            }*/
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

std::string Generator::gen_prog() {
	declared_funcs.clear();
	m_output << ".extern malloc\n"; // Necessary to many things

    //m_output << "main:\n";
    for (const NodeStmt& stmt : m_prog.stmts) {
        gen_stmt(stmt); 
    }

    return "";
}
