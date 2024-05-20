#include "linked_list.h"
#include <stddef.h>

struct Node {
  void *data;
  Node *next;
  Node *prev;
};

DoubleyLinkedList doubley_linked_list_init() {
  return (DoubleyLinkedList){.length = 0, .head = NULL};
}
void doubley_linked_list_push_front(void *data);
void doubley_linked_list_push_back(void *data);
void doubley_linked_list_pop_front();
void doubley_linked_list_pop_back();
void doubley_linked_list_destroy();
