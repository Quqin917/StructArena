#ifndef LINKED_H
#define LINKED_H

#include <stddef.h> // For size_t
#include <stdint.h>
#include <stdbool.h>

#include "node.h" // For node

typedef enum
{
  INT,
  CHAR,
  STRING,
  DOUBLE,
} dataType;

typedef struct
{
  node *head_;
  node *tail_;
  dataType type_;
  size_t size_;
} linkedList;

linkedList createLinked(dataType type);

bool appendHeadLinked(linkedList *src, intptr_t data);
bool appendTailLinked(linkedList *src, intptr_t data);
bool insertGivenPos(linkedList *src, intptr_t data, size_t pos);

void printLinkedList(linkedList *src);

#endif // !LINKED_H
