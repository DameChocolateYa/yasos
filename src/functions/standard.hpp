#pragma once

#include "../parser.hpp"
#include "../generation.hpp"

extern int push_result_in_func;

void handle_end(const NodeStmtCall& stmt, Generator* gen);
void handle_print(const NodeStmtCall& stmt, Generator* gen);
void handle_println(const NodeStmtCall& stmt, Generator* gen);
void handle_clsterm(const NodeStmtCall& stmt, Generator* gen);
void handle_colorterm(const NodeStmtCall& stmt, Generator* gen);

void handle_testret(const NodeExprCall& expr, Generator* gen);
void handle_itostr(const NodeExprCall& expr, Generator* gen);
void handle_stoint(const NodeExprCall& expr, Generator* gen);
void handle_stofl(const NodeExprCall& expr, Generator* gen);
void handle_scani(const NodeExprCall& expr, Generator* gen);
void handle_strcmp(const NodeExprCall& expr, Generator* gen);
void handle_isnum(const NodeExprCall& expr, Generator* gen);
void handle_isfloat(const NodeExprCall& expr, Generator* gen);

void handle_test_str(const NodeExprProperty& property, Generator* gen, int property_is_func=false);
void handle_strcat(const NodeExprProperty& property, Generator* gen, int property_is_func=false); 
void handle_strlen(const NodeExprProperty& property, Generator* gen, int property_is_func=false); 
