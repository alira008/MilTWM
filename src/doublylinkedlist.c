#include "doublylinkedlist.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
  void *data;
  Node *next;
  Node *prev;
};

// Deque *deque_init(size_t size_of_data) {
//   Deque *d = malloc(sizeof(Deque));
//   assert(d != NULL);
//   d->arr = malloc(size_of_data * DEQUE_INIT_CAPACITY);
//   d->front = 0;
//   d->rear = 0;
//   d->length = 0;
//   d->cap = DEQUE_INIT_CAPACITY;
//   d->size_of_data = size_of_data;
//   return d;
// }
//
// static inline bool deque_empty(Deque *deque) { return deque->length == 0; }
//
// static inline bool deque_full(Deque *deque) {
//   return deque->length == deque->cap;
// }
//
// void deque_push_front(Deque *deque, void *data) {
//   if (deque_full(deque)) {
//     deque->cap = deque->cap * 2;
//     deque->arr = realloc(deque->arr, deque->size_of_data * deque->cap);
//   }
//
//   if (!deque_empty(deque) && deque->front == 0) {
//     deque->front = deque->cap - 1;
//   } else if (deque->front > 0) {
//     deque->front--;
//   }
//
//   deque->arr[deque->front] = data;
//   deque->length++;
// }
//
// void deque_push_back(Deque *deque, void *data) {
//   if (deque_full(deque)) {
//     deque->cap = deque->cap * 2;
//     deque->arr = realloc(deque->arr, deque->size_of_data * deque->cap);
//   }
//
//   if (deque->rear > 0) {
//     deque->rear++;
//   } else if (deque->front == 0) {
//     deque->rear++;
//   }
//
//   deque->arr[deque->rear] = data;
//   deque->length++;
// }

// #define TRAVERSE_TO_LAST_ELEMENT(deque, current)                               \
//   do {                                                                         \
//     for (size_t i = 0; i < deque->length && current && current->next; ++i) {   \
//       current = current->next;                                                 \
//     }                                                                          \
//   } while (0)

DoublyLinkedList *doubly_linked_list_init(void) {
  DoublyLinkedList *d = malloc(sizeof(DoublyLinkedList));
  assert(d != NULL);
  d->head = NULL;
  d->length = 0;
  return d;
}

void doubly_linked_list_push_front(DoublyLinkedList *list, void *data) {
  Node *new_node = malloc(sizeof(Node));
  assert(new_node != NULL);
  new_node->data = data;

  if (list->length == 1) {
    new_node->next = list->head;
    new_node->prev = list->head;
    list->head = new_node;
  } else if (list->length > 1) {
    new_node->next = list->head;
    new_node->prev = list->head->prev;
    list->head->prev = new_node;
    list->head = new_node;
  } else {
    new_node->next = new_node;
    new_node->prev = new_node;
    list->head = new_node;
  }

  list->length++;
}

void doubly_linked_list_push_back(DoublyLinkedList *list, void *data) {
  Node *new_node = malloc(sizeof(Node));
  assert(new_node != NULL);
  new_node->data = data;

  Node *current = list->head;

  if (list->length == 1) {
    new_node->next = list->head;
    new_node->prev = list->head;
    list->head->next = new_node;
    list->head->prev = new_node;
  } else if (list->length > 1) {
    Node *tail = list->head->prev;
    tail->next = new_node;
    new_node->next = list->head;
    new_node->prev = tail;
    list->head->prev = new_node;
  } else {
    list->head = new_node;
    new_node->next = new_node;
    new_node->prev = new_node;
  }

  list->length++;
}

void *doubly_linked_list_pop_front(DoublyLinkedList *list) {
  assert(!list);
  if (list->length == 0 || !list->head)
    return NULL;

  void *ret = list->head->data;

  if (list->length == 1) {
    free(list->head);
    list->head = NULL;
    list->length = 0;
    return ret;
  }

  Node *old_head = list->head;
  Node *new_head = old_head->next;
  new_head->prev = old_head->prev;
  list->head = new_head;
  list->length--;

  free(old_head);
  old_head = NULL;
  return ret;
}

void *doubly_linked_list_pop_back(DoublyLinkedList *list) {
  assert(!list);
  if (list->length == 0 || !list->head)
    return NULL;

  Node *old_tail = list->head->prev;
  void *ret = old_tail->data;

  if (list->length == 1) {
    free(list->head);
    list->head = NULL;
    list->length = 0;
    return ret;
  }

  Node *new_tail = old_tail->prev;
  new_tail->next = list->head;
  list->head->prev = new_tail;
  list->length--;

  free(old_tail);
  old_tail = NULL;
  return ret;
}

void doubly_linked_list_destroy(DoublyLinkedList *list) {
  assert(!list);

  if (list->length == 0 || !list->head) {
    list->head = NULL;
    free(list);
    list = NULL;
    return;
  }

  Node *current = list->head;
  for (size_t i = 0; i < list->length; ++i) {
    if (!current)
      break;
    Node *temp = current;
    current = current->next;
    free(temp);
    temp = NULL;
  }

  list->length = 0;
  list->head = NULL;
  free(list);
  list = NULL;
}

void *doubly_linked_list_element_at(DoublyLinkedList *list, size_t loc) {
  assert(!list);
  assert(loc < list->length);

  if (!list->head || list->length == 0)
    return NULL;

  void *ret = NULL;
  Node *current = list->head;
  for (size_t i = 0; i < list->length && i <= loc; ++i) {
    if (!current->next)
      break;

    if (i == loc)
      ret = current->data;

    current = current->next;
  }

  return ret;
}
