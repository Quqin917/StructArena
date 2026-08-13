#include "linked.h"
#include "node.h"

#include <stdio.h>

linkedList createLinked (void)
{
  linkedList l = {
    .head_ = NULL,
    .tail_ = NULL,
    .size_ = 0,
  };

  return l;
}

void appendHeadLinked (Arena *arena, linkedList *src, int data_)
{
  if (!src) return;

  node *n = (node *)createNode(arena, data_);
  if (!n) return;

  n->next_ = src->head_;
  src->head_ = n;

  src->size_ += 1;
}

void appendTailLinked (Arena *arena, linkedList *src, int data_)
{
  if (!src) return;

  node *n = (node *)createNode(arena, data_);
  if (!n) return;

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
}

void printLinkedList (linkedList *src)
{
  if (!src) return;

  node *cur = src->head_;
  while (cur)
  {
    printf("%d\n", cur->data_);
    cur = cur->next_;
  }
}

#include <assert.h>

void insertGivenPos (Arena *arena, linkedList *src, int data_, size_t pos)
{
  if (!src || pos > src->size_) return;

  if (pos == 0)
  {
    appendHeadLinked(arena, src, data_);
    return;
  }
  else if (pos == src->size_)
  {
    appendTailLinked(arena, src, data_);
    return;
  }

  node *n = (node *)createNode(arena, data_);
  if (!n) return;

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
}
