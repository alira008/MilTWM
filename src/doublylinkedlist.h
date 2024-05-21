#pragma once

#define DEQUE_INIT_CAPACITY 256
typedef struct Node Node;

// typedef struct {
//   void **arr;
//   size_t size_of_data;
//   size_t length;
//   size_t cap;
//   size_t front;
//   size_t rear;
// } Deque;
//
// Deque *deque_init(size_t size_of_data);
// void deque_push_front(Deque *deque, void *data);
// void deque_push_back(Deque *deque, void *data);

typedef struct {
  Node *head;
  size_t length;
} DoublyLinkedList;

DoublyLinkedList *doubly_linked_list_init(void);
void doubly_linked_list_push_front(DoublyLinkedList *list, void *data);
void doubly_linked_list_push_back(DoublyLinkedList *list, void *data);
void *doubly_linked_list_pop_front(DoublyLinkedList *list);
void *doubly_linked_list_pop_back(DoublyLinkedList *list);
void doubly_linked_list_destroy(DoublyLinkedList *list);
void *doubly_linked_list_element_at(DoublyLinkedList *list, size_t loc);
