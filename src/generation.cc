#include "generation.hh"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <variant>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include "error.hh"
#include "global.hh"
#include "lexer.hh"
#include "parser.hh"

namespace fs = std::filesystem;

int main_func_declared = false;

std::vector<std::string> modules;

std::vector<std::string> m_mod;
std::unordered_map<std::string, std::string> m_fnc_mod;
std::unordered_map<std::string, Type> m_fnc_rets;

std::set<std::string> m_preprocessor_bool;
std::unordered_map<std::string, std::vector<Token>> m_preprocessor;

llvm::LLVMContext TheContext;
std::unique_ptr<llvm::Module> TheModule =
    std::make_unique<llvm::Module>("MainModule", TheContext);

struct Func {
  std::string name;
  std::vector<ArgType> args;
};

std::unordered_map<std::string, std::function<void(const NodeStmtCall &, Generator *)>>
    function_handlers;
std::unordered_map<std::string, std::function<void(const NodeExprCall &, Generator *)>>
    function_ret_handlers;
// std::vector<Func> decfuncs;

std::unordered_map<std::string,
                   std::function<void(const NodeExprProperty &, Generator *, int)>>
    str_ret_property;
std::unordered_map<std::string,
                   std::function<void(const NodeStmtProperty &, Generator *, int)>>
    str_property;

static bool is_int(const NodeExpr &expr) {
  return std::holds_alternative<NodeExprIntLit>(expr.var);
}
static bool is_str(const NodeExpr &expr) {
  return std::holds_alternative<NodeExprStrLit>(expr.var);
}
static bool is_float(const NodeExpr &expr) {
  return std::holds_alternative<NodeExprFloatLit>(expr.var);
}
static bool is_ident(const NodeExpr &expr) {
  return std::holds_alternative<NodeExprIdent>(expr.var);
}
static bool is_call(const NodeExpr &expr) {
  return std::holds_alternative<NodeExprCall>(expr.var);
}

struct TypeMapping {
  llvm::Type *type;
  llvm::Type *base_type;
};

TypeMapping map_type_to_llvm(const Type &t, Generator *gen, bool is_ref = false,
                             bool none_as_void = false) {
  using Kind = Type::Kind;
  TypeMapping type;

  switch (t.kind) {
  case Kind::Int:
    type.base_type = llvm::Type::getInt32Ty(TheContext);
    if (t.pointee != nullptr) {
      auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
      type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
      type.base_type = pointee_mapping.type;
    } else {
      type.type = type.base_type;
    }
    break;
  case Kind::Float:
    type.base_type = llvm::Type::getDoubleTy(TheContext);
    if (t.pointee != nullptr) {
      auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
      type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
      type.base_type = pointee_mapping.type;
    } else
      type.type = type.base_type;

    break;
  case Kind::Str:
    type.base_type = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext));
    if (t.pointee != nullptr) {
      auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
      type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
      type.base_type = pointee_mapping.type;
    } else
      type.type = type.base_type;
    break;
  case Kind::Char:
    type.base_type = llvm::Type::getInt8Ty(TheContext);
    if (t.pointee != nullptr) {
      auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
      type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
      type.base_type = pointee_mapping.type;
    } else
      type.type = type.base_type;
    break;
  case Kind::None:
    if (none_as_void) {
      type.type = llvm::Type::getVoidTy(TheContext);
      type.base_type = nullptr;
    } else {
      type.type = llvm::Type::getInt8Ty(TheContext); // i32eturn nullptr;
      type.base_type = llvm::Type::getInt8Ty(TheContext);
    }
    break;
  case Kind::List: // OBSOLETE AND UNSAFE
    type.type = llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(TheContext));
    break;
  case Kind::Struct: // OBSOLETE AND UNSAFE
    /*type.type = llvm::StructType::create(TheContext, t.user_type);
    break;*/
  case Kind::UserDefined:
    if (gen->m_struct_templates.contains(t.user_type)) {
      type.base_type = gen->m_struct_templates.at(t.user_type);
      if (t.pointee != nullptr) {
        auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
        type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
        type.base_type = pointee_mapping.type;
      } else
        type.type = type.base_type;
    }
    break;

  case Kind::Ptr: { // NOT RECOMMENDED TO USE. DELETE LATER PLS
    if (!t.pointee) {
      type.base_type = llvm::Type::getInt8Ty(TheContext);
      type.type = llvm::PointerType::getUnqual(type.base_type);
      break;
    }

    auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);

    type.base_type = pointee_mapping.type;
    type.type = llvm::PointerType::getUnqual(type.base_type);
    break;
  }
  case Kind::Any:
    type.base_type = llvm::Type::getInt8Ty(TheContext);
    if (t.pointee != nullptr) {
      auto pointee_mapping = map_type_to_llvm(*t.pointee, gen);
      type.type = llvm::PointerType::getUnqual(pointee_mapping.type);
      type.base_type = pointee_mapping.type;
    } else
      type.type = type.base_type;
    break;

  default:
    llvm::errs() << "Error: cannot map type kind " << (int)t.kind << " to LLVM\n";
    type.type = nullptr;
    type.base_type = nullptr;
    break;
  }

  return type;
}

static llvm::Type *get_base_type_from_opaque(llvm::Type *t) {
  // if (!t->isPointerTy()) return t;

  if (t->isIntegerTy())
    return llvm::Type::getInt32Ty(TheContext);

  return t;
}

static llvm::Type *get_type(const NodeExpr &e, Generator *gen,
                            bool get_base_type = true) {
  if (std::holds_alternative<NodeExprIdent>(e.var)) {
    NodeExprIdent ident = std::get<NodeExprIdent>(e.var);
    const std::string &name = ident.ident.value.value();
    if (gen->m_vars.contains(name)) {
      if (get_base_type)
        return gen->m_vars.at(name).base_type;
      else
        return gen->m_vars.at(name).type;
    }
    return nullptr;
  } else if (std::holds_alternative<NodeExprDeref>(e.var)) {
    NodeExprDeref deref = std::get<NodeExprDeref>(e.var);
    if (!deref.expr)
      return nullptr;
    llvm::Value *val = gen->gen_expr(*deref.expr);
    return val->getType();
  } else if (std::holds_alternative<NodeExprProperty>(e.var)) {
    NodeExprProperty expr_property = std::get<NodeExprProperty>(e.var);
    llvm::Value *base = gen->gen_expr(*expr_property.base, false, false);
    llvm::Value *base2 = gen->gen_expr(*expr_property.base, false, true);

    if (!base->getType()->isStructTy()) {
      add_error("Expected struct as base", expr_property.line);
      return nullptr;
    }

    std::map<std::string, std::pair<int, llvm::Type *>> struct_type =
        gen->m_struct_arg_templates.at(base->getType()->getStructName().str());
    if (struct_type.contains(expr_property.property.value.value())) {
      llvm::Type *opaque_type =
          struct_type.at(expr_property.property.value.value()).second;
      if (get_base_type)
        return get_base_type_from_opaque(opaque_type);
      return opaque_type;
    }
  }

  return nullptr;
}

bool is_string_literal(llvm::Value *val) {
  if (auto *gv = llvm::dyn_cast<llvm::GlobalVariable>(val)) {
    if (gv->hasInitializer()) {
      auto *init = gv->getInitializer();
      if (auto *cdata = llvm::dyn_cast<llvm::ConstantDataArray>(init)) {
        return cdata->isString();
      }
    }
  }
  return false;
}

static Type replace_user_defined_type_with_generic(Type type, Generator *gen) {
  if (type.kind != Type::Kind::UserDefined)
    return type;

  const std::string &name = type.user_type;

  return type;
}

static llvm::Type *get_var_type(const std::string &name, Generator *gen) {
  if (gen->m_vars.contains(name)) {
    return gen->m_vars.at(name).type;
  } else if (gen->current_mode == Mode::Function) {
    if (!gen->m_fnc_args.contains(gen->current_func)) {
    }
    for (const auto &arg : gen->m_fnc_args.at(gen->current_func)) {
      if (arg.name == name) {
        return arg.type;
      }
    }
  }
}

