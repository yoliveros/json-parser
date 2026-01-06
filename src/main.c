#include "base.h"
#include <stdbool.h>
#include <stdio.h>

i32 main(i32 argc, char **argsv) {
  if (argc != 2) {
    fprintf(stderr, "You are either missing, "
                    "or have to many arguments\n");
    return 1;
  }

  char *file_name = argsv[1];

  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error with %s file\n", file_name);
    return 1;
  }

  i32 c;
  while ((c = fgetc(file)) != EOF) {
    putchar(c);
  }

  fclose(file);
  return 0;
}
