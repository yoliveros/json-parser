#define ARENA_BASE_POS (sizeof(arena))
#define ARENA_ALIGN (sizeof(void *))

typedef struct {
  u64 reserve_size;
  u64 commit_size;

  u64 pos;
  u64 commit_pos;
} arena;
