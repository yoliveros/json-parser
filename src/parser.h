#pragma once
#include "ast.h"

json_value *parse_value(mem_arena *arena, lexer *lx);
void print_json_value(json_value *value, i32 indent);