static void set_value(Generator *gen, llvm::Value *target, llvm::Value *value) {}

static std::optional<llvm::Value *> call_func(const std::string &fn,
                                              std::vector<NodeExprPtr> arg_values,
                                              Generator *gen, int line,
                                              bool direct_call = false) {
  std::vector<llvm::Value *> values;

  for (const auto &arg_val : arg_values) {
    values.push_back(gen->gen_expr(*arg_val, false));
  }

  llvm::Function *func = gen->ModModule->getFunction(fn);

  if (!gen->declared_funcs.contains(fn) || func == nullptr) {
    add_error("Inexistent function (" + fn + ")", line);
    return std::nullopt;
  }

  if (!func->getReturnType()->isVoidTy()) {
    llvm::Value *call = gen->Builder.CreateCall(func, values, fn + "_ret");
    return call;
  } else {
    llvm::Value *call = gen->Builder.CreateCall(func, values);
  }

  return std::nullopt;
}

llvm::Value *Generator::gen_expr(const NodeExpr &expr, bool as_lvalue,
                                 bool get_pointer) {
  struct ExprVisitor {
    Generator *gen;
    bool as_lvalue;
    bool get_pointer;

    llvm::Value *operator()(const NodeExprIntLit &expr_int_lit) {
      return llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(TheContext),
        std::stoi(expr_int_lit.int_lit.value.value_or("0")));
    }

    llvm::Value *operator()(const NodeExprBinary &expr_bin) const {
      llvm::Value *lhs_val = gen->gen_expr(*expr_bin.lhs);
      llvm::Value *rhs_val = gen->gen_expr(*expr_bin.rhs);

      llvm::Type *v1 = lhs_val->getType();
      llvm::Type *v2 = rhs_val->getType();

      if (v1->isDoubleTy() || v2->isDoubleTy()) {
        if (!v1->isDoubleTy()) {
          lhs_val = gen->Builder.CreateSIToFP(
              lhs_val, llvm::Type::getDoubleTy(TheContext), "f_lhs");
          v1 = lhs_val->getType();
        }
        if (!v2->isDoubleTy()) {
          rhs_val = gen->Builder.CreateSIToFP(
              rhs_val, llvm::Type::getDoubleTy(TheContext), "f_rhs");
          v2 = rhs_val->getType();
        }
      }

      llvm::Value *result;
      switch (expr_bin.op_token.type) {
      case TokenType::plus:
        if (v1->isDoubleTy() || v2->isDoubleTy())
          result = gen->Builder.CreateFAdd(lhs_val, rhs_val, "faddtmp");
        else if (is_string_literal(lhs_val) && is_string_literal(rhs_val)) {

          std::vector<llvm::Type *> param_types = {
              llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext)),
              llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext))};

          llvm::Function *func = gen->ModModule->getFunction("strcat");
          if (func) {
            result = gen->Builder.CreateCall(func, {lhs_val, rhs_val}, "strcattmp");
            break;
          }

          llvm::FunctionType *func_type = llvm::FunctionType::get(
              llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext)),
              param_types, false);

          llvm::Function *strcat_func =
              llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                     "strcat", gen->ModModule.get());
          result = gen->Builder.CreateCall(strcat_func, {lhs_val, rhs_val}, "strcat");
        } else
          result = gen->Builder.CreateAdd(lhs_val, rhs_val, "add");
        break;
      case TokenType::minus:
        if (v1->isDoubleTy() || v2->isDoubleTy())
          result = gen->Builder.CreateFSub(lhs_val, rhs_val, "fsub");
        else
          result = gen->Builder.CreateSub(lhs_val, rhs_val, "sub");
        break;
      case TokenType::star:
        if (v1->isDoubleTy() || v2->isDoubleTy())
          result = gen->Builder.CreateFMul(lhs_val, rhs_val, "fmul");
        else
          result = gen->Builder.CreateMul(lhs_val, rhs_val, "mul");
        break;
      case TokenType::slash:
        if (v1->isDoubleTy() || v2->isDoubleTy())
          result = gen->Builder.CreateFDiv(lhs_val, rhs_val, "fdiv");
        else
          result = gen->Builder.CreateSDiv(lhs_val, rhs_val, "div");
        break;
      case TokenType::percent:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          llvm::Value *div = gen->Builder.CreateFDiv(lhs_val, rhs_val, "div");
          llvm::Value *trunc =
              gen->Builder.CreateFPToSI(div, llvm::Type::getInt32Ty(TheContext));
          llvm::Value *mul = gen->Builder.CreateFMul(
              rhs_val, gen->Builder.CreateSIToFP(trunc, v1, "mul"));
          result = gen->Builder.CreateFSub(lhs_val, mul, "fmod");
        } else
          result = gen->Builder.CreateSRem(lhs_val, rhs_val, "mod");
        break;
      case TokenType::eq_eq:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpUEQ(lhs_val, rhs_val, "ueq");
        } else {
          result = gen->Builder.CreateICmpEQ(lhs_val, rhs_val, "seq");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "cmp");
        break;
      case TokenType::bang_eq:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpUNE(lhs_val, rhs_val, "une");
        } else {
          result = gen->Builder.CreateICmpNE(lhs_val, rhs_val, "ine");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "neq");
        break;
      case TokenType::l_arrow:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpULT(lhs_val, rhs_val, "ult");
        } else {
          result = gen->Builder.CreateICmpSLT(lhs_val, rhs_val, "slt");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "lt");
        break;
      case TokenType::lte:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpULE(lhs_val, rhs_val, "ule");
        } else {
          result = gen->Builder.CreateICmpSLE(lhs_val, rhs_val, "sle");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "le");
        break;
      case TokenType::r_arrow:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpUGT(lhs_val, rhs_val, "fgt");
        } else {
          result = gen->Builder.CreateICmpSGT(lhs_val, rhs_val, "sgt");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "gt");
        break;
      case TokenType::gte:
        if (v1->isDoubleTy() || v2->isDoubleTy()) {
          result = gen->Builder.CreateFCmpUGE(lhs_val, rhs_val, "fge");
        } else {
          result = gen->Builder.CreateICmpSGE(lhs_val, rhs_val, "sge");
        }
        result =
            gen->Builder.CreateZExt(result, llvm::Type::getInt32Ty(TheContext), "ge");
        break;
      case TokenType::_and:
      case TokenType::_or: {

        llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *entry_block = gen->Builder.GetInsertBlock();
        llvm::BasicBlock *rhs_block =
            llvm::BasicBlock::Create(TheContext, "rhs", function);
        llvm::BasicBlock *end_block =
            llvm::BasicBlock::Create(TheContext, "end", function);

        llvm::Value *lhs_bool = gen->Builder.CreateICmpNE(
            lhs_val, llvm::ConstantInt::get(lhs_val->getType(), 0), "and_or_lhs");
        if (expr_bin.op_token.type == TokenType::_and)
          gen->Builder.CreateCondBr(lhs_bool, rhs_block, end_block);
        else
          gen->Builder.CreateCondBr(lhs_bool, end_block, rhs_block);

        gen->Builder.SetInsertPoint(rhs_block);
        llvm::Value *rhs_bool = gen->Builder.CreateZExt(
            rhs_val, llvm::Type::getInt32Ty(TheContext), "and_or_rhs");
        gen->Builder.CreateBr(end_block);

        gen->Builder.SetInsertPoint(end_block);
        llvm::PHINode *phi =
            gen->Builder.CreatePHI(llvm::Type::getInt32Ty(TheContext), 2, "phi");
        if (expr_bin.op_token.type == TokenType::_and) {
          phi->addIncoming(
              llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0),
              entry_block);
          phi->addIncoming(rhs_bool, rhs_block);
        } else {
          phi->addIncoming(
              llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1),
              entry_block);
          phi->addIncoming(rhs_bool, rhs_block);
        }
        result = phi;
        break;
      }
      default:
        add_error("Binary Operator (" + expr_bin.op_token.value.value_or("NULL_TOK") +
                      ") not supported",
                  expr_bin.line);
        break;
      }

      return result;
    }

    llvm::Value *operator()(const NodeExprUnary &expr_unary) const {
      llvm::Value *val = gen->gen_expr(*expr_unary.expr);

      if (expr_unary.op.type == TokenType::bang) {
        llvm::Value *zero =
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
        llvm::Value *cmp = gen->Builder.CreateICmpEQ(val, zero, "cmp_zero");
        return gen->Builder.CreateZExt(cmp, llvm::Type::getInt32Ty(TheContext),
                                       "bang_cmp");
      } else if (expr_unary.op.type == TokenType::minus) {
        if (val->getType()->isDoubleTy()) {
          llvm::Value *zero_double =
              llvm::ConstantFP::get(llvm::Type::getDoubleTy(TheContext), .0);
          return gen->Builder.CreateFSub(zero_double, val, "fneg");
        } else {
          llvm::Value *zero =
              llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
          return gen->Builder.CreateSub(zero, val, "neg");
        }
      }
    }

    llvm::Value *operator()(const NodeExprBinaryAssign &expr_bin_assign) const {
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

      gen->write("  mov %" + reg + ", " + std::to_string(offset_bytes) +
      "(%rsp)");
      //if (push_result) gen->push("rax");*/
    }
    llvm::Value *operator()(const NodeExprUnaryIncDec &expr_unary_operator) const {};

    llvm::Value *operator()(const NodeExprIdent &expr_ident) const {
      const std::string &name = expr_ident.ident.value.value();

      if (m_preprocessor.contains(name)) {
        std::vector<Token> splited_value = m_preprocessor.at(name);
        Parser parser(std::move(splited_value));
        NodeExpr expr = *parser.parse_expr();

        llvm::Value *val = gen->gen_expr(expr);
        return val;
      }

      if (gen->declared_funcs.contains(name)) { // Ident can also be a call function with no args and with no parenthesis
        std::optional<llvm::Value *> ret_val =
            call_func(name, {}, gen, expr_ident.line, false);
        if (!ret_val.has_value()) {
        }
        return *ret_val;
      }

      if (gen->m_vars.contains(name)) {
        if (gen->m_vars.at(name).is_globl) {
          llvm::GlobalVariable *var = gen->ModModule->getGlobalVariable(name);
          return gen->Builder.CreateLoad(var->getValueType(), var, name);
        } else {
          llvm::Value *var = gen->m_vars.at(name).var_ptr;
          if (get_pointer)
            return var;
          return gen->Builder.CreateLoad(gen->m_vars.at(name).type, var, name);
        }
      }

      return nullptr;
    }

    llvm::Value *operator()(const NodeExprStrLit &expr_str_lit) const {
      std::string value = escape_string(expr_str_lit.str_lit.value.value());
      llvm::Constant *str_constant =
          llvm::ConstantDataArray::getString(TheContext, value, true);

      llvm::GlobalVariable *g_var_str = new llvm::GlobalVariable(
          *gen->ModModule, str_constant->getType(), true,
          llvm::GlobalValue::PrivateLinkage, str_constant, ".str");
      g_var_str->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
      g_var_str->setAlignment(llvm::Align(1));

      llvm::Constant *zero =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
      llvm::Constant *indices[] = {zero, zero};

      llvm::Constant *str_ptr = llvm::ConstantExpr::getGetElementPtr(
          str_constant->getType(), g_var_str, indices);

      return str_ptr;
    }

    llvm::Value *operator()(const NodeExprCharLit &expr_char_lit) const {
      char value = escape_string(expr_char_lit.char_lit.value.value())[0];
      return llvm::ConstantInt::get(llvm::Type::getInt8Ty(TheContext), value);
    }

    llvm::Value *operator()(const NodeExprFloatLit &expr_float_lit) const {
      return llvm::ConstantFP::get(llvm::Type::getDoubleTy(TheContext),
                                   std::stof(expr_float_lit.float_lit.value.value()));
    }

    llvm::Value *operator()(const NodeExprNone &expr_none) const {
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
    }

    llvm::Value *operator()(const NodeExprNullptr &expr_nullptr) const {
      return llvm::ConstantPointerNull::get(
          llvm::PointerType::get(gen->Builder.getInt8Ty(), 0));
    }

    llvm::Value *operator()(const NodeExprNoArg &expr_no_arg) const {}
    llvm::Value *operator()(const NodeExprCR &expr_cr) const {}

    llvm::Value *operator()(const NodeExprBoolValue &expr_bool) const {
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext),
                                    expr_bool.value);
    }

    llvm::Value *operator()(const NodeExprCall &expr_call) const {
      const std::string &fn = expr_call.name.value.value();
      std::vector<NodeExprPtr> arg_values = expr_call.args;
      std::optional<llvm::Value *> ret_val =
          call_func(fn, arg_values, gen, expr_call.line, false);
      if (!ret_val.has_value()) {
      }
      return *ret_val;
    }

    llvm::Value *operator()(const NodeExprProperty &expr_property) const {
      llvm::Value *base = gen->gen_expr(*expr_property.base, false, false);
      llvm::Value *base2 = gen->gen_expr(*expr_property.base, false, true);

      if (!base->getType()->isStructTy()) {
        add_error("Expected struct as base", expr_property.line);
        return nullptr;
      }

      llvm::StructType *struct_type =
          gen->m_struct_templates.at(base->getType()->getStructName().str());
      llvm::Type *ptr_type = struct_type->getPointerTo();

      llvm::Value *base_ptr = nullptr;
      if (!base2->getType()->isPointerTy()) {
        base_ptr = gen->Builder.CreateAlloca(base2->getType(), nullptr, "tmp_struct");
        gen->Builder.CreateStore(base2, base_ptr);
      } else {
        base_ptr = base2;
      }

      int index = gen->m_struct_arg_templates.at(base->getType()->getStructName().str())
                      .at(expr_property.property.value.value())
                      .first;
      llvm::Type *type =
          gen->m_struct_arg_templates.at(base->getType()->getStructName().str())
              .at(expr_property.property.value.value())
              .second;
      llvm::Value *property = gen->Builder.CreateStructGEP(
          struct_type, base_ptr, index, expr_property.property.value.value());
      if (!as_lvalue && !property->getType()->isStructTy()) {
        return gen->Builder.CreateLoad(type, property, "deref");
      }

      return property;
    }

    llvm::Value *operator()(const NodeExprGetPtr &expr_ptr) const {
      const std::string &name = expr_ptr.ident.value.value();

      if (gen->m_vars.contains(name)) {
        if (gen->m_vars.at(name).is_globl) {
          llvm::GlobalVariable *var = gen->ModModule->getGlobalVariable(name);
          return var;
        } else {
          llvm::Value *var = gen->m_vars.at(name).var_ptr;
          return var;
        }
      }

      return nullptr;
    }

    llvm::Value *operator()(const NodeExprDeref &expr_ref) const {
      llvm::Value *ptr_val = gen->gen_expr(*expr_ref.expr);

      if (!ptr_val)
        return nullptr;

      if (!ptr_val->getType()->isPointerTy()) {
        add_error("Error: dereferencing non-pointer value");
        return nullptr;
      }

      llvm::Type *base_type = get_type(*expr_ref.expr, gen, true);

      if (as_lvalue) {
        return ptr_val;
      } else {
        return gen->Builder.CreateLoad(base_type, ptr_val, "deref");
      }
    }

    llvm::Value *operator()(const NodeExprList &expr_list) const {}

    llvm::Value *operator()(const NodeExprListElement &expr_list_element) const {
      llvm::Value *index = gen->gen_expr(*expr_list_element.index);
      llvm::Value *list_val = gen->gen_expr(*expr_list_element.list_expr);

      llvm::Type *base_type = get_type(*expr_list_element.list_expr, gen, true);
      base_type = get_base_type_from_opaque(base_type);

      llvm::Value *element_ptr =
        gen->Builder.CreateInBoundsGEP(base_type, list_val, index, "element_ptr");

      if (as_lvalue)
        return element_ptr;
      return gen->Builder.CreateLoad(base_type, element_ptr, "loadelem");
    }

    llvm::Value *operator()(const NodeExprStruct &expr_struct) const {}

    llvm::Value *operator()(const NodeExprNew &stmt_new) const {
      return llvm::ConstantInt::get(llvm::PointerType::getInt8Ty(TheContext), 0);
    }

    llvm::Value *operator()(const NodeExprIsDef &expr_is_def) const {
      if (m_preprocessor_bool.contains(expr_is_def.name.value.value()))
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1);
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
    }
    llvm::Value *operator()(const NodeExprIsNotDef &expr_is_ndef) const {
      if (m_preprocessor_bool.contains(expr_is_ndef.name.value.value()))
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1);
    }

    llvm::Value *operator()(const NodeExprSizeOf &expr_sizeof) const {
      llvm::Type *type = map_type_to_llvm(expr_sizeof.type, gen).type;

      if (type != nullptr) {
        const llvm::DataLayout &DL = gen->ModModule->getDataLayout();
        uint64_t size_in_bytes = DL.getTypeAllocSize(type);

        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext),
                                      size_in_bytes);
      }
    }

    llvm::Value *operator()(const NodeExprCast &expr_cast) const {
      llvm::Value *val = gen->gen_expr(*expr_cast.expr);
      TypeMapping target = map_type_to_llvm(expr_cast.target_type, gen, true);

      /*if (!val->getType()->isPointerTy()) {
        add_error("Bitcast only works on pointers", expr_cast.line);
      }*/

      return gen->Builder.CreateBitCast(val, target.type);
    }

    llvm::Value *operator()(const NodeExprFact &expr_fact) const {
      llvm::Value *val = gen->gen_expr(*expr_fact.expr);
      if (val == nullptr) return nullptr;

      std::vector<llvm::Type *> param_types = {
      llvm::Type::getInt32Ty(TheContext),
      llvm::Type::getInt32Ty(TheContext)};
      
      llvm::Value *result;

      llvm::Function *func = gen->ModModule->getFunction("fact");
      if (func) {
        result = gen->Builder.CreateCall(func, {val}, "facttemp");
        return result;
      }

      llvm::FunctionType *func_type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(TheContext),
        param_types, false);

      llvm::Function *strcat_func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
          "fact", gen->ModModule.get());
      result = gen->Builder.CreateCall(strcat_func, {val}, "fact");

      return result;
    }
  };

  ExprVisitor visitor{.gen = this, .as_lvalue = as_lvalue, .get_pointer = get_pointer};
  llvm::Value *value = std::visit(visitor, expr.var);

  return value;
}

