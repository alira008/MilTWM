#include "queue.h"
#include <limits.h>
#include <stdlib.h>

static inline int queue_full(Queue *queue) {
  return queue->size >= queue->capacity;
}

Queue *queue_new(size_t capacity) {
  if (capacity > ULLONG_MAX / sizeof(void *)) {
    return NULL;
  }

  Queue *q = malloc(sizeof(Queue));

  q->capacity = capacity;
  q->size = 0;
  q->front = 0;
  q->data = malloc(capacity * sizeof(void *));

  if (!q || !q->data) {
    free(q->data);
    free(q);
    return NULL;
  }

  return q;
}

void queue_free(Queue *queue) {
  free(queue->data);
  free(queue);
}

int queue_add(Queue *queue, void *item) {
  if (queue_full(queue)) {
    return -1;
  }

  int pos = queue->front + queue->size;
  if (pos >= queue->capacity) {
    pos -= queue->capacity;
  }

  queue->data[pos] = item;
  ++queue->size;

  return 0;
}

void *queue_remove(Queue *queue) {
  if (queue->size == 0) {
    return NULL;
  }

  void *item = queue->data[queue->front];

  if (++queue->front >= queue->capacity) {
    queue->front -= queue->capacity;
  }
  --queue->size;

  return item;
}

void *queue_peek(Queue *queue) {
  return queue->size == 0 ? NULL : queue->data[queue->front];
}
