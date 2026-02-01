#include "node.h"
#include "arena.h"

#include <stddef.h>
#include <stdint.h>

extern Arena arena;

void *createNode (intptr_t data)
{
  node *n = (node *)arenaAllocation(&arena, sizeof(node));
  if (!n) return NULL;

  n->data_ = data;
  n->next_ = NULL;

  return n;
}
