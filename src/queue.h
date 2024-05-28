#pragma once

typedef struct Queue {
  size_t size;
  size_t front;
  size_t capacity;
  void **data;
} Queue;


Queue* queue_new(size_t capacity);
void queue_free(Queue* queue);
int queue_add(Queue* queue, void* item);
void* queue_remove(Queue* queue);
void* queue_peek(Queue* queue);
