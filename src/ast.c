#include "ast.h"
#include <stdlib.h>

json_value *json_new_value(mem_arena *arena, json_value_type type) {
  json_value *v = PUSH_STRUCT(arena, json_value);
  v->type = type;
  return v;
}

json_value *builder_string(mem_arena *arena, const token *t) {
  json_value *v = json_new_value(arena, JSON_VALUE_STRING);

  v->as.string.chars = t->lexeme;
  v->as.string.length = t->length;
  return v;
}

json_value *builder_number(mem_arena *arena, const token *t) {
  json_value *v = json_new_value(arena, JSON_VALUE_NUMBER);

  v->as.number = strtod(t->lexeme, NULL);
  return v;
}

json_value *builder_bool(mem_arena *arena, i32 b) {
  json_value *v = json_new_value(arena, JSON_VALUE_BOOL);

  v->as.boolean = b ? 1 : 0;
  return v;
}

json_value *builder_null(mem_arena *arena) {
  return json_new_value(arena, JSON_VALUE_NULL);
}
