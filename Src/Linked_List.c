#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Linked_List.h"

void list_new(linked_list *list, int elementSize)
{
  assert(elementSize > 0);
  list->logicalLength = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->freeFn = free_int;
}

void list_destroy(linked_list *list)
{
  listNode *current;
  while(list->head != NULL) {
    current = list->head;
    list->head = current->next;

    if(list->freeFn) {
      list->freeFn(current->data);
    }

    free(current->data);
    free(current);
  }
}

void list_prepend(linked_list *list, int data[3])
{
  listNode *node = malloc(sizeof(listNode));
  for (int i=0; i<3; i++){
	  node->data[i] = data[i];
  }

  //node->data = data;
  memcpy(node->data, *data, list->elementSize);

  node->next = list->head;
  list->head = node;

  // first node?
  if(!list->tail) {
    list->tail = list->head;
  }

  list->logicalLength++;
}

void list_append(linked_list *list, int data[3])
{
  listNode *node = malloc(sizeof(listNode));
  for (int i=0; i<3; i++){
	  node->data[i] = data[i];
  }
  node->next = NULL;

  memcpy(node->data, *data, list->elementSize);

  if(list->logicalLength == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  list->logicalLength++;
}

void list_for_each(linked_list *list, listIterator iterator)
{
  assert(iterator != NULL);

  listNode *node = list->head;
  listbool result = TRUE;
  while(node != NULL && result) {
    result = iterator(node->data);
    node = node->next;
  }
}

void list_head(linked_list *list, void *element, listbool removeFromList)
{
  if (list->head == NULL) {
	  return;
  }

  listNode *node = list->head;
  memcpy(element, node->data, list->elementSize);

  if(removeFromList) {
    list->head = node->next;
    list->logicalLength--;

    free(node->data);
    free(node);
  }
}

void list_tail(linked_list *list, void *element)
{
  assert(list->tail != NULL);
  listNode *node = list->tail;
  memcpy(element, node->data, list->elementSize);
}

int list_size(linked_list *list)
{
  return list->logicalLength;
}

void free_int(int data[])
{
  free(data);
}
