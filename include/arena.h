#ifndef ARENA_H
#define ARENA_H

#include <stddef.h> // For size_t

#ifndef ARENA_ASSERT
#include <assert.h> // for assert()
#define ARENA_ASSERT assert
#endif // !ARENA_ASSERT

#define MEGABYTE_SIZE (1024u * 1024u)

typedef struct region Region;

typedef struct
{
  Region *head_;
  Region *tail_;
  int block_count;
  size_t total_allocated_to_system;
} Arena;

void *arenaAllocation(Arena *arena, size_t size);
void *arenaMemcpy(void *dest, const void *src, size_t n);

void *
arenaRealloc(Arena *arena, void *oldptr, size_t old_size, size_t new_size);

void *arenaAt(Arena *arena, size_t iterator, size_t item_size);

size_t arenaRemainingSpace(Arena *arena);

void arenaFree(Arena *arena);
void arenaReset(Arena *arena);

#endif // !ARENA_H
