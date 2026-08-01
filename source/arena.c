//  Copyright (c) 2025 Quqin

#include "arena.h"
#include <stdint.h>

#define MEGABYTE_SIZE (1024u * 1024u)
#define ARENA_DEFAULT_SIZE (MEGABYTE_SIZE * 2u) // 2 MB

#include <stdlib.h> // For malloc() and free()

struct region
{
  struct region *next_;
  size_t capacity_;
  size_t count_;
  intptr_t data_[];
};

Region *createRegion (size_t capacity)
{
  if (capacity == 0) return NULL;

  // Overflow guard
  if (capacity > (SIZE_MAX - sizeof(Region)) / sizeof(uintptr_t)) return NULL;

  size_t bytes = sizeof(Region) + sizeof(uintptr_t) * capacity;

  Region *region = (Region *)malloc(bytes);
  if (!region) return NULL;

  region->next_ = NULL;
  region->capacity_ = capacity;
  region->count_ = 0;

  return region;
}

void *arenaAllocation (Arena *arena, size_t size_in_bytes)
{
  if (!arena) return NULL;
  if (size_in_bytes == 0) return NULL;

  size_t words = (size_in_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
  if (words == 0) return NULL;

  // The first initialization of arena
  if (!arena->tail_)
  {
    ARENA_ASSERT(arena->head_ == NULL);
    arena->tail_ = createRegion(words);

    arena->head_ = arena->tail_;
    arena->block_count++;
    arena->total_allocated_to_system += (words * sizeof(uintptr_t));
  }

  // Move to the last region
  while (arena->tail_->count_ + words > arena->tail_->capacity_
         && arena->tail_->next_)
  {
    arena->tail_ = arena->tail_->next_;
  }

  // Append new region to existing arena
  if (arena->tail_->count_ + words > arena->tail_->capacity_)
  {
    ARENA_ASSERT(arena->tail_->next_ == NULL);

    size_t cap = (words > ARENA_DEFAULT_SIZE) ? words : ARENA_DEFAULT_SIZE;
    Region *r = createRegion(cap);
    if (!r) return NULL;

    arena->tail_->next_ = r;
    arena->tail_ = r;

    arena->block_count++;
    arena->total_allocated_to_system += (cap * sizeof(uintptr_t));
  }

  void *result = &arena->tail_->data_[arena->tail_->count_];
  arena->tail_->count_ += words;

  return result;
}

void *
arenaRealloc (Arena *arena, void *oldptr, size_t old_size, size_t new_size)
{
  if (new_size <= old_size) return oldptr;

  void *new_ptr = arenaAllocation(arena, new_size);

  char *old_ptr_byte = (char *)oldptr;
  char *new_ptr_byte = (char *)new_ptr;

  for (size_t i = 0; i < old_size; ++i)
    new_ptr_byte[i] = old_ptr_byte[i];

  return new_ptr;
}

void arenaFree (Arena *r)
{
  if (!r) return;

  Region *curr = r->head_;
  while (curr)
  {
    Region *next = curr->next_;
    free(curr);
    curr = next;
  }

  r->head_ = NULL;
  r->tail_ = NULL;
}

void arenaReset (Arena *a)
{
  if (!a || !a->head_) return;

  a->tail_ = a->head_;

  Region *curr = a->head_;
  while (curr)
  {
    curr->count_ = 0;
    curr = curr->next_;
  }
}

void *arenaMemcpy (void *dest, const void *src, size_t n)
{
  // Assigned data types to void
  char *d = dest;
  const char *s = src;

  // Copy every byte of src to dest
  for (; n; n--)
    *d++ = *s++;

  return dest;
}

void *regionAt (void *data, size_t byte_offset)
{ return (void *)((char *)data + byte_offset); }

void *arenaAt (Arena *arena, size_t offset, size_t item_size)
{
  Region *current = arena->head_;

  while (current != NULL)
  {
    size_t bytes_in_region = current->count_ * sizeof(uintptr_t);
    size_t iterator_region = offset * item_size;

    if (iterator_region < bytes_in_region)
    {
      return regionAt(current->data_, iterator_region);
    }

    size_t item_in_region = bytes_in_region / item_size;
    offset -= item_in_region;

    current = current->next_;
  }

  return NULL;
}

size_t arenaRemainingSpace (Arena *arena)
{
  if (!arena || !arena->tail_) { return 0; }

  return arena->tail_->capacity_ - arena->tail_->count_;
}
