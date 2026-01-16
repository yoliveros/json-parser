#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>

static json_value *parse_object(mem_arena *arena, lexer *lx) {
  json_value *obj = json_new_value(arena, JSON_VALUE_OBJECT);

  token t = lexer_next_token(lx);
  if (t.type == TOKEN_RBRACE) {
    obj->as.object.members = NULL;
    obj->as.object.count = 0;
    return obj;
  }

  u64 capacity = 8;
  json_member *members = PUSH_ARRAY(arena, json_member, capacity);
  u64 count = 0;

  do {
    if (t.type != TOKEN_STRING)
      return NULL;

    members[count].key = PUSH_ARRAY(arena, char, t.length);
    memcpy(members[count].key, t.lexeme, t.length);

    t = lexer_next_token(lx);
    if (t.type != TOKEN_COLON)
      return NULL;

    members[count].value = parse_value(arena, lx);
    if (!members[count].value)
      return NULL;

    count++;

    t = lexer_next_token(lx);
    if (t.type == TOKEN_RBRACE)
      break;
    else if (t.type != TOKEN_COMMA)
      return NULL;

    t = lexer_next_token(lx);
  } while (1);

  obj->as.object.members = members;
  obj->as.object.count = count;
  return obj;
}

static json_value *parse_array(mem_arena *arena, lexer *lx) {
  json_value *arr = json_new_value(arena, JSON_VALUE_ARRAY);

  u64 capacity = 8;
  json_value **items = PUSH_ARRAY(arena, json_value *, capacity);
  u64 count = 0;

  token t = {0};

  do {
    items[count] = parse_value(arena, lx);
    if (!items[count])
      return NULL;

    count++;

    t = lexer_next_token(lx);
    if (t.type == TOKEN_RBRACKET)
      break;
    if (t.type != TOKEN_COMMA)
      return NULL;
  } while (1);

  arr->as.array.items = items;
  arr->as.array.count = count;
  return arr;
}

json_value *parse_value(mem_arena *arena, lexer *lx) {
  token t = lexer_next_token(lx);
  switch (t.type) {
  case TOKEN_LBRACE:
    return parse_object(arena, lx);
  case TOKEN_LBRACKET:
    return parse_array(arena, lx);
  case TOKEN_STRING:
    return builder_string(arena, &t);
  case TOKEN_NUMBER:
    return builder_number(arena, &t);
  case TOKEN_TRUE:
    return builder_bool(arena, 1);
  case TOKEN_FALSE:
    return builder_bool(arena, 0);
  case TOKEN_NULL:
    return builder_null(arena);
  case TOKEN_ERROR:
  case TOKEN_EOF:
  default:
    return NULL;
  }
}

void print_json_value(json_value *value, i32 indent) {
  if (!value)
    return;

  for (i32 i = 0; i < indent; i++) {
    printf("  ");
  }

  switch (value->type) {
  case JSON_VALUE_OBJECT:
    printf("{\n");
    for (u64 i = 0; i < value->as.object.count; i++) {
      for (i32 j = 0; j < indent + 1; j++) {
        printf("  ");
      }
      printf("\"%s\": ", value->as.object.members[i].key);

      print_json_value(value->as.object.members[i].value, indent + 1);

      if (i < value->as.object.count - 1)
        printf(",");

      printf("\n");
    }
    for (i32 i = 0; i < indent; i++)
      printf("  ");

    printf("}");
    break;
  case JSON_VALUE_ARRAY:
    printf("[\n");
    for (u64 i = 0; i < value->as.array.count; i++) {
      for (i32 j = 0; j < indent + 1; j++) {
        printf("  ");
      }

      print_json_value(value->as.array.items[i], indent + 1);

      if (i < value->as.array.count - 1)
        printf(",");

      printf("\n");
    }
    for (i32 i = 0; i < indent; i++)
      printf("  ");

    printf("]");
    break;
  case JSON_VALUE_STRING:
    printf("\"%.*s\"", (i32)value->as.string.length, value->as.string.chars);
    break;
  case JSON_VALUE_NUMBER:
    printf("%f", value->as.number);
    break;
  case JSON_VALUE_BOOL:
    printf("%s", value->as.boolean ? "true" : "false");
    break;
  case JSON_VALUE_NULL:
    printf("null");
    break;
  }
}
