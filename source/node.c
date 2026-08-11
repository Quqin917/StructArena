#include "node.h"
#include "arena.h"

#include <stddef.h>
#include <stdint.h>

extern Arena my_arena;

void *createNode (int data)
{
  node *n = (node *)arenaAllocation(&my_arena, sizeof(node));
  if (!n) return NULL;

  n->data_ = data;
  n->next_ = NULL;

  return n;
}
