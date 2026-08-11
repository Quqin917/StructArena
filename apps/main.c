//  Copyright (c) 2025 Quqin

#include <stdio.h>

#include "arena.h"
#include "linked.h"

Arena my_arena = {0};

int main (void)
{
  printf("Arena Size Before: %ld\n", arenaRemainingSpace(&my_arena));

  linkedList list = createLinked();

  for (size_t i = 0; i < 5000000; i++)
  {
    appendTailLinked(&list, i);
  }

  long long total_sum = 0;
  node *curr = list.head_;

  while (curr != NULL)
  {
    total_sum += curr->data_;
    curr = curr->next_;
  }

  printf("Arena Size After: %ld\n", arenaRemainingSpace(&my_arena));
  printf("Benchmark complete. Total sum: %lld\n", total_sum);

  arenaFree(&my_arena);
  return 0;
}
