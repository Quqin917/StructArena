//  Copyright (c) 2025 Quqin

#include <stdio.h>

#include "arena.h"
#include "linked.h"

int main (void)
{
  Arena arena = {0};

  printf("Arena Size Before: %ld\n", arenaRemainingSpace(&arena));

  linkedList list = createLinked();

  for (size_t i = 0; i < 50000000; i++)
  {
    appendHeadLinked(&arena, &list, i);
  }

  long long total_sum = 0;

  node *curr = list.head_;
  while (curr != NULL)
  {
    total_sum += curr->data_;
    curr = curr->next_;
  }

  printf("Arena Size After: %ld\n", arenaRemainingSpace(&arena));
  printf("Total sum: %lld\n", total_sum);

  arenaFree(&arena);
  return 0;
}
