#include "generation.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <variant>

#include "functions/standard.hpp"
#include "global.hpp"
#include "parser.hpp"
#include "error.hpp"

int main_func_declared = false;

std::vector<std::string> modules;

std::vector<std::string> m_mod;
std::unordered_map<std::string, std::string> m_fnc_mod;
std::unordered_map<std::string, VarType> m_fnc_rets;

std::unordered_map<std::string, VarType> known_function_types = {
    {"sysexc", VarType::Int},
    {"testret", VarType::Str},
    {"itostr", VarType::Str},
    {"ftostr", VarType::Str},
    {"stoint", VarType::Int},
    {"stofl", VarType::Float},
    {"scani", VarType::Str},
    {"isnum", VarType::Int},
    {"isint", VarType::Int},
    {"isfloat", VarType::Int},
    {"isnum", VarType::Int},
    {"strcmp", VarType::Int},
    {"strcat", VarType::Str},
    {"strcut", VarType::Str},
    {"strsub", VarType::Str},
    {"len", VarType::Int},
    {"randi", VarType::Int},
    {"digtoabc", VarType::Str},
    {"sqrt", VarType::Float},
    {"round", VarType::Float},
    {"ceil", VarType::Float},
    {"floor", VarType::Float},
    {"pow", VarType::Float},
    {"fact", VarType::Int},
    {"log", VarType::Float}
};

struct Func {
    std::string name;
    std::vector<ArgType> args;
};

std::unordered_map<std::string, std::function<void(const NodeStmtCall&, Generator*)>> function_handlers;
std::unordered_map<std::string, std::function<void(const NodeExprCall&, Generator*)>> function_ret_handlers;
std::vector<Func> decfuncs;

std::unordered_map<std::string, std::function<void(const NodeExprProperty&, Generator*, int)>> str_ret_property;
std::unordered_map<std::string, std::function<void(const NodeStmtProperty&, Generator*, int)>> str_property;

void initialize_func_map() {
    //function_handlers["end"] = &handle_end;
    //function_handlers["clsterm"] = &handle_clsterm;
    //function_handlers["colorterm"] = &handle_colorterm;
}

void initialize_func_ret_map() {
    function_ret_handlers["testret"] = &handle_testret;
    //function_ret_handlers["stoint"] = &handle_stoint;
    //function_ret_handlers["stofl"] = &handle_stofl;
    //function_ret_handlers["scani"] = &handle_scani;
    //function_ret_handlers["isnum"] = &handle_isnum;
    //function_ret_handlers["isfloat"] = &handle_isfloat;
}

void initialize_str_property_map() {
    //
}

void initialize_str_ret_property_map() {
    str_ret_property["test"] = &handle_test_str;
    str_ret_property["cat"] = &handle_strcat;
    //str_ret_property["len"] = &handle_strlen;
}

static bool is_int(const NodeExpr& expr) { return std::holds_alternative<NodeExprIntLit>(expr.var); }
static bool is_str(const NodeExpr& expr) { return std::holds_alternative<NodeExprStrLit>(expr.var); }
static bool is_float(const NodeExpr& expr) { return std::holds_alternative<NodeExprFloatLit>(expr.var); }
static bool is_ident(const NodeExpr& expr) { return std::holds_alternative<NodeExprIdent>(expr.var); }
static bool is_call(const NodeExpr& expr) { return std::holds_alternative<NodeExprCall>(expr.var); }

