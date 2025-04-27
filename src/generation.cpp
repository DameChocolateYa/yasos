#include "generation.hpp"
#include <cstdlib>
#include <functional>
#include <string>
#include <variant>

#include "functions/standard.hpp"
#include "global.hpp"
#include "parser.hpp"

std::unordered_map<std::string, VarType> known_function_types = {
    {"testret", VarType::Str},
    {"itostr", VarType::Str},
    {"stoint", VarType::Int},
    {"scani", VarType::Str},
    {"strcmp", VarType::Int},
};

struct Func {
    std::string name;
    std::vector<ArgType> args;
};

std::unordered_map<std::string, std::function<void(const NodeStmtCall&, Generator*)>> function_handlers;
std::unordered_map<std::string, std::function<void(const NodeExprCall&, Generator*)>> function_ret_handlers;
std::vector<Func> decfuncs;

void initialize_func_map() {
    function_handlers["end"] = &handle_end;
    function_handlers["print"] = &handle_print;
    function_handlers["println"] = &handle_println;
    function_handlers["clsterm"] = &handle_clsterm;
}

void initialize_func_ret_map() {
    function_ret_handlers["testret"] = &handle_testret;
    function_ret_handlers["itostr"] = &handle_itostr;
    function_ret_handlers["stoint"] = &handle_stoint;
    function_ret_handlers["scani"] = &handle_scani;
    function_ret_handlers["strcmp"] = &handle_strcmp;
}

static bool is_int(const NodeExpr& expr) { return std::holds_alternative<NodeExprIntLit>(expr.var); }
static bool is_str(const NodeExpr& expr) { return std::holds_alternative<NodeExprStrLit>(expr.var); }
static bool is_float(const NodeExpr& expr) { return std::holds_alternative<NodeExprFloatLit>(expr.var); }
static bool is_ident(const NodeExpr& expr) { return std::holds_alternative<NodeExprIdent>(expr.var); }
static bool is_call(const NodeExpr& expr) { return std::holds_alternative<NodeExprCall>(expr.var); }

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
                std::cerr << "Error: argument " << current_arg + 1 << " should be a float\n";
                exit(EXIT_FAILURE);
            }
        }
        else if (required_arg.first == ArgType::Integer) {
            if (!is_int(args[current_arg]->var)) {
                std::cerr << "Error: argument " << current_arg + 1 << " should be an integer\n";
                exit(EXIT_FAILURE);
            }
        }
        else if (required_arg.first == ArgType::String) {
            if (!is_str(args[current_arg]->var)) {
                std::cerr << "Error: argument " << current_arg + 1 << " should be a string\n";
                exit(EXIT_FAILURE);
            }
        }
        else if (std::holds_alternative<NodeExprCall>(args[current_arg]->var)) {
            // We will handle it later
        }

        ++current_arg;
    }
}