void Generator::gen_stmt(const NodeStmt &stmt) {
  struct StmtVisitor {
    Generator *gen;

    void operator()(const NodeStmtAsmUserWrite &stmt_asm) const {
    }

    void operator()(const NodeStmtUse &stmt_import) const {
      const std::string &name = stmt_import.mod_name.value.value();
      const std::string &path = "/usr/include/ysinclude/" + name + ".ys";
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

      Lexer lexer(std::move(source_code));
      std::vector<Token> tokens = lexer.tokenize();

      Parser parser(std::move(tokens));
      std::optional<NodeProg> program = parser.parse_prog();

      if (!program.has_value()) {
        std::cerr << "Parsing failed in file: " << name << "\n";
      }

      std::string base_name = fs::path(path).stem().string();
      // std::string gen_name = (index == 0) ? "main" : base_name;
      std::string gen_name = base_name;

      auto temp_module = std::make_unique<llvm::Module>("temp", TheContext);
      Generator generator(program.value(), name,
                          std::move(temp_module)); // temporal module
      generator.gen_prog();

      llvm::SmallVector<llvm::ReturnInst *, 8> Returns;

      for (auto &fn_name : stmt_import.to_import) {
        if (auto *f = generator.ModModule->getFunction(fn_name)) {
          llvm::ValueToValueMapTy VMap;
          llvm::Function *new_f = llvm::Function::Create(
              f->getFunctionType(), llvm::GlobalValue::ExternalLinkage, f->getName(),
              gen->ModModule.get() // destination module
          );

          auto new_arg_it = new_f->arg_begin();
          for (auto &Arg : f->args()) {
            VMap[&Arg] = &*new_arg_it;
            ++new_arg_it;
          }

          llvm::CloneFunctionInto(
              new_f, f, VMap, llvm::CloneFunctionChangeType::LocalChangesOnly, Returns);
          new_f->copyAttributesFrom(f);
        }
      }

      llvm::ValueToValueMapTy VMap;
      for (auto &var_name : stmt_import.to_import) {
        if (auto *g = generator.ModModule->getGlobalVariable(var_name)) {

          llvm::GlobalVariable *new_g = new llvm::GlobalVariable(
              *gen->ModModule, g->getValueType(), g->isConstant(), g->getLinkage(),
              nullptr, g->getName());

          VMap[g] = new_g;

          if (g->hasInitializer()) {
            llvm::Constant *new_init = llvm::MapValue(g->getInitializer(), VMap);
            new_g->setInitializer(new_init);
          }

          new_g->setAlignment(g->getAlign());
          new_g->setThreadLocalMode(g->getThreadLocalMode());
          new_g->setExternallyInitialized(g->isExternallyInitialized());
        }
      }

      std::vector<std::string> to_import = stmt_import.to_import;
      for (const auto &glob_var : generator.m_vars) {
        if (std::find(to_import.begin(), to_import.end(), glob_var.first) ==
            to_import.end())
          continue;
        auto modified_var = glob_var;
        // modified_var.second.is_declared = false;
        gen->m_vars.insert(modified_var);
      }
      for (const auto &declared_func : generator.declared_funcs) {
        if (std::find(to_import.begin(), to_import.end(), declared_func.first) ==
            to_import.end())
          continue;
        gen->declared_funcs.insert(declared_func);
      }
      for (const auto &fnc_custom_ret : generator.m_fnc_custom_ret) {
        if (std::find(to_import.begin(), to_import.end(), fnc_custom_ret.first) ==
            to_import.end())
          continue;
        gen->m_fnc_custom_ret.insert(fnc_custom_ret);
      }
      for (const auto &struct_template : generator.m_struct_templates) {
        if (std::find(to_import.begin(), to_import.end(), struct_template.first) ==
            to_import.end())
          continue;
        gen->m_struct_templates.insert(struct_template);
      }
      for (const auto &struct_arg_template : generator.m_struct_arg_templates) {
        if (std::find(to_import.begin(), to_import.end(), struct_arg_template.first) ==
            to_import.end())
          continue;
        gen->m_struct_arg_templates.insert(struct_arg_template);
      }
    }
    void operator()(const NodeStmtImport &stmt_import) const {
      const std::string &name = stmt_import.mod_name.value.value();
      const std::string &path = "/usr/include/ysinclude/" + name + ".ys";
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

      Lexer lexer(std::move(source_code));
      std::vector<Token> tokens = lexer.tokenize();

      Parser parser(std::move(tokens));
      std::optional<NodeProg> program = parser.parse_prog();

      if (!program.has_value()) {
        std::cerr << "Parsing failed in file: " << name << "\n";
      }

      std::string base_name = fs::path(path).stem().string();
      std::string gen_name = base_name;

      auto temp_module = std::make_unique<llvm::Module>("temp", TheContext);
      Generator generator(program.value(), name,
                          std::move(temp_module)); // temporal module
      generator.gen_prog();

      llvm::SmallVector<llvm::ReturnInst *, 8> Returns;

      for (auto &f : generator.ModModule->functions()) {
        llvm::ValueToValueMapTy VMap;
        llvm::Function *new_f = llvm::Function::Create(
          f.getFunctionType(), llvm::GlobalValue::ExternalLinkage, f.getName(),
          gen->ModModule.get()
        );

        auto new_arg_it = new_f->arg_begin();
        for (auto &Arg : f.args()) {
          VMap[&Arg] = &*new_arg_it;
          ++new_arg_it;
        }

        llvm::CloneFunctionInto(
            new_f, &f, VMap, llvm::CloneFunctionChangeType::LocalChangesOnly, Returns);
        new_f->copyAttributesFrom(&f);
      }

      /*llvm::ValueToValueMapTy VMap;
      for (auto &g : generator.ModModule->globals()) {
        llvm::GlobalVariable *new_g = new llvm::GlobalVariable(
            *gen->ModModule, g.getValueType(), g.isConstant(), g.getLinkage(),
            nullptr, g.getName());

        VMap[&g] = new_g;

        if (g.hasInitializer()) {
          llvm::Constant *new_init = llvm::MapValue(g.getInitializer(), VMap);
          new_g->setInitializer(new_init);
        }

        new_g->setAlignment(g.getAlign());
        new_g->setThreadLocalMode(g.getThreadLocalMode());
        new_g->setExternallyInitialized(g.isExternallyInitialized());
      }

      for (const auto &glob_var : generator.m_vars) {
        gen->m_vars.insert(glob_var);
      }*/

      for (const auto &declared_func : generator.declared_funcs) {
        gen->declared_funcs.insert(declared_func);
      }

      for (const auto &fnc_custom_ret : generator.m_fnc_custom_ret) {
        gen->m_fnc_custom_ret.insert(fnc_custom_ret);
      }

      for (const auto &struct_template : generator.m_struct_templates) {
        gen->m_struct_templates.insert(struct_template);
      }

      for (const auto &struct_arg_template : generator.m_struct_arg_templates) {
        gen->m_struct_arg_templates.insert(struct_arg_template);
      }
    }

    void operator()(const NodeStmtMkpub mkpub) const {
      if (gen->current_mode == Mode::Function) {
        add_error("Making public a function inside of another function is not allowed",
                  mkpub.line);
      }
      for (const auto &func : mkpub.functions) {
        bool func_exists = false;
        for (const auto &mapped_func : gen->m_fnc_args) {
          if (mapped_func.first == func.value.value()) {
            func_exists = true;
          }
        }
        if (!func_exists) {
          /*for (const auto& e : gen->m_fnc_args) {
              std::cerr << e.first << "\n";
          }*/
          add_error("Making public an inexistent function (" + func.value.value() + ")",
                    mkpub.line);
        }
        gen->write("  .globl " + func.value.value());
        gen->write("  .type " + func.value.value() + ", @function");
        if (func.value.value() == "main")
          continue;
        gen->m_output << ".globl " << func.value.value() << "\n";
        gen->m_output << ".type " << func.value.value() << ", " << "@function" << "\n";
      }
    }

    void operator()(const NodeStmtVar &stmt_var) const {
      TypeMapping type_mapping =
          map_type_to_llvm(stmt_var.type, gen, stmt_var.type.is_ref);
      llvm::Type *base_type = type_mapping.base_type;
      llvm::Type *llvm_type = type_mapping.type;
      const std::string &name = stmt_var.ident.value.value();

      llvm::Value *var_ptr = nullptr;
      llvm::Value *init_val =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
      if (stmt_var.has_initial_value) {
        init_val = gen->gen_expr(stmt_var.expr);
      }

      llvm::Constant *initializer = nullptr;

      if (init_val) {
        // Solo usar Constant si realmente es un literal
        if (llvm::isa<llvm::Constant>(init_val)) {
          initializer = llvm::cast<llvm::Constant>(init_val);
        } else {
          if (llvm_type->isPointerTy()) {
            initializer = llvm::ConstantPointerNull::get(
                llvm::cast<llvm::PointerType>(llvm_type));
          } else if (llvm_type->isIntegerTy()) {
            initializer = llvm::ConstantInt::get(llvm_type, 0);
          } else if (llvm_type->isFloatingPointTy()) {
            initializer = llvm::ConstantFP::get(llvm_type, 0.0);
          } else if (llvm_type->isStructTy() &&
                     !llvm::cast<llvm::StructType>(llvm_type)->isOpaque()) {
            initializer = llvm::Constant::getNullValue(llvm_type);
          } else {
            initializer = nullptr;
            add_error("Cannot create null initializer for this type", stmt_var.line);
          }
        }
      } else {
        if (llvm_type->isPointerTy()) {
          initializer =
              llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(llvm_type));
        } else if (llvm_type->isIntegerTy()) {
          initializer = llvm::ConstantInt::get(llvm_type, 0);
        } else if (llvm_type->isFloatingPointTy()) {
          initializer = llvm::ConstantFP::get(llvm_type, 0.0);
        } else if (llvm_type->isStructTy()) {
          initializer = llvm::Constant::getNullValue(llvm_type);
        } else {
          initializer = nullptr;
        }
      }

      if (gen->current_mode == Mode::Global) {
        llvm::Constant *global_init_val;
        if (global_init_val = dyn_cast<llvm::Constant>(init_val)) {
        } else {
          add_error("Initial value of a global variables must be a constant",
                    stmt_var.line);
        }

        var_ptr =
            new llvm::GlobalVariable(*gen->ModModule, llvm_type, !stmt_var.is_mutable,
                                     llvm::GlobalValue::ExternalLinkage,
                                     global_init_val, stmt_var.ident.value.value());
      } else {
        var_ptr = gen->Builder.CreateAlloca(llvm_type, nullptr, name);

        if (init_val) {
          llvm::Value *store_ptr = var_ptr;

          if (llvm_type->isPointerTy() && stmt_var.has_initial_value &&
              init_val->getType() != llvm_type) {
            store_ptr = gen->Builder.CreateBitCast(var_ptr, llvm_type->getPointerTo());
          }

          gen->Builder.CreateStore(init_val, store_ptr);
        }
      }

      Var var = gen->insert_var(stmt_var.ident.value.value(), nullptr, llvm_type,
                                base_type, var_ptr, stmt_var.is_mutable,
                                gen->current_mode == Mode::Global);
    }

    void operator()(const NodeStmtAssign &stmt) const { // -> value, target, op_token
      llvm::Value *target_ptr = nullptr;

      if (std::holds_alternative<NodeExprIdent>(stmt.target.var)) {
        NodeExprIdent expr_ident = std::get<NodeExprIdent>(stmt.target.var);
        const std::string &name = expr_ident.ident.value.value();
        if (!gen->m_vars.contains(name)) {
          add_error("Variable (" + name + ") not declared", stmt.line);
          return;
        }

        if (!gen->m_vars.at(name).is_mutable) {
          add_error("Variable (" + name + ") is not mutable", stmt.line);
          return;
        }

        target_ptr = gen->m_vars.at(name).var_ptr;
      } else if (std::holds_alternative<NodeExprListElement>(stmt.target.var)) {
        NodeExprListElement expr_ele = std::get<NodeExprListElement>(stmt.target.var);
        target_ptr = gen->gen_expr(expr_ele, true);
      } else if (std::holds_alternative<NodeExprDeref>(stmt.target.var)) {
        NodeExprDeref expr_deref = std::get<NodeExprDeref>(stmt.target.var);
        target_ptr = gen->gen_expr(expr_deref, true);
      } else if (std::holds_alternative<NodeExprProperty>(stmt.target.var)) {
        NodeExprProperty expr_property = std::get<NodeExprProperty>(stmt.target.var);
        target_ptr = gen->gen_expr(expr_property, true);
      } else {
        throw std::runtime_error(
            "Solo asignaciones a identificadores soportadas por ahora");
      }

      llvm::Value *value = gen->gen_expr(stmt.value);
      if (stmt.op_tok.type == TokenType::plus_eq ||
          stmt.op_tok.type == TokenType::minus_eq ||
          stmt.op_tok.type == TokenType::star_eq ||
          stmt.op_tok.type == TokenType::slash_eq) {
        TokenType tok;
        if (stmt.op_tok.type == TokenType::plus_eq)
          tok = TokenType::plus;
        else if (stmt.op_tok.type == TokenType::minus_eq)
          tok = TokenType::minus;
        else if (stmt.op_tok.type == TokenType::star_eq)
          tok = TokenType::star;
        else
          tok = TokenType::slash;

        NodeExpr expr_bin = NodeExpr(NodeExprBinary{
            .lhs = std::make_shared<NodeExpr>(stmt.target),
            .op_token = Token{.type = tok, .value = "", .line = stmt.line},
            .rhs = std::make_shared<NodeExpr>(stmt.value),
            .line = stmt.line});

        value = gen->gen_expr(expr_bin);
      }
      llvm::Value *store_ptr = target_ptr;
      llvm::Type *type = target_ptr->getType();

      if (type->isPointerTy() && value->getType() != type) {
        store_ptr =
            gen->Builder.CreateBitCast(target_ptr, value->getType()->getPointerTo());
      }

      gen->Builder.CreateStore(value, store_ptr);
    }

    void operator()(const NodeStmtVarRe &stmt_var) const {
      // Obsolete function: now StmtAssign is use for all
    }

    void operator()(const NodeStmtIf &stmt_if) const {
      static int label_counter = 0;
      int var_n = gen->m_vars.size();

      llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();

      int current = label_counter++;
      llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(
          TheContext, "endif_" + std::to_string(current), function);
      llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(
          TheContext, "else_" + std::to_string(current), function);

      llvm::BasicBlock *ifBlock = llvm::BasicBlock::Create(
          TheContext, "if_" + std::to_string(current), function);

      std::vector<llvm::BasicBlock *> elifBlocks;
      for (size_t i = 0; i < stmt_if.elif_conditions.size(); ++i) {
        elifBlocks.push_back(llvm::BasicBlock::Create(
          TheContext, "elif_" + std::to_string(current) + "_" + std::to_string(i),
          function));
      }

      llvm::Value *condVal = gen->gen_expr(stmt_if.condition);
      llvm::Value *condBool = gen->Builder.CreateICmpNE(
          condVal, llvm::ConstantInt::get(condVal->getType(), 0), "if_cond");

      if (!stmt_if.elif_conditions.empty()) {
        gen->Builder.CreateCondBr(condBool, ifBlock, elifBlocks[0]);
      } else {
        gen->Builder.CreateCondBr(condBool, ifBlock, elseBlock);
      }

      gen->Builder.SetInsertPoint(ifBlock);
      for (const auto &stmt : stmt_if.then_branch) {
        gen->gen_stmt(stmt);
      }
      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }
      gen->Builder.CreateBr(endBlock);

      for (size_t i = 0; i < stmt_if.elif_conditions.size(); ++i) {
        gen->Builder.SetInsertPoint(elifBlocks[i]);
        llvm::Value *elifCond = gen->gen_expr(stmt_if.elif_conditions[i]);
        llvm::Value *elifBool = gen->Builder.CreateICmpNE(
            elifCond, llvm::ConstantInt::get(elifCond->getType(), 0), "elif_cond");

        llvm::BasicBlock *nextBlock =
            (i + 1 < stmt_if.elif_conditions.size()) ? elifBlocks[i + 1] : elseBlock;

        gen->Builder.CreateCondBr(elifBool,
          llvm::BasicBlock::Create(TheContext,
            "elif_body_" +
              std::to_string(current) +
              "_" + std::to_string(i),
            function),
          nextBlock);

        gen->Builder.SetInsertPoint(llvm::cast<llvm::BasicBlock>(
            gen->Builder.GetInsertBlock()->getTerminator()->getSuccessor(0)));
        for (const auto &stmt : stmt_if.elif_branches[i]) {
          gen->gen_stmt(stmt);
        }
        while (gen->m_vars.size() > var_n) {
          int i = gen->m_vars_order.size() - 1;
          const auto &var_name = gen->m_vars_order[i];
          gen->m_vars.erase(var_name);
          gen->m_vars_order.pop_back();
        }
        gen->Builder.CreateBr(endBlock);
      }

      gen->Builder.SetInsertPoint(elseBlock);
      for (const auto &stmt : stmt_if.else_branch) {
        gen->gen_stmt(stmt);
      }
      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }
      gen->Builder.CreateBr(endBlock);

      gen->Builder.SetInsertPoint(endBlock);
    }

    void operator()(const NodeStmtWhile &stmt_while) const {
      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock *startBlock = llvm::BasicBlock::Create(
          TheContext, "while_" + std::to_string(current) + "_start", function);
      llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(
          TheContext, "while_" + std::to_string(current) + "_body", function);
      llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(
          TheContext, "while_" + std::to_string(current) + "_end", function);
      gen->stmt_orde.push({startBlock, {endBlock, nullptr}});

      gen->Builder.CreateBr(startBlock);

      gen->Builder.SetInsertPoint(startBlock);
      llvm::Value *condVal = gen->gen_expr(stmt_while.condition);
      llvm::Value *condBool = gen->Builder.CreateICmpNE(
          condVal, llvm::ConstantInt::get(condVal->getType(), 0), "while_cond");
      gen->Builder.CreateCondBr(condBool, bodyBlock, endBlock);

      gen->Builder.SetInsertPoint(bodyBlock);

      size_t var_n = gen->m_vars.size();
      for (const auto &stmt : stmt_while.then_branch) {
        gen->gen_stmt(stmt);
      }
      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->Builder.CreateBr(startBlock);

      gen->Builder.SetInsertPoint(endBlock);
      gen->stmt_orde.pop();
    }
    void operator()(const NodeStmtLoop &stmt_loop) const {
      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock *loopBlock = llvm::BasicBlock::Create(
          TheContext, "loop_" + std::to_string(current) + "_start", function);
      llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(
          TheContext, "loop_" + std::to_string(current) + "_end", function);
      gen->stmt_orde.push({loopBlock, {endBlock, nullptr}});

      gen->Builder.CreateBr(loopBlock);

      gen->Builder.SetInsertPoint(loopBlock);

      size_t var_n = gen->m_vars.size();
      for (const auto &stmt : stmt_loop.then_branch) {
        gen->gen_stmt(stmt);
      }

      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->Builder.CreateBr(loopBlock);

      gen->Builder.SetInsertPoint(endBlock);
      gen->stmt_orde.pop();
    }

    void operator()(const NodeStmtFor &stmt_for) const {
      size_t var_n_before_loop = gen->m_vars.size();

      for (const auto &stmt : stmt_for.init)
        gen->gen_stmt(stmt);

      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function *func = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock *start_bb = llvm::BasicBlock::Create(
          TheContext, "for_start_" + std::to_string(current), func);
      llvm::BasicBlock *body_bb = llvm::BasicBlock::Create(
          TheContext, "for_body_" + std::to_string(current), func);
      llvm::BasicBlock *update_bb = llvm::BasicBlock::Create(
          TheContext, "for_update_" + std::to_string(current), func);
      llvm::BasicBlock *end_bb = llvm::BasicBlock::Create(
          TheContext, "for_end_" + std::to_string(current), func);

      gen->stmt_orde.push({start_bb, {end_bb, update_bb}});

      gen->Builder.CreateBr(start_bb);

      gen->Builder.SetInsertPoint(start_bb);
      llvm::Value *cond_val = gen->gen_expr(stmt_for.condition);
      llvm::Value *cond_bool = gen->Builder.CreateICmpNE(
          cond_val, llvm::ConstantInt::get(cond_val->getType(), 0), "for_cond");
      gen->Builder.CreateCondBr(cond_bool, body_bb, end_bb);

      gen->Builder.SetInsertPoint(body_bb);
      for (const auto &stmt : stmt_for.code_branch)
        gen->gen_stmt(stmt);
      gen->Builder.CreateBr(update_bb);

      gen->Builder.SetInsertPoint(update_bb);
      for (const auto &stmt : stmt_for.update)
        gen->gen_stmt(stmt);
      gen->Builder.CreateBr(start_bb);

      gen->Builder.SetInsertPoint(end_bb);

      while (gen->m_vars.size() > var_n_before_loop) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->stmt_orde.pop();
    }
    void operator()(const NodeStmtStop &stmt_stop) const {
      if (gen->stmt_orde.empty()) {
        add_error("Can not stop an inexistent loop", stmt_stop.line);
      }
      llvm::BasicBlock *loop_end = gen->stmt_orde.top().second.first;
      gen->Builder.CreateBr(loop_end);

      llvm::Function *func = gen->Builder.GetInsertBlock()->getParent();
      llvm::BasicBlock *after_stop = llvm::BasicBlock::Create(TheContext, "after_stop");
      gen->Builder.SetInsertPoint(after_stop);
    }

    void operator()(const NodeStmtContinue &stmt_continue) const {
      if (gen->stmt_orde.empty()) {
        add_error("Can not use 'continue' in an inexistent loop", stmt_continue.line);
      }

      if (gen->stmt_orde.top().second.second != nullptr) { // is for
        llvm::BasicBlock *for_update = gen->stmt_orde.top().second.second;
        gen->Builder.CreateBr(for_update);
      } else {
        llvm::BasicBlock *loop_start = gen->stmt_orde.top().first;
        gen->Builder.CreateBr(loop_start);
      }

      llvm::Function *func = gen->Builder.GetInsertBlock()->getParent();
      llvm::BasicBlock *after_continue =
          llvm::BasicBlock::Create(TheContext, "after_continue");
      gen->Builder.SetInsertPoint(after_continue);
    }

    void operator()(const NodeStmtDefFunc &stmt_def_func) const {
      std::string name = stmt_def_func.name.value.value();
      for (const auto &mod : m_mod) {
        name.append("$MOD" + mod);
      }

      size_t var_n = gen->m_vars.size();
      llvm::Type *ret_type = map_type_to_llvm(stmt_def_func.return_type, gen, stmt_def_func.return_type.is_ref, true).type;
      if (name == "main")
        ret_type = llvm::Type::getInt32Ty(TheContext);

      std::vector<llvm::Type *> param_types;
      for (const auto &c_arg : stmt_def_func.args) {
        param_types.push_back(
            map_type_to_llvm(c_arg.arg_type, gen, c_arg.arg_type.is_ref).type);
      }
      gen->declared_funcs.insert({name, {ret_type, param_types}});

      llvm::FunctionType *func_type =
          llvm::FunctionType::get(ret_type, param_types, stmt_def_func.is_vargs);

      llvm::GlobalValue::LinkageTypes linkage = stmt_def_func.is_extern
        ? llvm::Function::ExternalLinkage
        : llvm::Function::InternalLinkage;

      linkage = stmt_def_func.is_pub ? llvm::Function::ExternalLinkage : linkage;

      if (name == "main")
        linkage = llvm::Function::ExternalLinkage;

      llvm::Function *func =
        gen->ModModule->getFunction(name);

      if (!func) {
        func = llvm::Function::Create(
          func_type, linkage,
          name,
          gen->ModModule.get());
      } else {
        if (func->arg_size() != param_types.size() ||
          func->getReturnType() != ret_type) {
          add_error("Conflicting declaration of function " +
            name,
            stmt_def_func.line);
          return;
        }
      }

      if (!stmt_def_func.is_defined || stmt_def_func.is_extern)
        return;


      llvm::BasicBlock *entry =
        llvm::BasicBlock::Create(TheContext, name, func);
      gen->Builder.SetInsertPoint(entry);
      gen->current_mode = Mode::Function;

      auto argIt = func->arg_begin();
      for (const auto &c_arg : stmt_def_func.args) {
        argIt->setName(c_arg.name);

        llvm::AllocaInst *alloca_inst =
          gen->Builder.CreateAlloca(argIt->getType(), nullptr, c_arg.name + "_local");

        gen->Builder.CreateStore(&*argIt, alloca_inst);

        TypeMapping type_mapping =
            map_type_to_llvm(c_arg.arg_type, gen, c_arg.arg_type.is_ref);
        llvm::Type *type = type_mapping.type;
        llvm::Type *base_type = type_mapping.base_type;
        gen->insert_var(c_arg.name, nullptr, type, base_type, alloca_inst, true, false);

        ++argIt;
      }

      for (const auto &stmt : stmt_def_func.code_branch) {
        gen->gen_stmt(stmt);
      }

      if (ret_type->isVoidTy()) {
        gen->Builder.CreateRetVoid();
      } else if (name == "main") {
        gen->Builder.CreateRet(llvm::ConstantInt::get(
          llvm::Type::getInt32Ty(TheContext),
          0
        ));
      }

      gen->Builder.ClearInsertionPoint();
      gen->current_mode = Mode::Global;

      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }
    }

    void operator()(const NodeStmtEndfn &stmt_end_fn)
        const { // NOTE: OBSOLETE AND NOT USED FUNCTION
      gen->write("  leave");
      gen->write("  ret");
      gen->current_mode = Mode::Global;
      gen->current_func = "";
    }

    void operator()(const NodeStmtRet &stmt_ret) const {
      llvm::Value *ret_val = gen->gen_expr(stmt_ret.value);
      gen->Builder.CreateRet(ret_val);
    }

    void operator()(const NodeStmtUnload &stmt_unload) const {
      // Not used (at least by the moment)
    }

    void operator()(const NodeStmtCall &stmt_call) const {
      const std::string &fn = stmt_call.name.value.value();

      std::vector<NodeExprPtr> arg_values = stmt_call.args;
      call_func(fn, arg_values, gen, stmt_call.line, false);
    }

    void operator()(const NodeStmtProperty &stmt_property) const {}

    void operator()(const NodeStmtDeclmod &stmt_declmod) const {
      const std::string &ident = stmt_declmod.module_name.value.value();
      m_mod.push_back(ident);
    }
    void operator()(const NodeStmtEndmod &stmt_endmod) const {
      m_mod.pop_back();
    }
    void operator()(const NodeStmtUmod &stmt_umod) const {
      const std::string &name = stmt_umod.module_name.value.value();
      int mod_exists = false;
      for (int i = 0; i < m_mod.size(); ++i) {
        if (m_mod[i] == name)
          mod_exists = true;
      }
      if (!mod_exists) {
        add_error("Module '" + name + "' not found", stmt_umod.line);
      }

      for (const auto &fnc : m_fnc_mod) {
        if (fnc.first == name)
          gen->write("  .extern " + fnc.second);
      }
    }
    void operator()(const NodeStmtUbeepmod &stmt_umod) const {
      const std::string &name = stmt_umod.module_name.value.value();
      const std::vector<std::string> beepmodules = {"string", "std"};

      for (const std::string &module : beepmodules) {
        if (name == module) {
          const std::string &loc = "/usr/include/beepc/" + name + ".o";
          modules.push_back(loc);
          return;
        }
      }
      add_error("Unknown beep module", stmt_umod.line);
    }

    void operator()(const NodeStmtLlibrary &stmt_llibrary) const {
      gen->libraries.push_back(stmt_llibrary.name.value.value());
    }
    void operator()(const NodeStmtLibpath &stmt_libpath) const {
      gen->libpaths.push_back(stmt_libpath.path.value.value());
    }

    void operator()(const NodeStmtSetPtr &stmt_ptr) const {}

    void operator()(const NodeStmtGlobl &stmt_globl) const {
      // Not used (by the moment)
    }

    void operator()(const NodeStmtHeader &stmt_header) const { gen->is_header = true; }
    void operator()(const NodeStmtUhead &stmt_import) const {
      const std::string &name = stmt_import.mod_name.value.value();
      const std::string &path = name + ".ys";
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

      Lexer lexer(std::move(source_code));
      std::vector<Token> tokens = lexer.tokenize();

      Parser parser(std::move(tokens));
      std::optional<NodeProg> program = parser.parse_prog();

      if (!program.has_value()) {
        std::cerr << "Parsing failed in file: " << name << "\n";
      }

      std::string base_name = fs::path(path).stem().string();
      std::string gen_name = base_name;

      auto temp_module = std::make_unique<llvm::Module>("temp", TheContext);
      Generator generator(program.value(), name,
                          std::move(temp_module)); // temporal module
      generator.gen_prog();

      llvm::SmallVector<llvm::ReturnInst *, 8> Returns;

      for (auto &f : generator.ModModule->functions()) {
        llvm::ValueToValueMapTy VMap;
        llvm::Function *new_f = llvm::Function::Create(
          f.getFunctionType(), llvm::GlobalValue::ExternalLinkage, f.getName(),
          gen->ModModule.get()
        );

        auto new_arg_it = new_f->arg_begin();
        for (auto &Arg : f.args()) {
          VMap[&Arg] = &*new_arg_it;
          ++new_arg_it;
        }

        llvm::CloneFunctionInto(
            new_f, &f, VMap, llvm::CloneFunctionChangeType::LocalChangesOnly, Returns);
        new_f->copyAttributesFrom(&f);
      }

      /*llvm::ValueToValueMapTy VMap;
      for (auto &g : generator.ModModule->globals()) {
        llvm::GlobalVariable *new_g = new llvm::GlobalVariable(
            *gen->ModModule, g.getValueType(), g.isConstant(), g.getLinkage(),
            nullptr, g.getName());

        VMap[&g] = new_g;

        if (g.hasInitializer()) {
          llvm::Constant *new_init = llvm::MapValue(g.getInitializer(), VMap);
          new_g->setInitializer(new_init);
        }

        new_g->setAlignment(g.getAlign());
        new_g->setThreadLocalMode(g.getThreadLocalMode());
        new_g->setExternallyInitialized(g.isExternallyInitialized());
      }

      for (const auto &glob_var : generator.m_vars) {
        gen->m_vars.insert(glob_var);
      }*/

      for (const auto &declared_func : generator.declared_funcs) {
        gen->declared_funcs.insert(declared_func);
      }

      for (const auto &fnc_custom_ret : generator.m_fnc_custom_ret) {
        gen->m_fnc_custom_ret.insert(fnc_custom_ret);
      }

      for (const auto &struct_template : generator.m_struct_templates) {
        gen->m_struct_templates.insert(struct_template);
      }

      for (const auto &struct_arg_template : generator.m_struct_arg_templates) {
        gen->m_struct_arg_templates.insert(struct_arg_template);
      }
    }

    void operator()(const NodeStmtLeave &stmt_leave) const {
      gen->Builder.CreateRetVoid();
    }

    void operator()(const NodeStmtListElement &stmt_list_element) const {}

    void operator()(const NodeStmtStruct &stmt_struct) const {
      std::string name = stmt_struct.name.value.value();

      llvm::StructType *the_struct = llvm::StructType::create(TheContext, name);
      std::vector<llvm::Type *> parsed_fields;
      std::map<std::string, Type> raw_types;
      std::map<std::string, std::pair<int, llvm::Type *>> args;

      int index = 0;
      for (const auto &field : stmt_struct.fields) {
        auto llvm_field_type =
            map_type_to_llvm(field.second, gen, field.second.is_ref).type;
        if (!llvm_field_type) {
          std::cerr << "Error: type for field " << field.first << " is nullptr!\n";
          continue;
        }

        parsed_fields.push_back(llvm_field_type);
        raw_types.insert({field.first, field.second});
        args.insert({field.first, {index, llvm_field_type}});
        ++index;
      }

      the_struct->setBody(parsed_fields);
      gen->m_struct_templates.insert({name, the_struct});
      gen->m_struct_arg_templates.insert({name, args});
    }

    void operator()(const NodeStmtDefine &stmt_def) const {
      if (m_preprocessor_bool.contains(stmt_def.name.value.value())) {
        add_warning("Preprocessor already defined", stmt_def.line);
        return;
      }

      m_preprocessor_bool.insert(stmt_def.name.value.value());
      if (!stmt_def.value.empty()) {
        m_preprocessor.insert({stmt_def.name.value.value(), stmt_def.value});
      }
    }
    void operator()(const NodeStmtUndef &stmt_undef) const {
      if (!m_preprocessor.contains(stmt_undef.name.value.value())) {
        add_error("Inexistent preprocessor", stmt_undef.line);
        return;
      }

      m_preprocessor_bool.erase(stmt_undef.name.value.value());
    }

    void operator()(const NodeStmtPreprocessorCond &stmt_if) const {
      if (m_preprocessor_bool.contains(stmt_if.condition.value.value())) {
        for (const auto &stmt : stmt_if.then_branch) {
          gen->gen_stmt(stmt);
        }
        return;
      }

      int elif_index = 0;
      for (const auto &elif : stmt_if.elif_conditions) {
        if (m_preprocessor_bool.contains(elif.value.value())) {
          for (const auto &stmt : stmt_if.elif_branches[elif_index]) {
            gen->gen_stmt(stmt);
          }
          return;
        }
        ++elif_index;
      }

      for (const auto &stmt : stmt_if.else_branch) {
        gen->gen_stmt(stmt);
      }
    }

    void operator()(const NodeStmtPreError &stmt_error) const {
      add_error(stmt_error.err_msg, stmt_error.line);
    }
    void operator()(const NodeStmtPreWarning &stmt_warning) const {
      add_warning(stmt_warning.warn_msg, stmt_warning.line);
    }

    void operator()(const NodeStmtPrint &stmt_print) const {
      const std::string &fn = "print$MODstd";
      std::vector<llvm::Value *> values;

      values.push_back(gen->gen_expr(
          NodeExpr(NodeExprStrLit{.str_lit = stmt_print.fmt, .line = stmt_print.line}),
          false));
      for (const auto &arg_val : stmt_print.args) {
        values.push_back(gen->gen_expr(arg_val, false));
      }

      llvm::Function *func = gen->ModModule->getFunction(fn);
      if (!func) {
        std::vector<llvm::Type *> param_types = {
            llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(TheContext))};

        llvm::FunctionType *func_type = llvm::FunctionType::get(
            llvm::Type::getVoidTy(TheContext), param_types, true);

        func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                      "print", gen->ModModule.get());
      }
      llvm::Value *call = gen->Builder.CreateCall(func, values);
    }

    void operator()(const NodeStmtLabel &stmt_label) const {
      if (gen->current_mode != Mode::Function) {
        add_error("'label' instruction must be inside of a function", stmt_label.line);
        return;
      }

      llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();
      llvm::BasicBlock *block;
      auto it = gen->m_declared_blocks.find(stmt_label.ident.value.value());
      if (it == gen->m_declared_blocks.end()) {
        block = llvm::BasicBlock::Create(TheContext, stmt_label.ident.value.value(),
                                         function);
        gen->m_declared_blocks[stmt_label.ident.value.value()] = block;
      } else {
        block = it->second;
      }

      llvm::BasicBlock *prev_block = gen->Builder.GetInsertBlock();
      if (!prev_block->getTerminator()) {
        gen->Builder.CreateBr(block);
      }

      gen->Builder.SetInsertPoint(block);
    }

    void operator()(const NodeStmtGoto &stmt_goto) const {
      if (gen->current_mode != Mode::Function) {
        add_error("'goto' instruction must be inside of a function", stmt_goto.line);
        return;
      }

      if (!gen->m_declared_blocks.contains(stmt_goto.ident.value.value())) {
        llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *block = llvm::BasicBlock::Create(
            TheContext, stmt_goto.ident.value.value(), function);
        gen->m_declared_blocks.insert({stmt_goto.ident.value.value(), block});
      }

      gen->Builder.CreateBr(gen->m_declared_blocks.at(stmt_goto.ident.value.value()));
    }

    void operator()(const NodeStmtScope &stmt_scope) {
      static int label_counter = 0;
      int current = ++label_counter;

      llvm::Function *function = gen->Builder.GetInsertBlock()->getParent();

      llvm::BasicBlock *scope = llvm::BasicBlock::Create(
          TheContext, "scope_" + std::to_string(current) + "_start", function);
      gen->stmt_orde.push({scope, {nullptr, nullptr}});

      gen->Builder.CreateBr(scope);

      gen->Builder.SetInsertPoint(scope);

      size_t var_n = gen->m_vars.size();
      for (const auto &stmt : stmt_scope.code_branch) {
        gen->gen_stmt(stmt);
      }

      while (gen->m_vars.size() > var_n) {
        int i = gen->m_vars_order.size() - 1;
        const auto &var_name = gen->m_vars_order[i];
        gen->m_vars.erase(var_name);
        gen->m_vars_order.pop_back();
      }

      gen->stmt_orde.pop();
    }
  };

  StmtVisitor visitor{.gen = this};
  std::visit(visitor, stmt.var);
}

void Generator::gen_prog() {
  declared_funcs.clear();

  for (const NodeStmt &stmt : m_prog.stmts) {
    gen_stmt(stmt);
  }
}
