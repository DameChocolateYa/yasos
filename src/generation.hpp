#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include "parser.hpp"
#include "global.hpp"

#undef __FILE__
#define __FILE__ "src/generation.hpp"

static std::string escape_string(const std::string& raw);

enum ArgType {
    NoArg,
    None,
    Integer,
    String,
    Float,
    OptionalInteger,
    OptionalString,
    OptionalFloat
};

enum ArgRequired {Yes, No};

static void check_func_args(std::vector<NodeExpr> args, std::unordered_map<ArgType, ArgRequired> required_args) {
    int required_args_count = 0;
    for (const auto& required_arg : required_args) {
        if (required_arg.second == ArgRequired::Yes) ++required_args_count;
    }
    if (args.size() != required_args_count) {
        std::cerr << "Error, invalid args\n";
        exit(EXIT_FAILURE);
    }

    int current_arg = 0;
    for (const auto& required_arg : required_args) {
        if (required_arg.second == ArgRequired::No && dynamic_cast<NodeExprNoArg*>(args[current_arg].var)) continue;
        
        ++current_arg;
    }
}

enum class PrintType : int {
    Int = 0,
    Float = 1,
    Str = 2
};

class Generator {
private:
    struct Var {
        size_t stack_loc;
        VarType type;
    };

    const NodeProg m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Var> m_vars;
    std::vector<std::string> m_string_literals;
    std::vector<float> m_float_literals;

    void push(const std::string& reg) {
        m_output << "  push " << reg << "\n";
        ++m_stack_size;
    }

    void push_float(const std::string& reg) {
        //m_output << "   sub rsp, 8\n";
        m_output << "   movsd [rsp], " << reg << "\n";
        m_output << "   add rsp, 8\n";
        ++m_stack_size;
    }

    void pop(const std::string& reg) {
        m_output << "  pop " << reg << "\n";
        if (m_stack_size == 0) {
            std::cerr << "Stack underflow!\n";
            exit(EXIT_FAILURE);
        }
        --m_stack_size;
    }

    void pop_float(const std::string& reg) {
        m_output << "   movsd " << reg << ", [rsp]\n";
        m_output << "   add rsp, 8\n";
        if (m_stack_size == 0) {
            std::cerr << "Stack underflow!\n";
            exit(EXIT_FAILURE);
        }
        --m_stack_size;
    }

    size_t get_var(const std::string& var_name) {
        if (!m_vars.contains(var_name)) {
            std::cerr << "Error trying to get an undeclared variable\n";
            exit(EXIT_FAILURE);
        }
        size_t offset_bytes = (m_stack_size - m_vars.at(var_name).stack_loc - 1) * 8; // this should be the var pos
        return offset_bytes;
    }

public:
    inline explicit Generator(NodeProg root) : m_prog(std::move(root)) {}

    std::vector<std::string> libraries;

