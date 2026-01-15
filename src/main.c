#include "arena.h"
#include "base.h"
#include "lexer.h"
#include <stdio.h>

i32 main(i32 argc, char **argsv) {
  if (argc != 2) {
    fprintf(stderr, "You are either missing, "
                    "or have to many arguments\n");
    return 1;
  }

  mem_arena *perm_arena = arena_create(GiB(1), MiB(1));

  char *file_name = argsv[1];
  i64 file_size;

  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error with %s file\n", file_name);
    return 1;
  }

  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  if (file_size == -1) {
    perror("Error getting file size");
    return 1;
  }

  rewind(file);

  char *file_txt = PUSH_ARRAY(perm_arena, char, file_size);

  i64 bytes_read = fread(file_txt, 1, file_size, file);
  if (bytes_read != file_size) {
    fprintf(stderr, "Error reading file\n");
    return 1;
  }

  lexer lx;
  lexer_init(&lx, file_txt, file_size);

  token token;

  do {
    token = lexer_next_token(&lx);
  } while (token.type != TOKEN_EOF);

  // TODO: AST

  fclose(file);

  arena_destroy(perm_arena);
  return 0;
}
