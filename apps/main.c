//  Copyright (c) 2025 Quqin

#include <stdio.h>

#include "arena.h"
#include "linked.h"

Arena arena = {0};

int main (void)
{
  linkedList linked = createLinked(INT);

  if (!appendHeadLinked(&linked, 1))
  {
    fprintf(stderr, "Out of memory\n");
    freeArena(&arena);
    return 1;
  };

  if (!appendTailLinked(&linked, 3))
  {
    fprintf(stderr, "Out of memory\n");
    freeArena(&arena);
    return 2;
  };

  if (!insertGivenPos(&linked, 2, 1))
  {
    fprintf(stderr, "Out of memory\n");
    freeArena(&arena);
    return 3;
  };

  printLinkedList(&linked);

  printf("size of linked list: %zu\n", linked.size_);
  printf("types of data: %d\n", (int)linked.type_);

  freeArena(&arena);
  return 0;
}
