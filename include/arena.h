#ifndef ARENA_H
#define ARENA_H

#include <stddef.h> // For size_t
#include <stdint.h> // For intptr_t

#ifndef ARENA_ASSERT
#include <assert.h> // for assert()
#define ARENA_ASSERT assert
#endif // !ARENA_ASSERT

typedef struct region Region;

Region *createRegion(size_t capacity);
void freeRegion(Region *a);

typedef struct
{
  Region *head_;
  Region *tail_;
  int initialized_;
} Arena;

void *arenaAllocation(Arena *arena, size_t size);
void *arenaMemcpy(void *dest, const void *src, size_t n);

void *
arenaRealloc(Arena *arena, void *oldptr, size_t old_size, size_t new_size);

void *arenaAt(Arena *arena, size_t iterator, size_t item_size);

void freeArena(Arena *arena);

#endif // !ARENA_H
