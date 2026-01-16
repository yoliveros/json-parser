#include "lexer.h"
#include <ctype.h>

void lexer_init(lexer *lx, const char *buffer, u64 length) {
  lx->buffer = buffer;
  lx->buffer_length = length;
  lx->position = 0;
  lx->line = 1;
  lx->column = 1;
}

static i32 lexer_at_end(lexer *lx) { return lx->position >= lx->buffer_length; }

static char lexer_peek(lexer *lx) { return lx->buffer[lx->position]; }

static char lexer_advance(lexer *lx) {
  char c = lexer_peek(lx);

  if (lexer_at_end(lx))
    return c;

  lx->position++;
  if (c == '\n') {
    lx->line++;
    lx->column = 1;
  } else {
    lx->column++;
  }

  return c;
}

static void lexer_skip_whitespace(lexer *lx) {
  while (isspace((u8)lexer_peek(lx))) {
    lexer_advance(lx);
  }
}

static token lexer_string(lexer *lx) {
  const char *start = lx->buffer + lx->position;
  i32 line = lx->line;
  i32 col = lx->column;

  while (!lexer_at_end(lx)) {
    char c = lexer_peek(lx);

    if (c == '"') {
      const char *end;
      end = lx->buffer + lx->position;
      lexer_advance(lx);
      token t;
      t.type = TOKEN_STRING;
      t.lexeme = start;
      t.length = (i32)(end - start);
      t.line = line;
      t.column = col;

      return t;
    } else if (c == '\n' || c == '\r' || c == '\0') {
      token t;
      t.type = TOKEN_ERROR;
      t.lexeme = start;
      t.length = (i32)(lx->buffer + lx->position - start);
      t.line = line;
      t.column = col;
      return t;
    } else {
      lexer_advance(lx);
    }
  }

  token t;
  t.type = TOKEN_ERROR;
  t.lexeme = start;
  t.length = (i32)(lx->buffer + lx->position - start);
  t.line = line;
  t.column = col;
  return t;
}

static b32 is_digit(char c) { return c >= '0' && c <= '9'; }

static token lexer_number(lexer *lx) {
  const char *start = lx->buffer + lx->position - 1;

  i32 line = lx->line;
  i32 col = lx->column;

  if (lexer_peek(lx) == '-') {
    lexer_advance(lx);
  }

  if (!is_digit(lx->buffer[lx->position - 1])) {
    token t = {0};
    t.type = TOKEN_ERROR;
    t.lexeme = start;
    t.length = 1;
    t.line = line;
    t.column = col;
    return t;
  }

  if (lexer_peek(lx) == '0') {
    lexer_advance(lx);
  } else {
    while (is_digit(lexer_peek(lx))) {
      lexer_advance(lx);
    }
  }

  if (lexer_peek(lx) == '.') {
    lexer_advance(lx);
    if (!is_digit(lexer_peek(lx))) {
      token t = {0};
      t.type = TOKEN_ERROR;
      t.lexeme = start;
      t.length = (i32)(lx->buffer + lx->position - start);
      t.line = line;
      t.column = col;
      return t;
    }
    while (is_digit(lexer_peek(lx))) {
      lexer_advance(lx);
    }
  }

  char c = lexer_peek(lx);
  if (c == 'e' || c == 'E') {
    lexer_advance(lx);
    c = lexer_peek(lx);
    if (c == '+' || c == '-') {
      lexer_advance(lx);
    }

    if (!is_digit(lexer_peek(lx))) {
      token t = {0};
      t.type = TOKEN_ERROR;
      t.lexeme = start;
      t.length = (i32)(lx->buffer + lx->position - start);
      t.line = line;
      t.column = col;
      return t;
    }
    while (is_digit(lexer_peek(lx))) {
      lexer_advance(lx);
    }
  }

  token t;
  t.type = TOKEN_NUMBER;
  t.lexeme = start;
  t.length = (i32)(lx->buffer + lx->position - start);
  t.line = line;
  t.column = col;
  return t;
}

static i32 match_literal(lexer *lx, const char *word) {
  u64 start = lx->position;
  while (*word != '\0') {
    if (lexer_at_end(lx)) {
      lx->position = start;
      return 0;
    }
    lexer_advance(lx);
    word++;
  }

  return 1;
}

token lexer_next_token(lexer *lx) {
  lexer_skip_whitespace(lx);

  token t = {0};

  if (lexer_at_end(lx)) {
    t.type = TOKEN_EOF;
    t.lexeme = lx->buffer + lx->position;
    t.length = 0;
    t.line = lx->line;
    t.column = lx->column;
    return t;
  }

  char c = lexer_advance(lx);
  const char *start = lx->buffer + (lx->position - 1);
  i32 line = lx->line;
  i32 col = lx->column - 1;

  switch (c) {
  case '{':
    t.type = TOKEN_LBRACE;
    break;
  case '}':
    t.type = TOKEN_RBRACE;
    break;
  case '[':
    t.type = TOKEN_LBRACKET;
    break;
  case ']':
    t.type = TOKEN_RBRACKET;
    break;
  case ':':
    t.type = TOKEN_COLON;
    break;
  case ',':
    t.type = TOKEN_COMMA;
    break;
  case '"':
    return lexer_string(lx);
    break;
  case 't':
    lx->position--;
    lx->column--;
    if (match_literal(lx, "true")) {
      t.type = TOKEN_TRUE;
      start = lx->buffer + (lx->position - 4);
      t.length = 4;
    } else {
      t.type = TOKEN_ERROR;
      t.length = 1;
    }
    break;
  case 'f':
    lx->position--;
    lx->column--;
    if (match_literal(lx, "false")) {
      t.type = TOKEN_FALSE;
      start = lx->buffer + (lx->position - 5);
      t.length = 5;
    } else {
      t.type = TOKEN_ERROR;
      t.length = 1;
    }
    break;
  case 'n':
    lx->position--;
    lx->column--;
    if (match_literal(lx, "null")) {
      t.type = TOKEN_NULL;
      start = lx->buffer + (lx->position - 4);
      t.length = 4;
    } else {
      t.type = TOKEN_ERROR;
      t.length = 1;
    }
    break;
  default:
    if (c == '-' || is_digit(c)) {
      return lexer_number(lx);
    } else {
      t.type = TOKEN_ERROR;
      t.length = 1;
    }
    break;
  }

  t.lexeme = start;
  if (t.length == 0) {
    t.length = (i32)(lx->buffer + lx->position - start);
  }
  t.line = line;
  t.column = col;
  return t;
}