    void gen_expr(const NodeExpr& expr) {
        struct ExprVisitor {
            Generator* gen;

            void operator()(const NodeExprIntLit& expr_int_lit) const {
                gen->m_output << "  mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->push("rax");
            }

            void operator()(const NodeExprBinary& expr_bin) const {
                const std::string& op = expr_bin.op_token.value.value();

                gen->gen_expr(*expr_bin.lhs);
                gen->gen_expr(*expr_bin.rhs);

                gen->pop("rbx");
                gen->pop("rax");

                if (op == "+") {
                    gen->m_output << "  add rax, rbx\n";
                }
                else if (op == "-") {
                    gen->m_output << "  sub rax, rbx\n";
                }
                else if (op == "*") {
                    gen->m_output << "  imul rax, rbx\n";
                }
                else if (op == "/") {
                    gen->m_output << "  xor rdx, rdx\n";
                    gen->m_output << "  idiv rbx\n";
                }
                else if (op == "==") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  sete al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == "!=") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  setne al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == "<") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  setl al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == "<=") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  setle al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == ">") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  setg al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == ">=") {
                    gen->m_output << "  cmp rax, rbx\n";
                    gen->m_output << "  setge al\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == "&&") {
                    gen->m_output << "  cmp rax, 0\n";
                    gen->m_output << "  setne al\n";
                    gen->m_output << "  movzx rax, al\n";
                    gen->m_output << "  cmp rbx, 0\n";
                    gen->m_output << "  setne bl\n";
                    gen->m_output << "  and al, bl\n";
                    gen->m_output << "  movzx rax, al\n";
                }
                else if (op == "||") {
                    gen->m_output << "  cmp rax, 0\n";
                    gen->m_output << "  setne al\n";
                    gen->m_output << "  movzx rax, al\n";
                    gen->m_output << "  cmp rbx, 0\n";
                    gen->m_output << "  setne bl\n";
                    gen->m_output << "  or al, bl\n";
                    gen->m_output << "  movzx rax, al\n";
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
                gen->m_output << "  mov rax, QWORD [ rsp + " << offset_bytes << "]\n";
                gen->push("rax");

                gen->gen_expr(*expr_bin_assign.right_expr);

                gen->pop("rbx");
                gen->pop("rax");

                if (op == "+=") {
                    gen->m_output << "  add rax, rbx\n";
                }
                else if (op == "-=") {
                    gen->m_output << "  sub rax, rbx\n";
                }
                else if (op == "*=") {
                    gen->m_output << "  imul rax, rbx\n";
                }
                else if (op == "/=") {
                    gen->m_output << "  xor rdx, rdx\n";
                    gen->m_output << "  idiv rbx\n";
                }
                else {
                    std::cerr << "Invalid assignment operator\n";
                    terminate(EXIT_FAILURE);
                }

                gen->m_output << "  mov QWORD [ rsp + " << offset_bytes << "], rax\n";
                gen->push("rax");
            }
            void operator()(const NodeExprUnaryIncDec& expr_unary_operator) const {
                const std::string& op = expr_unary_operator.op_token.value.value();
                const std::string& var_name = expr_unary_operator.ident.value.value();

                size_t offset_bytes = gen->get_var(var_name);
                gen->m_output << "  mov rax, QWORD [ rsp + " << offset_bytes << " ]\n";
                gen->push("rax");

                if (op == "++") {
                    gen->m_output << "  add rax, 1\n";
                }
                else if (op == "--") {
                    gen->m_output << "  sub rax, 1\n";
                }
                else {
                    std::cerr << "Invalid unary operator\n";
                    terminate(EXIT_FAILURE);
                }
                gen->m_output << "  mov QWORD [ rsp + " << offset_bytes << "], rax\n";
                gen->push("rax");
            };

            void operator()(const NodeExprIdent& expr_ident) const {
                const std::string& var_name = expr_ident.ident.value.value();
                size_t offset_bytes = gen->get_var(var_name);
                gen->m_output << "  mov rsi, QWORD [rsp + " << offset_bytes << "]\n";
                gen->push("rsi");
            }

            void operator()(const NodeExprStrLit& expr_str_lit) const {
                std::string label = "str_" + std::to_string(gen->m_string_literals.size());
                gen->m_string_literals.push_back(expr_str_lit.str_lit.value.value());
                gen->m_output << "  lea rax, [ rel " << label << "]\n";
                gen->push("rax");
            }

            void operator()(const NodeExprFloatLit& expr_float_lit) const {
                std::string label = "float_" + std::to_string(gen->m_float_literals.size());
                gen->m_float_literals.push_back((std::stof(expr_float_lit.float_lit.value.value())));
                gen->m_output << "  movsd xmm0, [ " << label << "]\n";
                gen->push_float("xmm0");
            }

            void operator()(const NodeExprCall& expr_call) const {
                const std::string& fn = expr_call.name.value.value();

                if (fn == "itostr") {
                    if (expr_call.args.size() != 1) {
                        std::cerr << "Expected 1 arg\n";
                        terminate(EXIT_FAILURE);
                    }

                    gen->gen_expr(*expr_call.args[0]);
                    gen->pop("rdi");

                    gen->m_output << "  call itostr\n";
                    gen->push("rax");
                }
                else if (fn == "stoint") {
                    //if
                    gen->gen_expr(*expr_call.args[0]);
                    gen->pop("rdi");

                    gen->m_output << "  call stoint\n";
                    gen->push("rax");
                }
                else if (fn == "scani") {
                    gen->m_output << "  call scani\n";
                    gen->push("rax");
                }
                else if (fn == "testret") {
                    gen->m_output << "  call testret\n";
                    gen->push("rax");
                }
                else if (fn == "strcmp") {
                    gen->gen_expr(*expr_call.args[0]);
                    gen->gen_expr(*expr_call.args[1]);

                    gen->pop("rdi");
                    gen->pop("rsi");

                    gen->m_output << "  call strcmp\n";
                    gen->push("rax");
                }
            }
        };

        ExprVisitor visitor{.gen = this};
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
            Generator* gen;

            void operator()(const NodeStmtImport& stmt_import) const {
                const std::string& name = stmt_import.to_import.value.value();

                gen->libraries.push_back(name);
            }

            void operator()(const NodeStmtUse& stmt_use) const {
                //std::cout << stmt_use.use[0].value.value() << "\n";
                for (int i = 0; i < stmt_use.use.size(); ++i) {
                    gen->m_output << "  extern " << stmt_use.use[i].value.value() << "\n";
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
                gen->m_output << "  cmp rax, 0\n";
                gen->m_output << "  je " << else_label << "\n";

                for (const auto& stmt : stmt_if.then_branch) {
                    gen->gen_stmt(stmt);
                }
                gen->m_output << "  jmp " << end_label << "\n";

                gen->m_output << else_label << ":\n";
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

                gen->m_output << end_label << ":\n";
            }

            std::unordered_map<std::string, VarType> known_function_types = {
                {"testret", VarType::Str},
                {"itostr", VarType::Str},
                {"stoint", VarType::Int},
                {"scani", VarType::Str},
                {"strcmp", VarType::Int},
            };
            void operator()(const NodeStmtCall& stmt_call) const {
                const std::string& fn = stmt_call.name.value.value();

                if (fn == "end" || fn == "killme") {
                    //gen->m_output << "  mov rdi, rax\n";
                    //gen->m_output << "  call free\n";
                    gen->m_output << "  mov rax, 60\n";
                    if (stmt_call.args.size() == 0) {
                        gen->m_output << "  mov rdi, 0\n";
                    }
                    else if (stmt_call.args.size() == 1) {
                        if (std::holds_alternative<NodeExprIdent>(stmt_call.args[0]->var)) {
                            gen->gen_expr(*stmt_call.args[0]);
                            gen->pop("rdi");
                        }
                        else if (std::holds_alternative<NodeExprIntLit>(stmt_call.args[0]->var)) {
                            gen->m_output << "  mov rdi, " << std::get<NodeExprIntLit>(stmt_call.args[0]->var).int_lit.value.value() << "\n";
                        }
                        else if (std::holds_alternative<NodeExprCall>(stmt_call.args[0]->var)) {
                            const auto& call_expr = std::get<NodeExprCall>(stmt_call.args[0]->var);
                            gen->gen_expr(*call_expr.args[0]);  // Evaluamos el primer argumento
                            gen->pop("rdi");  // Movemos el valor a rdi para la syscall
                        }
                    }
                    else {
                        std::cerr << "Expected optionally 1 arg\n";
                        terminate(EXIT_FAILURE);
                    }
                    gen->m_output << "  syscall\n";
                }
                else if (fn == "print" || fn == "println") {
                    PrintType print_type = PrintType::Int;
                    for (const auto& arg : stmt_call.args) {
                        if (std::holds_alternative<NodeExprIdent>(arg->var)) {
                            const auto& name = std::get<NodeExprIdent>(arg->var).ident.value.value();
                            if (!gen->m_vars.contains(name)) {
                                std::cerr << "Undeclared identifier: " << name << "\n";
                                exit(EXIT_FAILURE);
                            }
                            auto var_type = gen->m_vars.at(name).type;
                            if (var_type == VarType::Str) print_type = PrintType::Str;
                        }
                        else if (std::holds_alternative<NodeExprStrLit>(arg->var)) {
                            print_type = PrintType::Str;
                        }
                        else if (std::holds_alternative<NodeExprFloatLit>(arg->var)) {
                            print_type = PrintType::Float;
                        }
                        else if (std::holds_alternative<NodeExprCall>(arg->var)) {
                            const auto& call = std::get<NodeExprCall>(arg->var);
                            for (const auto& know_return : known_function_types) {
                                if (know_return.first == call.name.value.value()) print_type = PrintType::Str;
                            }
                        }

                        gen->gen_expr(*arg);

                        if (print_type != PrintType::Float) {
                            gen->m_output << "  mov rdi, " << static_cast<int>(print_type) << "\n";
                            gen->pop("rsi");
                            gen->m_output << "  mov rdx, 0" << "\n";
                            LOG(__FILE__, "Printing float");
                        }
                        else {
                            gen->pop_float("xmm0");
                            gen->m_output << "  mov rdi, 1\n";
                            gen->m_output << "  mov rdx, 0\n";
                        }
                        gen->m_output << "  call print\n";
                    }
                    if (fn != "println") return;
                    gen->m_output << "  mov rdx, 3\n"; // Only an emty line for println
                    gen->m_output << "  call print\n";
                }
                else if (fn == "clsterm") {
                    if (stmt_call.args.size() != 0) {
                        std::cerr << "Expected 0 args\n";
                        exit(EXIT_FAILURE);
                    }
                    gen->m_output << "  call clsterm\n";
                }
                else if (fn == "waitk") {
                    if (stmt_call.args.size() != 0) {
                        std::cerr << "Expected 0 args\n";
                        exit(EXIT_FAILURE);
                    }
                    gen->m_output << "  call waitk\n";
                }
                else {
                    std::cerr << "Error, unknown function or statment\n";
                    terminate(EXIT_FAILURE);
                }
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]] std::string gen_prog() {
        m_output << "global main\nsection .text\nmain:\n";
        m_output << "  extern free\n";

        for (const NodeStmt& stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

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
};

static std::string escape_string(const std::string& raw) {
    std::string out;
    for (char c : raw) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:   out += c;
        }
    }
    return out;
}
