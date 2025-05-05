#include "standard.hpp"
#include <string>
#include <variant>

int push_result_in_func = true;

void handle_end(const NodeStmtCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, No}});

    //gen->m_output << "  mov rdi, rax\n";
    //gen->m_output << "  call free\n";
    gen->write("    mov rax, 60");
    if (expr.args.size() == 0) {
        gen->write("  mov rdi, 0");
    }
    else if (expr.args.size() == 1) {
        if (std::holds_alternative<NodeExprIdent>(expr.args[0]->var)) {
            gen->gen_expr(*expr.args[0]);
            gen->pop("rdi");
        }
        else if (std::holds_alternative<NodeExprIntLit>(expr.args[0]->var)) {
            gen->write("  mov rdi, " + std::get<NodeExprIntLit>(expr.args[0]->var).int_lit.value.value());
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
    gen->write( "  syscall");
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
            gen->write("  mov rdi, " + std::to_string(static_cast<int>(print_type)));
            gen->pop("rsi");
            gen->write("  mov rdx, 0");
        }
        else if (print_type == PrintType::CR) {
            gen->write("  mov rdi, 2\n");
            gen->write("  mov rdx, 3\n");
        }
        else {
            gen->pop_float("xmm0");
            gen->write("  mov rdi, 1\n");
            gen->write("  mov rdx, 0\n");
        }
        gen->write("  call print\n");
    }
    if (println) {
        gen->write("  mov rdx, 3\n"); // Only an emty line for println
        gen->write("  call print\n");
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
    gen->write("  call clsterm");
}

void handle_testret(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});
    gen->write("  call testret");
    if (push_result_in_func) gen->push("rax");
}

void handle_itostr(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->write("  mov rdi, rax");

    gen->write( "  call itostr");
    if (push_result_in_func) gen->push("rax");
}

void handle_stoint(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->write("  mov rdi, rax");

    gen->write( "  call stoint");
    if (push_result_in_func) gen->push("rax");
}

void handle_scani(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});

    gen->write("  call scani");
    if (push_result_in_func) gen->push("rax");
}

void handle_strcmp(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}, {String, Yes}});
    gen->gen_expr(*expr.args[0], false, "rsi");
    gen->gen_expr(*expr.args[1], false , "rdi");

    gen->write("  call strcmp");
    if (push_result_in_func) gen->push("rax");
}
