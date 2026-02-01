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

typedef enum
{
  SUCCESS = 0,
  ERROR_TYPE_MISMATCH = 1,
  ERROR_MEMORY_ALLOCATION_FAILED = 2,
} ErrorCode;

linkedList _createLinked(dataType type);

bool appendHeadLinked(linkedList *src, intptr_t data);
bool appendTailLinked(linkedList *src, intptr_t data);
bool insertGivenPos(linkedList *src, intptr_t data, size_t pos);

void printLinkedList(linkedList *src);

#endif // !LINKED_H
