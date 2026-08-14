#ifndef NODE_H
#define NODE_H

#include <stddef.h>
#include <stdint.h>

typedef struct node node;

struct node
{
  struct node *next_;
  int data_;
};

void *createNode(int data);

#endif // !NODE_H
