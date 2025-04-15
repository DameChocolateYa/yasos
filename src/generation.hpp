#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include "parser.hpp"

static std::string escape_string(const std::string& raw);

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

    void push(const std::string& reg) {
        m_output << "  push " << reg << "\n";
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
                else {
                    std::cerr << "Unsupported binary operator\n";
                    exit(EXIT_FAILURE);
                }
                gen->push("rax");
            }
            void operator()(const NodeExprBinaryAssign& expr_bin_assign) const {}
            void operator()(const NodeExprUnaryIncDec& algo) const {};

            void operator()(const NodeExprIdent& expr_ident) const {
                const std::string& name = expr_ident.ident.value.value();
                if (!gen->m_vars.contains(name)) {
                    std::cerr << "Undeclared identifier: " << name << "\n";
                    exit(EXIT_FAILURE);
                }
                const auto& var = gen->m_vars.at(name);
                size_t offset_bytes = (gen->m_stack_size - var.stack_loc - 1) * 8;
                gen->m_output << "  mov rsi, QWORD [rsp + " << offset_bytes << "]\n";
                gen->push("rsi");
            }

            void operator()(const NodeExprStrLit& expr_str_lit) const {
                std::string label = "str_" + std::to_string(gen->m_string_literals.size());
                gen->m_string_literals.push_back(expr_str_lit.str_lit.value.value());
                gen->m_output << "  lea rax, [" << label << "]\n";
                gen->push("rax");
            }

            void operator()(const NodeExprCall& expr_call) const {
                const std::string& fn = expr_call.name.value.value();

                if (fn == "itostr") {
                    if (expr_call.args.size() != 1) {
                        std::cerr << "Expected 1 arg\n";
                        exit(EXIT_FAILURE);
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
            }
        };

        ExprVisitor visitor{.gen = this};
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
            Generator* gen;

            void operator()(const NodeStmtInclude& stmt_include) const {
                //std::cout << stmt_include.include.value.value() << "\n";
                const std::string& name = stmt_include.include.value.value();

                if (name == "standard") {
                    gen->libraries.push_back("std");
                }
            }

            void operator()(const NodeStmtUse& stmt_use) const {
                //std::cout << stmt_use.use[0].value.value() << "\n";
                for (int i = 0; i < stmt_use.use.size(); ++i) {
                    gen->m_output << "  extern " << stmt_use.use[i].value.value() << "\n";
                }
            }

            void operator()(const NodeStmtVar& stmt_var) const {
                const std::string& name = stmt_var.ident.value.value();
                if (gen->m_vars.contains(name)) {
                    gen->gen_expr(stmt_var.expr);
                    auto& var = gen->m_vars.find(name)->second;
                    for (auto it = gen->m_vars.begin(); it != gen->m_vars.end();) {
                        if (it->first == name) {
                            gen->m_vars.erase(it);
                            break;
                        }
                        else {
                            ++it;
                        }
                    }
                    gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var.type}});
                    return;
                }

                gen->gen_expr(stmt_var.expr);

                VarType var_type;
                switch (stmt_var.type.type) {
                    case TokenType::str_type: var_type = VarType::Str; break;
                    case TokenType::int_type: var_type = VarType::Int; break;
                    default:
                        std::cerr << "Unknown variable type for: " << name << "\n";
                        exit(EXIT_FAILURE);
                }

                gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1, .type = var_type}});
            }

            std::unordered_map<std::string, VarType> known_function_types = {
                {"testret", VarType::Str},
                {"itostr", VarType::Str},
                {"scani", VarType::Str}
            };
            void operator()(const NodeStmtCall& stmt_call) const {
                const std::string& fn = stmt_call.name.value.value();

                if (fn == "end") {
                    gen->m_output << "  mov rax, 60\n";
                    if (stmt_call.args.size() == 0) {
                        gen->m_output << "  mov rdi, 0\n";
                    }
                    else if (stmt_call.args.size() == 1) {
                        gen->m_output << "mov rdi, " << std::get<NodeExprIntLit>(stmt_call.args[0]->var).int_lit.value.value() << "\n";
                    }
                    else {
                        std::cerr << "Expected optionally 1 arg\n";
                        exit(EXIT_FAILURE);
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
                        else if (std::holds_alternative<NodeExprCall>(arg->var)) {
                            const auto& call = std::get<NodeExprCall>(arg->var);
                            for (const auto& know_return : known_function_types) {
                                if (know_return.first == call.name.value.value()) print_type = PrintType::Str;
                            }
                        }

                        gen->gen_expr(*arg);

                        gen->m_output << "  mov rdi, " << static_cast<int>(print_type) << "\n";
                        gen->pop("rsi");
                        gen->m_output << "  mov rdx, 0" << "\n";
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
                else {
                    std::cerr << "Error, unknown function or statment\n";
                    exit(EXIT_FAILURE);
                }
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]] std::string gen_prog() {
        m_output << "global main\nsection .text\nmain:\n";

        for (const NodeStmt& stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

        if (!m_string_literals.empty()) {
            m_output << "\nsection .rodata\n";
            for (size_t i = 0; i < m_string_literals.size(); ++i) {
                m_output << "str_" << i << ": db \"" << escape_string(m_string_literals[i]) << "\", 0\n";
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