static VarType check_value(const NodeExpr& expr, Generator* gen) {
    if (std::holds_alternative<NodeExprIntLit>(expr.var)) return VarType::Int;
    else if (std::holds_alternative<NodeExprStrLit>(expr.var)) return VarType::Str;
    else if (std::holds_alternative<NodeExprFloatLit>(expr.var)) return VarType::Float;
    else if (std::holds_alternative<NodeExprCall>(expr.var)) {
        NodeExprCall expr_call = std::get<NodeExprCall>(expr.var);
        const std::string& name = expr_call.name.value.value();
        if (!known_function_types.contains(name)) {
            return VarType::Void;
        }
        for (const auto& fnc : known_function_types) {
            if (fnc.first == name) return fnc.second;
        }
    }
    else if (std::holds_alternative<NodeExprCallCustomFunc>(expr.var)) {
        NodeExprCallCustomFunc expr_call = std::get<NodeExprCallCustomFunc>(expr.var);
        const std::string& name = expr_call.name.value.value();
        if (m_fnc_rets.contains(name)) {
            return m_fnc_rets.at(name);
        }
        if (known_function_types.contains(name)) {
            return known_function_types.at(name);
        }
        return VarType::Void;
    }
    else if (std::holds_alternative<NodeExprIdent>(expr.var)) {
        NodeExprIdent ident = std::get<NodeExprIdent>(expr.var);
        const std::string& name = ident.ident.value.value();
        if (gen->m_vars.contains(name)) {
            return gen->m_vars.at(name).type;
        }
        else if (gen->current_mode == Mode::Function) {
            if (!gen->m_fnc_args.contains(gen->current_func)) {
                return VarType::Void;
            }
            for (const auto& arg : gen->m_fnc_args.at(gen->current_func)) {
                if (arg.name == name) {
                    return arg.type;
                }
            }
            return VarType::Void;
        }
        else {
            return VarType::Void;
        }
    }
    else if (std::holds_alternative<NodeExprProperty>(expr.var)) {
        NodeExprProperty expr_property = std::get<NodeExprProperty>(expr.var);
        const std::string& name = expr_property.property.value.value();
        if (!known_function_types.contains(name)) {
            return VarType::Void;
        }
        for (const auto& fnc : known_function_types) {
            if (fnc.first == name) return fnc.second;
        }
    }
    else if (std::holds_alternative<NodeExprBinaryAssign>(expr.var)) {
        NodeExprBinaryAssign expr_bin = std::get<NodeExprBinaryAssign>(expr.var);
        if (gen->current_mode == Mode::Function) {
            if (!gen->m_fnc_args.contains(gen->current_func)) {
                return VarType::Void;
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
        VarType v1, v2;
        v1 = check_value(*expr_bin.lhs, gen);
        v2 = check_value(*expr_bin.rhs, gen);
        if (v1 == VarType::Float || v2 == VarType::Float) return VarType::Float;
        else return v1;
    }
    else if (std::holds_alternative<NodeExprUnaryIncDec>(expr.var)) {
        return VarType::Int;
    }

    return VarType::Void;
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
        if (required_arg.first == ArgType::VariableValue) continue;
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

void Generator::gen_expr(const NodeExpr& expr, bool push_result, const std::string& reg) {
    struct ExprVisitor {
        Generator* gen;
        bool push_result;
        std::string reg;

        void operator()(const NodeExprIntLit& expr_int_lit) const {
            gen->write("  mov $" + expr_int_lit.int_lit.value.value() + ", %" + reg);
            if (push_result) gen->push(reg);
        }

        void operator()(const NodeExprBinary& expr_bin) const {
            const std::string& op = expr_bin.op_token.value.value();

            VarType v1 = check_value(*expr_bin.lhs, gen);
            VarType v2 = check_value(*expr_bin.rhs, gen); 


            if (v1 != VarType::Float && v2 != VarType::Float) {
                gen->gen_expr(*expr_bin.rhs, false, "rbx");
                gen->gen_expr(*expr_bin.lhs, false);
            }
            else if (v1 == VarType::Int && v2 == VarType::Float) {
                gen->gen_expr(*expr_bin.lhs, false, "rbx");
                gen->write("cvtsi2sd %rbx, %xmm0");
                gen->gen_expr(*expr_bin.rhs, false, "xmm1");
            }
            else if (v1 == VarType::Float && v2 == VarType::Int) {
                gen->gen_expr(*expr_bin.rhs, false, "rbx");
                gen->write("cvtsi2sd %rbx, %xmm1");
                gen->gen_expr(*expr_bin.lhs, false, "xmm0");
            }
            else if (v1 == VarType::Float && v2 == VarType::Float) {
                gen->gen_expr(*expr_bin.rhs, false, "xmm1");
                gen->gen_expr(*expr_bin.lhs, false, "xmm0");
            }

            if (op == "+") { 
                if (v1 == VarType::Int && v2 == VarType::Int) gen->write( "  add %rbx, %rax");
                else if (v1 == VarType::Str) {
                    gen->write("  mov %rax, %rdi");
                    gen->write("  mov %rbx, %rsi");
                    gen->call("strcat"); 
                }
                else if (v1 == VarType::Float || v2 == VarType::Float) {
                    gen->write("  addsd %xmm1, %xmm0");
                }
            }
            else if (op == "-") {
                if (v1 == VarType::Int && v2 == VarType::Int) gen->write("  sub %rbx, %rax");
                else if (v1 == VarType::Float || v2 == VarType::Float) gen->write("  subsd %xmm1, %xmm0");
            }
            else if (op == "*") {
                if (v1 == VarType::Int && v2 == VarType::Int) gen->write("  imul %rbx, %rax");
                else if (v1 == VarType::Float || v2 == VarType::Float) gen->write("  mulsd %xmm1, %xmm0");
            }
            else if (op == "/") {
                gen->write("  xor %rdx, %rdx");
                if (v1 == VarType::Int && v2 == VarType::Int) gen->write("  idiv %rbx");
                else if (v1 == VarType::Float || v2 == VarType::Float) gen->write("  divsd %xmm1, %xmm0");
            }
            else if (op == "%") {
                if (v1 == VarType::Int && v2 == VarType::Int) {
                    gen->write("  xor %rdx, %rdx");
                    gen->write("  idiv %rbx");
                    gen->write("  mov %rdx, %rax");
                }
                else if (v1 == VarType::Float || v2 == VarType::Float) {
                    gen->write("  movapd %xmm0, %xmm2");        // copia a en xmm2
                    gen->write("  divsd %xmm1, %xmm0");         // xmm0 = a / b
                    gen->write("  roundsd $3, %xmm0, %xmm0");   // trunc(a / b)
                    gen->write("  mulsd %xmm1, %xmm0");         // xmm0 = b * trunc(a / b)
                    gen->write("  subsd %xmm0, %xmm2");         // xmm2 = a - b * trunc(a / b)
                    gen->write("  movapd %xmm2, %xmm0");        // resultado final en xmm0
                }
            }
            else if (op == "==") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  sete %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == "!=") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  setne %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == "<") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  setl %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == "<=") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  setle %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == ">") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  setg %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == ">=") {
                gen->write("  cmp %rbx, %rax");
                gen->write("  setge %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == "&&") {
                gen->write("  cmp $0, %rax");
                gen->write("  setne %al");
                gen->write("  movzx %al, %rax");
                gen->write("  cmp $0, %rbx");
                gen->write("  setne %bl");
                gen->write("  and %bl, %al");
                gen->write("  movzx %al, %rax");
            }
            else if (op == "||") {
                gen->write("  cmp $0, %rax");
                gen->write("  setne %al");
                gen->write("  movzx %al, %rax");
                gen->write("  cmp $0, %rbx");
                gen->write("  setne %bl");
                gen->write("  or %bl, %al");
                gen->write("  movzx %al, %rax");
            }
            else {
                add_error("Unsupported binary operator");
            }
            if (push_result) {
                if (v1 != VarType::Float && v2 != VarType::Float) gen->push(reg);
                else gen->push_float(reg);
            }
        }
        void operator()(const NodeExprBinaryAssign& expr_bin_assign) const {
            const std::string& op = expr_bin_assign.op_token.value.value();
            const std::string& var_name = expr_bin_assign.left_token.value.value();

            gen->gen_expr(*expr_bin_assign.right_expr, false);

            gen->write("  mov %rax, %rbx");

            size_t offset_bytes = gen->get_var(var_name);
            gen->write("  mov " + std::to_string(offset_bytes) + "(%rsp), %rax");

            if (op == "+=") {
                VarType var_type = gen->m_vars.at(var_name).type;

                if (var_type == VarType::Int) gen->write("  add %rbx, %rax");
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
            //if (push_result) gen->push("rax");
        }
        void operator()(const NodeExprUnaryIncDec& expr_unary_operator) const {
            const std::string& op = expr_unary_operator.op_token.value.value();
            const std::string& var_name = expr_unary_operator.ident.value.value();

            size_t offset_bytes = gen->get_var(var_name);
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
            if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& var : fnc.second) {
                            if (var.name == expr_ident.ident.value.value()) {
                                if (var.type != VarType::Float) gen->write("  mov -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                else gen->write("  movsd -" + std::to_string(var.stack_loc) + "(%rbp), %" + reg);
                                //gen->write("  mov rax, rsi");
                                if (push_result) {
                                    if (var.type != VarType::Float) gen->push(reg);
                                    else gen->push_float(reg);
                                }
                                return;
                            }
                        }
                    }
                }
            }
            const std::string& var_name = expr_ident.ident.value.value();
            size_t offset_bytes = gen->get_var(var_name);
            //std::cerr << gen->m_vars.at(var_name).stack_loc << "\n";
            if (gen->m_vars.at(var_name).type != VarType::Float) gen->write("  mov " +  std::to_string(offset_bytes) +" (%rsp), %" + reg);
            else gen->write("  movsd " + std::to_string(offset_bytes) + "(%rsp), %" + reg);
            //gen->write("  mov rax, rsi");
            if (push_result) {
                if (gen->m_vars.at(var_name).type != VarType::Float) gen->push(reg);
                else gen->push_float(reg);
            }
        }

        void operator()(const NodeExprStrLit& expr_str_lit) const {
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
            gen->write("  mov $0x7FFFFFFF, %" + reg);
            if (push_result) gen->push(reg);
        }
        void operator()(const NodeExprNoArg& expr_no_arg) const {}
        void operator()(const NodeExprCR& expr_cr) const {}

        void operator()(const NodeExprBoolValue& expr_bool) const {
            gen->write("  mov $" + std::to_string(expr_bool.value) + ", %" + reg);
            if (push_result) gen->push(reg);
        }

        void operator()(const NodeExprCallCustomFunc expr_call) const {
            std::vector<NodeExpr> arg_values = expr_call.arg_values;
            std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"};
            int index = 0;
            for (const auto& arg : arg_values) {
                VarType var_type = check_value(arg, gen);
                if (var_type != VarType::Float) gen->gen_expr(arg, false, regs[index]);
                else gen->gen_expr(arg, false, float_regs[index]);
                ++index;
            }
            gen->write("  call " + expr_call.name.value.value());
        }

        void operator()(const NodeExprCall& expr_call) const {
            const std::string& fn = expr_call.name.value.value();
            std::vector<NodeExprPtr> arg_values = expr_call.args;
            std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"};
            int index = 0;
            int reg_index = 0;
            int float_reg_index = 0;

            auto it = function_ret_handlers.find(fn);
            if (it != function_ret_handlers.end()) {
                it->second(expr_call, gen);
            }
            else {
                for (const auto& arg : arg_values) {
                    VarType var_type = check_value(*arg, gen);
                    if (var_type != VarType::Float) gen->gen_expr(*arg, false, regs[reg_index]);
                    else gen->gen_expr(*arg, false, float_regs[float_reg_index]);
                    if (var_type == VarType::Float) ++float_reg_index;
                    else ++reg_index;
                    ++index;
                }
                gen->write("  mov $" + std::to_string(float_reg_index) + ", %al");
                gen->call(fn);
                //add_error("Unknown function (" + fn + ")", expr_call.line);
            }
        }

        void operator()(const NodeExprProperty& stmt_property) const {
            const std::string& ident = stmt_property.ident.value.value();
            const std::string& property = stmt_property.property.value.value();

            VarType var_type = VarType::Void; 
            if (gen->current_mode == Mode::Function) {
                for (const auto& fnc : gen->m_fnc_args) {
                    if (fnc.first == gen->current_func) {
                        for (const auto& arg : fnc.second) {
                            if (arg.name == ident) var_type = arg.type;
                        }
                    }
                } 
            }
            else if (gen->m_vars.contains(ident)) {
                var_type = gen->m_vars.at(ident).type;
            }
            if (var_type == VarType::Void) add_error("Unknown variable (" + ident + ")", stmt_property.line);


            switch (var_type) {
                case VarType::Str:
                    auto it = str_ret_property.find(property);
                    if (it != str_ret_property.end()) {
                        push_result_in_func = push_result;
                        it->second(stmt_property, gen, stmt_property.is_func);
                    }
                    else {
                        add_error("Unknown func (" + property, stmt_property.line);
                        exit(EXIT_FAILURE);
                    }
                    break;
            }
        }
    };

    ExprVisitor visitor{.gen = this, .push_result = push_result, .reg = reg};
    std::visit(visitor, expr.var);
}

