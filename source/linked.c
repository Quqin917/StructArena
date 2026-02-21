#include "linked.h"
#include "node.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

linkedList _createLinked (dataType type)
{
  linkedList l;

  l.head_ = NULL;
  l.tail_ = NULL;
  l.type_ = type;
  l.size_ = 0;

  return l;
}

bool appendHeadLinked (linkedList *src, intptr_t data_)
{
  if (!src) return false;

  node *n = (node *)createNode(data_);
  if (!n) return false;

  if (src->tail_ == NULL)
  {
    src->tail_ = n;
    src->head_ = src->tail_;
  }
  else
  {
    n->next_ = src->head_;
    src->head_ = n;
  }

  src->size_ += 1;
  return true;
}

bool appendTailLinked (linkedList *src, intptr_t data_)
{
  if (!src) return false;

  node *n = (node *)createNode(data_);
  if (!n) return false;

  if (src->tail_ == NULL)
  {
    src->tail_ = n;
    src->head_ = src->tail_;
  }
  else
  {
    src->tail_->next_ = n;
    src->tail_ = n;
  }

  src->size_ += 1;
  return true;
}

void printLinkedList (linkedList *src)
{
  if (!src) return;

  for (node *cur = src->head_; cur; cur = cur->next_)
  {
    printf("%ld\n", (long)cur->data_);
  }
}

#include <assert.h>

bool insertGivenPos (linkedList *src, intptr_t data_, size_t pos)
{
  if (!src) return false;
  if (pos > src->size_) return false;

  if (pos == 0)
    return appendHeadLinked(src, data_);
  else if (pos == src->size_)
    return appendTailLinked(src, data_);

  node *n = (node *)createNode(data_);
  if (!n) return false;

  node *prev = NULL;
  node *curr = src->head_;

  for (size_t i = 0; i < pos; ++i)
  {
    prev = curr;
    curr = curr->next_;
  }

  n->next_ = curr;
  prev->next_ = n;
  src->size_ += 1;

  return true;
}
