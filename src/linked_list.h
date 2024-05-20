#pragma once

typedef struct Node Node;

typedef struct DoubleyLinkedList {
  size_t length;
  Node *head;
} DoubleyLinkedList;

DoubleyLinkedList doubley_linked_list_init();
void doubley_linked_list_push_front(void* data);
void doubley_linked_list_push_back(void* data);
void doubley_linked_list_pop_front();
void doubley_linked_list_pop_back();
void doubley_linked_list_destroy();
