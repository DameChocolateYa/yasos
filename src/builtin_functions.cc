#include <vector>

#include "parser.hh"
#include "generation.hh"

// Duplicated function, fix later
llvm::Value *str_to_val_DUPLICATED(Generator *gen, const std::string& value) {
  llvm::Constant *str_constant =
    llvm::ConstantDataArray::getString(TheContext, value, true);

  llvm::GlobalVariable *g_var_str = new llvm::GlobalVariable(
    *gen->ModModule, str_constant->getType(), false,
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

std::string ysformat(Generator *gen, NodeExpr expr) {
  std::string raw_expr;
  gen->gen_expr(expr, false, false, &raw_expr);

  return raw_expr;
}

void ysprint(Generator *gen, const std::string& fmt, std::vector<NodeExprPtr> args, int line) {
  std::string output;
  bool open_bracket = false;

  size_t i = 0;
  for (const char c : fmt) {
    if (c == '{') {
      open_bracket = true;
      std::string raw_arg = ysformat(gen, *args.at(i++));
      output += raw_arg;
      continue;
    } else if (c == '}' && open_bracket) {
      open_bracket = false;
      continue;
    }

    output += c;
  }

  const char *fn = "print$MODstd";
  llvm::Function *func = gen->ModModule->getFunction(fn);
  std::vector<llvm::Value*> values;
  values.push_back(str_to_val_DUPLICATED(gen, output));

  if (!gen->declared_funcs.contains(fn) || func == nullptr)
    add_error("Inexistent function (" + std::string(fn) + ")", line);

  gen->Builder.CreateCall(func, values);
}