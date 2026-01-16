#pragma once

#include "arena.h"
#include "base.h"
#include "lexer.h"

typedef enum {
  JSON_VALUE_OBJECT,
  JSON_VALUE_ARRAY,
  JSON_VALUE_STRING,
  JSON_VALUE_NUMBER,
  JSON_VALUE_BOOL,
  JSON_VALUE_NULL
} json_value_type;

typedef struct json_value json_value;

typedef struct {
  char *key;
  json_value *value;
} json_member;

struct json_value {
  json_value_type type;

  union {
    struct {
      json_member *members;
      u64 count;
    } object;

    struct {
      json_value **items;
      u64 count;
    } array;

    struct {
      const char *chars;
      u64 length;

    } string;

    f64 number;
    b8 boolean;
  } as;
};

json_value *json_new_value(mem_arena *arena, json_value_type type);
json_value *builder_string(mem_arena *arena, const token *t);
json_value *builder_number(mem_arena *arena, const token *t);
json_value *builder_bool(mem_arena *arena, b32 b);
json_value *builder_null(mem_arena *arena);
