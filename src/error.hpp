#pragma once

#include <iostream>

void add_error(const std::string& msg, int line = -1);
void add_warning(const std::string& msg, int line = -1);
