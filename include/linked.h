#ifndef LINKED_H
#define LINKED_H

#include <stddef.h>

#include "node.h"
#include "arena.h"

typedef struct
{
  node *head_;
  node *tail_;
  size_t size_;
} linkedList;

linkedList createLinked(void);

void appendHeadLinked(Arena *arena, linkedList *src, int data);
void appendTailLinked(Arena *arena, linkedList *src, int data);
void insertGivenPos(Arena *arena, linkedList *src, int data, size_t pos);

void printLinkedList(linkedList *src);

#endif // !LINKED_H
