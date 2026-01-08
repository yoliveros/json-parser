#include "arena.h"

#if defined(__linux__)

#include <sys/mman.h>
#include <unistd.h>

mem_arena *arena_create(u64 capacity) {
  mem_arena arena = (mem_arena *)malloc(capacity);
}

#endif