void Generator::gen_expr(const NodeExpr& expr) {
    struct ExprVisitor {
        Generator* gen;

        void operator()(const NodeExprIntLit& expr_int_lit) const {
            gen->write("  mov rax, " + expr_int_lit.int_lit.value.value());
            gen->push("rax");
        }

        void operator()(const NodeExprBinary& expr_bin) const {
            const std::string& op = expr_bin.op_token.value.value();

            gen->gen_expr(*expr_bin.lhs);
            gen->gen_expr(*expr_bin.rhs);

            gen->pop("rbx");
            gen->pop("rax");

            if (op == "+") {
                gen->write( "  add rax, rbx");
            }
            else if (op == "-") {
                gen->write("  sub rax, rbx");
            }
            else if (op == "*") {
                gen->write("  imul rax, rbx");
            }
            else if (op == "/") {
                gen->write("  xor rdx, rdx");
                gen->write("  idiv rbx");
            }
            else if (op == "==") {
                gen->write("  cmp rax, rbx");
                gen->write("  sete al");
                gen->write("movzx rax, al");
            }
            else if (op == "!=") {
                gen->write("  cmp rax, rbx");
                gen->write("  setne al");
                gen->write("  movzx rax, al");
            }
            else if (op == "<") {
                gen->write("  cmp rax, rbx");
                gen->write("  setl al");
                gen->write("  movzx rax, al");
            }
            else if (op == "<=") {
                gen->write("  cmp rax, rbx");
                gen->write("  setle al");
                gen->write("  movzx rax, al");
            }
            else if (op == ">") {
                gen->write("  cmp rax, rbx");
                gen->write("  setg al");
                gen->write("  movzx rax, al");
            }
            else if (op == ">=") {
                gen->write("  cmp rax, rbx");
                gen->write("  setge al");
                gen->write("  movzx rax, al");
            }
            else if (op == "&&") {
                gen->write("  cmp rax, 0");
                gen->write("  setne al");
                gen->write("  movzx rax, al");
                gen->write("  cmp rbx, 0");
                gen->write("  setne bl");
                gen->write("  and al, bl");
                gen->write("  movzx rax, al");
            }
            else if (op == "||") {
                gen->write("  cmp rax, 0");
                gen->write("  setne al");
                gen->write("  movzx rax, al");
                gen->write("  cmp rbx, 0");
                gen->write("  setne bl");
                gen->write("  or al, bl");
                gen->write("  movzx rax, al");
            }
            else {
                std::cerr << "Unsupported binary operator\n";
                terminate(EXIT_FAILURE);
            }
            gen->push("rax");
        }
        void operator()(const NodeExprBinaryAssign& expr_bin_assign) const {
            const std::string& op = expr_bin_assign.op_token.value.value();
            const std::string& var_name = expr_bin_assign.left_token.value.value();

            size_t offset_bytes = gen->get_var(var_name);
            gen->write("  mov rax, QWORD [ rsp + " + std::to_string(offset_bytes) + "]");
            gen->push("rax");

            gen->gen_expr(*expr_bin_assign.right_expr);

            gen->pop("rbx");
            gen->pop("rax");

            if (op == "+=") {
                gen->write("  add rax, rbx");
            }
            else if (op == "-=") {
                gen->write("  sub rax, rbx");
            }
            else if (op == "*=") {
                gen->write("  imul rax, rbx");
            }
            else if (op == "/=") {
                gen->write("  xor rdx, rdx\n");
                gen->write("  idiv rbx");
            }
            else {
                std::cerr << "Invalid assignment operator\n";
                terminate(EXIT_FAILURE);
            }

            gen->write("  mov QWORD [ rsp + " + std::to_string(offset_bytes) + "], rax");
            gen->push("rax");
        }
        void operator()(const NodeExprUnaryIncDec& expr_unary_operator) const {
            const std::string& op = expr_unary_operator.op_token.value.value();
            const std::string& var_name = expr_unary_operator.ident.value.value();

            size_t offset_bytes = gen->get_var(var_name);
            gen->write("  mov rax, QWORD [ rsp + " + std::to_string(offset_bytes) + " ]");
            gen->push("rax");

            if (op == "++") {
                gen->write("  add rax, 1");
            }
            else if (op == "--") {
                gen->write("  sub rax, 1");
            }
            else {
                std::cerr << "Invalid unary operator\n";
                terminate(EXIT_FAILURE);
            }
            gen->write("  mov QWORD [ rsp + " + std::to_string(offset_bytes) + "], rax");
            gen->push("rax");
        };

        void operator()(const NodeExprIdent& expr_ident) const {
            const std::string& var_name = expr_ident.ident.value.value();
            size_t offset_bytes = gen->get_var(var_name);
            gen->write("  mov rsi, QWORD [rsp + " + std::to_string(offset_bytes) + "]");
            gen->push("rsi");
        }

        void operator()(const NodeExprStrLit& expr_str_lit) const {
            std::string label = "str_" + std::to_string(gen->m_string_literals.size());
            gen->m_string_literals.push_back(expr_str_lit.str_lit.value.value());
            gen->write("  lea rax, [ rel " + label + "]");
            gen->push("rax");
        }

        void operator()(const NodeExprFloatLit& expr_float_lit) const {
            std::string label = "float_" + std::to_string(gen->m_float_literals.size());
            gen->m_float_literals.push_back((std::stof(expr_float_lit.float_lit.value.value())));
            gen->write("  movsd xmm0, [ " + label + "]");
            gen->push_float("xmm0");
        }

        void operator()(const NodeExprNone& expr_none) const {}
        void operator()(const NodeExprNoArg& expr_no_arg) const {}
        void operator()(const NodeExprCR& expr_cr) const {}

        void operator()(const NodeExprCall& expr_call) const {
            const std::string& fn = expr_call.name.value.value();

            auto it = function_ret_handlers.find(fn);
            if (it != function_ret_handlers.end()) {
                it->second(expr_call, gen);
            }
            else {
                std::cerr << "Error, unknown func\n";
                exit(EXIT_FAILURE);
            }
        }
    };

    ExprVisitor visitor{.gen = this};
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
                gen->write("  extern " + stmt_use.use[i].value.value());
            }
        }

        void operator()(const NodeStmtVar& stmt_var) const {
            const std::string& name = stmt_var.ident.value.value();
            gen->gen_expr(stmt_var.expr);
            if (gen->m_vars.contains(name)) {
                auto& var = gen->m_vars.find(name)->second;
                gen->m_vars.erase(name);
                gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var.type}});
                return;
            }

            VarType var_type;
            switch (stmt_var.type.type) {
                case TokenType::str_type: var_type = VarType::Str; break;
                case TokenType::int_type: var_type = VarType::Int; break;
                case TokenType::float_type: var_type = VarType::Float; break;
                default:
                    std::cerr << "Unknown variable type for: " << name << "\n";
                    terminate(EXIT_FAILURE);
            }

            gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var_type}});
        }

        void operator()(const NodeStmtIf stmt_if) const {
            static int label_counter = 0;
            int current = label_counter++;

            std::string else_label = "else_" + std::to_string(current);
            std::string end_label = "endif" + std::to_string(current);

            gen->gen_expr(stmt_if.condition);
            gen->pop("rax");
            gen->write( "  cmp rax, 0\n");
            gen->write("  je " + else_label);

            for (const auto& stmt : stmt_if.then_branch) {
                gen->gen_stmt(stmt);
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
        }

        void operator()(const NodeStmtPrint& stmt_print) const {
            gen->write("  mov rdi, 2");
            gen->write("  mov rdx, 0");
            std::string label = "str_" + std::to_string(gen->m_string_literals.size());
            gen->m_string_literals.push_back(stmt_print.str_lit.value.value());
            gen->write("  lea rsi, [ rel " + label + "]");
            gen->push("rsi");
            gen->write("  call print\n");

            if (stmt_print.args.empty()) return;
            for (const auto& arg : stmt_print.args) {
                PrintType print_type = PrintType::Int;
                if(std::holds_alternative<NodeExprIdent>(arg.var)) {
                    const auto& name = std::get<NodeExprIdent>(arg.var).ident.value.value();
                    if (!gen->m_vars.contains(name)) {
                        std::cerr << "Undeclared Indeitifier\n";
                        exit(EXIT_FAILURE);
                    }
                    auto var_type = gen->m_vars.at(name).type;
                    if (var_type == VarType::Str) print_type = PrintType::Str;
                }
                else if (std::holds_alternative<NodeExprStrLit>(arg.var)) {
                    print_type = PrintType::Str;
                }
                else if (std::holds_alternative<NodeExprIntLit>(arg.var)) print_type = PrintType::Int;
                else if (std::holds_alternative<NodeExprFloatLit>(arg.var)) {
                    print_type = PrintType::Float;
                }
                else if (std::holds_alternative<NodeExprCall>(arg.var)) {
                    const auto& call = std::get<NodeExprCall>(arg.var);
                    for (const auto& know_return : known_function_types) {
                        if (know_return.first == call.name.value.value()) {
                            print_type = static_cast<PrintType>(know_return.second);
                        }
                    }
                }
                else if (std::holds_alternative<NodeExprCR>(arg.var)) print_type = PrintType::CR;
                gen->gen_expr(arg);

                if (print_type != PrintType::Float && print_type != PrintType::CR) {
                    gen->write("  mov rdi, " + std::to_string(static_cast<int>(print_type)));
                    gen->pop("rsi");
                    gen->write("  mov rdx, 0");
                }
                else if (print_type == PrintType::CR) {
                    gen->write("  mov rdi, 2");
                    gen->write("  mov rdx, 3");
                }
                gen->write("  call print");
            }
        }

        const std::vector<std::string> regs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

        void operator()(const NodeStmtDefFunc& stmt_def_func) const {
            gen->current_mode = Mode::Function;
            gen->write(stmt_def_func.name.value.value() + ":");
            gen->write("  push rbp");
            gen->write("  mov rbp, rsp");
            if (stmt_def_func.args.size() == 0) return;

            for (int i = 0; i < stmt_def_func.args.size(); ++i) {
                VarType var_type;
                if (stmt_def_func.args[i].arg_type == ArgType::Integer) var_type = VarType::Int;
                else if (stmt_def_func.args[i].arg_type == ArgType::String) var_type = VarType::Str;

                // Insert temporal value to avoid 'Undeclared Identifier'
                gen->m_vars.insert({stmt_def_func.args[i].name, Var{.stack_loc = (gen->m_stack_size - 1) * 4, .type = var_type}});
            }
        }

        void operator()(const NodeStmtEndfn& stmt_end_fn) const {
            gen->write("  mov rsp, rbp");
            gen->write("  pop rbp");
            gen->write("  ret");
            gen->current_mode = Mode::Global;
        }

        void operator()(const NodeStmtCallCustomFunc& stmt_call_custom_func) const {
            std::vector<NodeExpr> arg_values = stmt_call_custom_func.arg_values;
            std::vector<Token> arg_names = stmt_call_custom_func.arg_names;

            int index = 0;
            std::string reg = ""; // orde: rdi, rsi, rdx, rcx, r8, r9
            for (const auto& arg : arg_values) {
                /*switch (reg_index) {
                    case 0: reg = "rdi"; break;
                    case 1: reg = "rsi"; break;
                    case 2: reg = "rdx"; break;
                    case 3: reg = "rcx"; break;
                    case 4: reg = "r8"; break;
                    case 5: reg = "r9"; break;
                }*/
                gen->gen_expr(arg);
                size_t current_stack_loc = gen->m_stack_size;

                //gen->write("  mov " + reg + ", rax");
                gen->push("rax");

                VarType var_type;
                if (is_int(arg.var)) var_type = VarType::Int;
                if (is_str(arg.var)) var_type = VarType::Str;

                if (index < arg_values.size()) {
                    gen->m_vars.insert({arg_names[index].value.value(), Var{.stack_loc = (current_stack_loc) * 8, .type = var_type}});
                    if (gen->m_vars.find(arg_names[index].value.value()) != gen->m_vars.end()) {
                        std::cerr << arg_names[index].value.value() << "\n";
                        std::cerr << gen->m_vars.at(arg_names[index].value.value()).stack_loc << "\n";
                    }
                }
                ++index;
            }

            /*for (int i = 0; i < arg_values.size(); ++i) {
                gen->m_output << "mov " << regs[i] << ", " << gen->get_var(arg_names[i].value.value()) << "\n";
                }*/

            gen->write("  call " + stmt_call_custom_func.name.value.value());
        }

        void operator()(const NodeStmtCall& stmt_call) const {
            const std::string& fn = stmt_call.name.value.value();
            auto it = function_handlers.find(fn);
            if (it != function_handlers.end()) {
                it->second(stmt_call, gen);
            }
            else {
                std::cerr << "Error, unknown func\n";
                exit(EXIT_FAILURE);
            }
        }
    };

    StmtVisitor visitor{.gen = this};
    std::visit(visitor, stmt.var);
}

[[nodiscard]] std::string Generator::gen_prog() {
    m_output << "global main\nsection .text\n";
    m_output << "  extern free\n";

    //m_output << "main:\n";
    for (const NodeStmt& stmt : m_prog.stmts) {
        gen_stmt(stmt);
    }
    m_output << "main:\n";
    m_output << function_buffer.str();

    m_output << "\nsection .rodata\n";
    if (!m_string_literals.empty()) {
        for (size_t i = 0; i < m_string_literals.size(); ++i) {
            m_output << "str_" << i << ": db \"" << escape_string(m_string_literals[i]) << "\", 0\n";
        }
    }
    if (!m_float_literals.empty()) {
        for (size_t i = 0; i < m_float_literals.size(); ++i) {
            m_output << "float_" << i << ": dq " << m_float_literals[i] << "\n";
        }
    }

    return m_output.str();
}
