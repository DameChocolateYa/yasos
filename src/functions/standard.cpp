#include "standard.hpp"
#include <variant>

void handle_end(const NodeStmtCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, No}});

    //gen->m_output << "  mov rdi, rax\n";
    //gen->m_output << "  call free\n";
    gen->m_output << "  mov rax, 60\n";
    if (expr.args.size() == 0) {
        gen->m_output << "  mov rdi, 0\n";
    }
    else if (expr.args.size() == 1) {
        if (std::holds_alternative<NodeExprIdent>(expr.args[0]->var)) {
            gen->gen_expr(*expr.args[0]);
            gen->pop("rdi");
        }
        else if (std::holds_alternative<NodeExprIntLit>(expr.args[0]->var)) {
            gen->m_output << "  mov rdi, " << std::get<NodeExprIntLit>(expr.args[0]->var).int_lit.value.value() << "\n";
        }
        else if (std::holds_alternative<NodeExprCall>(expr.args[0]->var)) {
            const auto& call_expr = std::get<NodeExprCall>(expr.args[0]->var);
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

void print(const NodeStmtCall& stmt, Generator* gen, int println = false) {
    PrintType print_type = PrintType::Int;
    for (const auto& arg : stmt.args) {
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
        else if (std::holds_alternative<NodeExprIntLit>(arg->var)) print_type = PrintType::Int;
        else if (std::holds_alternative<NodeExprFloatLit>(arg->var)) {
            print_type = PrintType::Float;
        }
        else if (std::holds_alternative<NodeExprCall>(arg->var)) {
            const auto& call = std::get<NodeExprCall>(arg->var);
            for (const auto& know_return : known_function_types) {
                if (know_return.first == call.name.value.value()) {
                    print_type = static_cast<PrintType>(know_return.second);
                }
            }
        }
        else if (std::holds_alternative<NodeExprCR>(arg->var)) print_type = PrintType::CR;

        gen->gen_expr(*arg);

        if (print_type != PrintType::Float && print_type != PrintType::CR) {
            gen->m_output << "  mov rdi, " << static_cast<int>(print_type) << "\n";
            gen->pop("rsi");
            gen->m_output << "  mov rdx, 0" << "\n";
        }
        else if (print_type == PrintType::CR) {
            gen->m_output << "  mov rdi, 2\n";
            gen->m_output << "  mov rdx, 3\n";
        }
        else {
            gen->pop_float("xmm0");
            gen->m_output << "  mov rdi, 1\n";
            gen->m_output << "  mov rdx, 0\n";
        }
        gen->m_output << "  call print\n";
    }
    if (println) {
        gen->m_output << "  mov rdx, 3\n"; // Only an emty line for println
        gen->m_output << "  call print\n";
    }
}

void handle_print(const NodeStmtCall& stmt, Generator* gen) {
    check_func_args(stmt.args, {{NxtUndefNum, Yes}});
    print(stmt, gen, false);
}
void handle_println(const NodeStmtCall& stmt, Generator* gen) {
    check_func_args(stmt.args, {{NxtUndefNum, Yes}});
    print(stmt, gen, true);
}

void handle_clsterm(const NodeStmtCall& stmt, Generator* gen) {
    check_func_args(stmt.args, {});
    gen->m_output << "  call clsterm\n";
}

void handle_testret(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});
    gen->m_output << "  call testret\n";
    gen->push("rax");
}

void handle_itostr(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, Yes}});

    gen->gen_expr(*expr.args[0]);
    gen->pop("rdi");

    gen->m_output << "  call itostr\n";
    gen->push("rax");
}

void handle_stoint(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}});

    gen->gen_expr(*expr.args[0]);
    gen->pop("rdi");

    gen->m_output << "  call stoint\n";
    gen->push("rax");
}

void handle_scani(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});

    gen->m_output << "  call scani\n";
    gen->push("rax");
}

void handle_strcmp(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}, {String, Yes}});
    gen->gen_expr(*expr.args[0]);
    gen->gen_expr(*expr.args[1]);

    gen->pop("rdi");
    gen->pop("rsi");

    gen->m_output << "  call strcmp\n";
    gen->push("rax");
}
