#include "node.h"

#include <stddef.h>

void *createNode (Arena *arena, int data)
{
  node *n = (node *)arenaAllocation(arena, sizeof(node));
  if (!n) return NULL;

  n->data_ = data;
  n->next_ = NULL;

  return n;
}
