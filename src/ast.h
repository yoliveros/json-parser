#include "base.h"

typedef enum {
  JSON_VALUE_OBJECT,
  JSON_VALUE_ARRAY,
  JSON_VALuE_STRING,
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

    char *string;
    f64 number;
    b8 boolean;
  } as;
};
