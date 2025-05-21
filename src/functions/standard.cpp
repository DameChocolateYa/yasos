#include "standard.hpp"
#include <string>
#include <variant>

int push_result_in_func = true;

void handle_end(const NodeStmtCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, No}});

    //gen->m_output << "  mov rdi, rax\n";
    //gen->m_output << "  call free\n";
    gen->write("  mov $60, %rax");
    if (expr.args.size() == 0) {
        gen->write("  mov $0, %rdi");
    }
    else if (expr.args.size() == 1) {
        if (std::holds_alternative<NodeExprIdent>(expr.args[0]->var)) {
            gen->gen_expr(*expr.args[0]);
            gen->pop("rdi");
        }
        else if (std::holds_alternative<NodeExprIntLit>(expr.args[0]->var)) {
            gen->write("  mov $" + std::get<NodeExprIntLit>(expr.args[0]->var).int_lit.value.value() + ", %rdi");
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
    gen->write("  syscall");
}

void handle_clsterm(const NodeStmtCall& stmt, Generator* gen) {
    check_func_args(stmt.args, {});
    gen->call("clsterm");
}

void handle_colorterm(const NodeStmtCall& stmt, Generator* gen) {
    check_func_args(stmt.args, {});

    gen->gen_expr(*stmt.args[0], false, "rdi");
    gen->call("colorterm");
}

void handle_testret(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});
    gen->call("testret");
    if (push_result_in_func) gen->push("rax");
}

void handle_itostr(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{Integer, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->write("  mov %rax, %rdi");

    gen->call("itostr");
    if (push_result_in_func) gen->push("rax");
}

void handle_stoint(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");

    gen->call("stoint");
    if (push_result_in_func) gen->push("rax");
}

void handle_stofl(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");

    gen->call("stofl");
    if (push_result_in_func) gen->push("xmm0");
}

void handle_scani(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {});

    gen->call("scani");
    if (push_result_in_func) gen->push("rax");
}

void handle_strcmp(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{String, Yes}, {String, Yes}});
    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->gen_expr(*expr.args[1], false , "rsi");

    gen->call("strcmp");
    if (push_result_in_func) gen->push("rax");
}

void handle_isnum(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{VariableValue, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->call("isnum");

    if (push_result_in_func) gen->push("rax");
}

void handle_isfloat(const NodeExprCall& expr, Generator* gen) {
    check_func_args(expr.args, {{VariableValue, Yes}});

    gen->gen_expr(*expr.args[0], false, "rdi");
    gen->call("isfloat");

    if (push_result_in_func) gen->push("rax");
}

// String properties

void handle_test_str(const NodeExprProperty& property, Generator* gen, int property_is_func) {
    gen->call("testret");
    if (push_result_in_func) gen->push("rax");
}

void handle_strcat(const NodeExprProperty& property, Generator* gen, int property_is_func) {
    check_func_args(property.args, {{String, Yes}});

    gen->write("  mov " + std::to_string(gen->get_var(property.ident.value.value())) + "(%rsp), %rdi");
    gen->gen_expr(*property.args[0], false, "rsi");

    gen->call("strcat");

    if (push_result_in_func) gen->push("rax");
}

void handle_strlen(const NodeExprProperty& property, Generator* gen, int property_is_func) {
    check_func_args(property.args, {});

    gen->write("  mov " + std::to_string(gen->get_var(property.ident.value.value())) + "(%rsp), %rdi");
    gen->call("len");

    if (push_result_in_func) gen->push("rax");
}
