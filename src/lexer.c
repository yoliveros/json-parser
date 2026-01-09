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
