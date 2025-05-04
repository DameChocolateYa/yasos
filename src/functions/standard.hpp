#pragma once

#include "../parser.hpp"
#include "../generation.hpp"

extern int push_result_in_func;

void handle_end(const NodeStmtCall& stmt, Generator* gen);
void handle_print(const NodeStmtCall& stmt, Generator* gen);
void handle_println(const NodeStmtCall& stmt, Generator* gen);
void handle_clsterm(const NodeStmtCall& stmt, Generator* gen);

void handle_testret(const NodeExprCall& expr, Generator* gen);
void handle_itostr(const NodeExprCall& expr, Generator* gen);
void handle_stoint(const NodeExprCall& expr, Generator* gen);
void handle_scani(const NodeExprCall& expr, Generator* gen);
void handle_strcmp(const NodeExprCall& expr, Generator* gen);