void Generator::gen_stmt(const NodeStmt& stmt) {
    struct StmtVisitor {
        Generator* gen;

        void operator()(const NodeStmtImport& stmt_import) const {
            const std::string& name = stmt_import.to_import.value.value();

            gen->libraries.push_back(name);
        }

        void operator()(const NodeStmtUse& stmt_use) const {
            //std::cout << stmt_use.use[0].value.value() << "\n";
            for (int i = 0; i < stmt_use.use.size(); ++i) {
                gen->write("  .extern " + stmt_use.use[i].value.value());
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
            }
        }

        void operator()(const NodeStmtVar& stmt_var) const {
            if (gen->current_mode == Mode::Global) {
                add_error("Variables only can be declared inside of a function", stmt_var.line);
            }

            const std::string& name = stmt_var.ident.value.value(); 
            VarType value_type = check_value(stmt_var.expr, gen);
            if (value_type != VarType::Float) gen->gen_expr(stmt_var.expr, false);
            else gen->gen_expr(stmt_var.expr, false, "xmm0");
 
            if (value_type != stmt_var.type) {
                //add_error("Tried to assign a value of diferent type", stmt_var.line);
            }
            
            if (stmt_var.type != VarType::Float) gen->push("rax", false);
            else gen->push_float("xmm0", false);
            gen->write(" # Creating a new var (" + name + ")");
            //gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var_type, .name = name}});
            gen->insert_var(name, value_type, stmt_var.is_mutable);
        }

        void operator()(const NodeStmtVarRe& stmt_var) const {
            if (gen->current_mode == Mode::Global) {
                add_error("Variables only can be redefined inside of a function", stmt_var.line);
            }

            const std::string& name = stmt_var.ident.value.value();
            if (!gen->m_vars.contains(name)) {
                add_error("Tried to edit an inexistent variable", stmt_var.line);
            }

            VarType value_type = check_value(stmt_var.expr, gen);
            if (value_type != VarType::Float) gen->gen_expr(stmt_var.expr, false);
            else gen->gen_expr(stmt_var.expr, false, "xmm0");

            if (gen->m_vars.contains(name)) {
                Var var = gen->m_vars.at(name);
                if (!var.is_mutable) {
                    add_error("Cannot modify a constant (" + name + ")", stmt_var.line);
                }
                if (gen->m_vars.at(name).type != value_type) {
                    //std::cerr << static_cast<int>(value_type) << "\n";
                    add_error("Expected same type in reassigment", stmt_var.line);
                }

                size_t offset_bytes = gen->get_var(name);
                gen->write("  mov %rax, " + std::to_string(offset_bytes)  + "(%rsp) # Editing var value (" + name + ")");
                return;
            }

        }

        void operator()(const NodeStmtIf& stmt_if) const {
            static int label_counter = 0;
            int current = label_counter++;

            std::string else_label = "else_" + std::to_string(current);
            std::string end_label = "endif" + std::to_string(current);

            gen->gen_expr(stmt_if.condition, false);
            gen->write( "  cmp $0, %rax");
            gen->write("  je " + else_label);

            size_t stack_start = gen->m_vars.size();
            for (const auto& stmt : stmt_if.then_branch) {
                gen->gen_stmt(stmt);
            }
            while (gen->m_vars.size() > stack_start) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->pop("rax");
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }

            gen->write("  jmp " + end_label);

            gen->write(else_label + ":");
            if (stmt_if.else_branch.has_value()) {
                if (std::holds_alternative<std::shared_ptr<NodeStmtIf>>(stmt_if.else_branch.value())) {
                    gen->gen_stmt(NodeStmt{
                        .var = *std::get<std::shared_ptr<NodeStmtIf>>(stmt_if.else_branch.value())
                    });
                }
                else {
                    for (const auto& stmt : std::get<std::vector<NodeStmt>>(stmt_if.else_branch.value())) {
                        gen->gen_stmt(stmt);
                    }
                }
            }

            gen->write(end_label + ":");
            while (gen->m_vars.size() > stack_start) {
                gen->pop("rax");
            }
        }

        void operator()(const NodeStmtWhile& stmt_while) const {
            static int label_counter = 0;
            int current = ++label_counter;

            std::string label = "while_" + std::to_string(current) + "_";
            std::string start_label = label + "start";
            std::string end_label = label + "end";
            gen->stmt_orde.push(label);
            size_t stack_start = gen->m_vars.size();

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

            for (const auto& stmt : stmt_while.afi) {
                //gen->gen_stmt(stmt);
            }

            while (gen->m_vars.size() > stack_start) {
                int i = gen->m_vars_order.size() - 1;
                const auto& var_name = gen->m_vars_order[i];
                gen->pop("rax");
                gen->m_vars.erase(var_name);
                gen->m_vars_order.pop_back();
            }
 
            gen->write("  jmp " + start_label);
            gen->write(end_label + ":"); 
            while (gen->m_vars.size() > stack_start) {
                gen->pop("rax");
            }
        }

        void operator()(const NodeStmtStop& stmt_stop) const {
            if (gen->stmt_orde.empty()) {
                add_error("Can not stop an inexistent loop", stmt_stop.line);
            }
            std::string end_label = gen->stmt_orde.top() + "end";
            gen->write("  jmp " + end_label);
            gen->stmt_orde.pop();
        }

        void operator()(const NodeStmtContinue& stmt_continue) const {
            if (gen->stmt_orde.empty()) {
                add_error("Can not use 'continue' in an inexistent loop", stmt_continue.line);
            }
            std::string start_label = gen->stmt_orde.top() + "start";
            gen->write("  jmp " + start_label);
        }

        void operator()(const NodeStmtPrint& stmt_print) const {
            NodeExpr format = stmt_print.str;
            std::vector<NodeExpr> arg_values = stmt_print.args;
            std::vector<std::string> regs = {"rsi", "rdx", "rcx", "r8", "r9"};
            std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"};
            int index = 0;
            int reg_index = 0;
            int float_reg_index = 0;

            gen->gen_expr(format, false, "rdi");
            for (const auto& arg : stmt_print.args) {
                VarType var_type = check_value(arg, gen);
                if (var_type != VarType::Float) gen->gen_expr(arg, false, regs[reg_index]);
                else gen->gen_expr(arg, false, float_regs[float_reg_index]);
                if (var_type == VarType::Float) ++float_reg_index;
                else ++reg_index;
                ++index;
            }
            gen->write("  mov $" + std::to_string(float_reg_index) + ", %al");
            gen->call("printbp");
        }

        const std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
        const std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm3", "xmm4", "xmm5", "xmm6"};

        void operator()(const NodeStmtDefFunc& stmt_def_func) const {
            if (gen->current_mode == Mode::Function) {
                add_error("Function declaration inside antoher function is not allowed", stmt_def_func.line);
            }
            if (gen->mod == Mode::Mod) {
                m_fnc_mod.insert({gen->current_mod, stmt_def_func.name.value.value()});
                gen->write("  .globl " + stmt_def_func.name.value.value());
            }
            if (stmt_def_func.name.value.value() == "main" && main_func_declared != -5) {
                main_func_declared = true;
            }

            gen->current_mode = Mode::Function;
            
            int index = 1;
            int reg_index = 0;
            int float_reg_index = 0;
            std::vector<Var> args;

            gen->current_func = stmt_def_func.name.value.value();
            gen->write(stmt_def_func.name.value.value() + ":");
            gen->write("  push %rbp");
            gen->write("  mov %rsp, %rbp");

            for (int i = 0; i < stmt_def_func.args.size(); ++i) {
                auto arg = stmt_def_func.args[i];
                VarType var_type;
                if (arg.arg_type == ArgType::String) var_type = VarType::Str;
                if (arg.arg_type == ArgType::Integer) var_type = VarType::Int;
                if (arg.arg_type == ArgType::Float) var_type = VarType::Float;

                args.push_back({.stack_loc = static_cast<size_t>(index) * 16, .type = var_type, .name = arg.name});
                if (var_type != VarType::Float) { 
                    gen->write("  sub $16, %rsp");
                    gen->write("  mov %" + regs[reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 16) + "(%rbp)"); 
                    ++reg_index;
                }
                else {
                    gen->write("  sub $16, %rsp");
                    gen->write("  movsd %" + float_regs[float_reg_index] + ", -" + std::to_string(static_cast<size_t>(index) * 16) + "(%rbp)");  
                    ++float_reg_index;
                }

                ++index;
            }
            gen->m_fnc_args.insert({stmt_def_func.name.value.value(), args});
            m_fnc_rets.insert({stmt_def_func.name.value.value(), stmt_def_func.return_type}); 
        }

        void operator()(const NodeStmtEndfn& stmt_end_fn) const {
            gen->write("  mov %rbp, %rsp");
            gen->write("  pop %rbp");
            gen->write("  ret");
            gen->current_mode = Mode::Global;
            gen->current_func = "";
        }

        void operator()(const NodeStmtRet& stmt_ret) const {
            NodeExpr value = stmt_ret.value;
            if (check_value(value, gen) != VarType::Float) gen->gen_expr(value, false, "rax");
            else gen->gen_expr(value, false, "xmm0");

            gen->write("  mov %rbp, %rsp");
            gen->write("  pop %rbp");
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
                gen->write("  pop %rax");
                gen->write("  sub $" + std::to_string(offset) + ", %rsp");
                gen->m_vars.erase(it);  // borrar sin invalidar iterador de bucle
                auto& names = gen->m_vars_order;
                names.erase(
                    std::remove(names.begin(), names.end(), name),
                    names.end()
                ); 
            }
        }

        void operator()(const NodeStmtCallCustomFunc& stmt_call_custom_func) const {
            std::vector<NodeExpr> arg_values = stmt_call_custom_func.arg_values;

            std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"};
            int index = 0;
            int reg_index = 0;
            int float_reg_index = 0;

            for (const auto& arg : arg_values) {
                VarType var_type = check_value(arg, gen);
                if (var_type != VarType::Float) {
                    gen->gen_expr(arg, false, regs[reg_index]);
                    ++reg_index;
                }
                else { 
                    gen->gen_expr(arg, false, float_regs[float_reg_index]);
                    ++float_reg_index;
                }
                ++index;
            }
            gen->write("  call " + stmt_call_custom_func.name.value.value());
        }

        void operator()(const NodeStmtCall& stmt_call) const {
            const std::string& fn = stmt_call.name.value.value();
            std::vector<NodeExprPtr> arg_values = stmt_call.args;
            std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            std::vector<std::string> float_regs = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4"};
            int index = 0;
            int reg_index = 0;
            int float_reg_index = 0;

            auto it = function_handlers.find(fn);
            if (it != function_handlers.end()) {
                it->second(stmt_call, gen);
            }
            else {
                for (const auto& arg : arg_values) {
                    VarType var_type = check_value(*arg, gen);
                    if (var_type != VarType::Float) gen->gen_expr(*arg, false, regs[reg_index]);
                    else gen->gen_expr(*arg, false, float_regs[float_reg_index]);
                    if (var_type == VarType::Float) ++float_reg_index;
                    else ++reg_index;
                    ++index;
                }
                gen->write("  mov $" + std::to_string(float_reg_index) + ", %al");
                gen->call(fn);
                //add_error("Unknown function (" + fn + ")", expr_call.line);
            }
        }

        void operator()(const NodeStmtProperty& stmt_property) const {
            const std::string& ident = stmt_property.ident.value.value();
            const std::string& property = stmt_property.property.value.value();

            if (!gen->m_vars.contains(ident)) {
                add_error("Unknown variable (" + ident + ")", stmt_property.line);
            }
            VarType var_type = gen->m_vars.at(ident).type;

            switch (var_type) {
                case VarType::Str:
                    auto it = str_property.find(property);
                    if (it != str_property.end()) {
                        it->second(stmt_property, gen, stmt_property.is_func);
                    }
                    else {
                        add_error("Unknown function (" + property + ")", stmt_property.line);
                    }
                    break;
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
    };

    StmtVisitor visitor{.gen = this};
    std::visit(visitor, stmt.var);
}

[[nodiscard]] std::string Generator::gen_prog() {
    //m_output << ".globl " << filename << /*"\nsection .text*/"\n";
    //m_output << "extern free\n";

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

    //m_output << "\nsection .rodata\n";
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
