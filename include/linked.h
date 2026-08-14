#ifndef LINKED_H
#define LINKED_H

#include <stddef.h> // For size_t
#include <stdint.h>
#include <stdbool.h>

#include "node.h" // For node

typedef struct
{
  node *head_;
  node *tail_;
  size_t size_;
} linkedList;

linkedList createLinked(void);

bool appendHeadLinked(linkedList *src, int data);
bool appendTailLinked(linkedList *src, int data);
bool insertGivenPos(linkedList *src, int data, size_t pos);

void printLinkedList(linkedList *src);

#endif // !LINKED_H
