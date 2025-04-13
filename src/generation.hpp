#pragma once

#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_map>
#include "parser.hpp"

extern "C" void print();

static std::string escape_string(const std::string& raw);

enum class PrintType : int {
    Int = 0,
    Float = 1,
    Str = 2
};

class Generator {
    private:
        void push(const std::string& reg) {
            m_output << "  push " << reg << "\n";
            ++m_stack_size;
        }

        void pop(const std::string& reg) {
            m_output << "  pop " << reg << "\n";
            --m_stack_size;
        }

        struct Var {
            size_t stack_loc;
        };

        const NodeProg m_prog;
        std::stringstream m_output;
        size_t m_stack_size = 0;
        std::unordered_map<std::string, Var> m_vars {};
        std::vector<std::string> m_string_literals {};

    public:
        inline explicit Generator(NodeProg root) : m_prog(root) {}

        void gen_expr(const NodeExpr& expr) {
            struct ExprVisitor {
                Generator* gen;
                void operator()(const NodeExprIntLit& expr_int_lit) const {
                    gen->m_output << "  mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                    gen->push("rax");
                }

                void operator()(const NodeExprIdent& expr_ident) {
                    const std::string& name = expr_ident.ident.value.value();
                    if (!gen->m_vars.contains(name)) {
                        std::cerr << "Undeclared Identifier: " << name << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    const auto& var = gen->m_vars.at(name);
                    size_t offset_bytes = (gen->m_stack_size - var.stack_loc - 1) * 8;
                    gen->m_output << "  mov rsi, QWORD [rsp + " << offset_bytes << "]\n";
                }

                void operator()(const NodeExprStrLit& expr_str_lit) const {
                    std::string label = "str_" + std::to_string(gen->m_string_literals.size());
                    gen->m_string_literals.push_back(expr_str_lit.str_lit.value.value());
                    gen->m_output << "  lea rax, [" << label << "]\n";
                    gen->push("rax");
                }
            };

            ExprVisitor visitor{.gen = this};
            std::visit(visitor, expr.var);
        }

        void gen_stmt(const NodeStmt& stmt) {
            struct StmtVisitor {
                Generator* gen;

                void operator()(const NodeStmtExit& stmt_exit) const {
                    gen->gen_expr(stmt_exit.expr);
                    gen->m_output << "  mov rax, 60\n";
                    gen->pop("rdi");
                    gen->m_output << "  syscall\n";
                }

                void operator()(const NodeStmtVar& stmt_var) const {
                    const std::string& name = stmt_var.ident.value.value();
                    if (gen->m_vars.contains(name)) {
                        std::cerr << "Identifier already used: " << name << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    gen->gen_expr(stmt_var.expr);
                    gen->m_vars.insert({name, Var{.stack_loc = gen->m_stack_size - 1}});
                }

                void operator()(const NodeStmtCall& stmt_call) const {
                    std::string fn = stmt_call.name.value.value();
                    if (fn == "print" || fn == "println") {
                        gen->gen_expr(stmt_call.arg);
                        // Solo hacemos pop si no accedimos con mov directo (como en string literal)
                        if (!std::holds_alternative<NodeExprIdent>(stmt_call.arg.var)) {
                            gen->pop("rsi");
                        }
                        gen->m_output << "  mov rdi, " << static_cast<int>(PrintType::Str) << "\n";
                        gen->m_output << "  mov rdx, " << (fn == "println" ? 1 : 0) << "\n";
                        gen->m_output << "  extern print\n";
                        gen->m_output << "  call print\n";
                    }
                    else {
                        std::cerr << "Unknown function: " << fn << "\n";
                        exit(EXIT_FAILURE);
                    }
                }
            };

            StmtVisitor visitor{.gen = this};
            std::visit(visitor, stmt.var);
        }

        [[nodiscard]] std::string gen_prog() {
            m_output << "global main\nmain:\n";

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
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}
