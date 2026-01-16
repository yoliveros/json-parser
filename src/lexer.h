#pragma once

#include "arena.h"
#include "base.h"

typedef enum {
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_NULL,
  TOKEN_EOF,
  TOKEN_ERROR
} token_type;

typedef struct {
  token_type type;
  const char *lexeme;
  i32 length;
  i32 line;
  i32 column;
} token;

typedef struct {
  const char *buffer;
  u64 buffer_length;
  u64 position;
  i32 line;
  i32 column;
} lexer;

void lexer_init(lexer *lexer, const char *buffer, u64 length);
token lexer_next_token(lexer *lexer);
