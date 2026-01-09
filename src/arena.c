#include "arena.h"
#include <stdlib.h>

#if defined(__linux__)

#include <sys/mman.h>
#include <unistd.h>

mem_arena *arena_create(u64 reserve_size, u64 commit_size) {
  u32 pagesize = plat_get_pagesize();
  // u32 page_size = mmap(NULL, capacity, PROT_READ | PROT_WRITE,
  // MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  mem_arena *arena = (mem_arena *)arena->capacity = capacity;
  arena->pos = ARENA_BASE_POS;

  return arena;
}

void arena_destroy(mem_arena *arena) { munmap(arena, arena->capacity); }

void *arena_push(mem_arena *arena, u64 size) {
  u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
  u64 new_pos = pos_aligned + size;

  if (new_pos > arena->capacity) {
    return NULL;
    exit(1);
  }

  arena->pos = new_pos;

  u8 *out = (u8 *)arena + pos_aligned;
  return out;
}

void arena_pop(mem_arena *arena, u64 size) {
  size = MIN(size, arena->pos - ARENA_BASE_POS);
  arena->pos -= size;
}

void arena_pop_to(mem_arena *arena, u64 pos) {
  u64 size = pos < arena->pos ? arena->pos - pos : 0;
  arena_pop(arena, size);
}

void arena_clear(mem_arena *arena) { arena_pop_to(arena, ARENA_BASE_POS); }

u32 plat_get_pagesize(void) { return (u32)sysconf(_SC_PAGESIZE); }

#endif
