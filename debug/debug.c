#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "arena.h"

typedef struct Node
{
  intptr_t data_;
  struct Node *next_;
} Node;

Arena arena = {0};

static volatile intptr_t sink;

static inline double now_sec(void);
static double test_malloc_free(size_t N);
static double test_arena(size_t N);

int main (void)
{
  const size_t N = 1000000;
  const int runs = 7;

  double best_m = 1e9, best_a = 1e9;

  for (int r = 0; r < runs; ++r)
  {
    double tm = test_malloc_free(N);
    if (tm < best_m) best_m = tm;

    double ta = test_arena(N);
    if (ta < best_a) best_a = ta;
  }

  printf("N=%zu (best of %d)\n", N, runs);
  printf("malloc/free: %.6f sec\n", best_m);
  printf("arena:    %.6f sec\n", best_a);
  printf("speedup:   %.2fx\n\n", best_m / best_a);

  printf("Arena Stats:\n");
  printf("  Blocks created: %d\n", arena.block_count);
  printf("  Total OS memory: %zu bytes\n", arena.total_allocated_to_system);

  arenaFree(&arena);
}

static inline double now_sec (void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

static double test_malloc_free (size_t N)
{
  double t0 = now_sec();

  Node *head = NULL, *tail = NULL;
  for (size_t i = 0; i < N; ++i)
  {
    Node *n = (Node *)malloc(sizeof(*n));
    if (!n) exit(1);

    n->data_ = (intptr_t)i;
    n->next_ = NULL;

    if (!tail) { head = tail = n; }
    else
    {
      tail->next_ = n;
      tail = n;
    }
  }

  intptr_t sum = 0;
  for (Node *c = head; c; c = c->next_)
    sum += c->data_;
  sink = sum;

  for (Node *c = head; c;)
  {
    Node *nx = c->next_;
    free(c);
    c = nx;
  }

  return now_sec() - t0;
}

static double test_arena (size_t N)
{
  arenaReset(&arena);

  double t0 = now_sec();

  Node *head = NULL, *tail = NULL;
  for (size_t i = 0; i < N; ++i)
  {
    Node *n = (Node *)arenaAllocation(&arena, sizeof(*n));
    if (!n) exit(1);

    n->data_ = (intptr_t)i;
    n->next_ = NULL;

    if (!tail)
      head = tail = n;
    else
    {
      tail->next_ = n;
      tail = n;
    }
  }

  intptr_t sum = 0;
  for (Node *c = head; c; c = c->next_)
    sum += c->data_;
  sink = sum;

  return now_sec() - t0;
}
