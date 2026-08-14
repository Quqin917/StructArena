#ifndef NODE_H
#define NODE_H

#include <stddef.h>
#include <stdint.h>

#include "arena.h"

typedef struct node node;

struct node
{
  struct node *next_;
  int data_;
};

void *createNode(Arena *arena, int data);

#endif // !NODE_H
