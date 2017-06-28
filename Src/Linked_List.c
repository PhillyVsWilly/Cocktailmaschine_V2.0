#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Linked_List.h"

void list_new(linked_list *list)
{

  list->logicalLength = 0;
  list->elementSize = NODE_SIZE;
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
      list->freeFn(current->ingredient);
    }

    free(current->ingredient);
    free(current);
  }
}

void list_prepend(linked_list *list, ingredient_t ingredient)
{
  listNode *node = malloc(NODE_SIZE);


  //node->data = data;
  memcpy(node->ls_data, *data, NODE_SIZE);

  node->next = list->head;
  list->head = node;

  // first node?
  if(!list->tail) {
    list->tail = list->head;
  }

  list->logicalLength++;
}

void list_append(linked_list *list, int ls_data)
{
  listNode *node = malloc(sizeof(listNode));

  node->ls_data = ls_data;

  node->next = NULL;

  memcpy(node->ls_data, *ls_data, list->elementSize);

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
    result = iterator(node->ls_data);
    node = node->next;
  }
}

void list_head(linked_list *list, void *element, listbool removeFromList)
{
  if (list->head == NULL) {
	  return;
  }

  listNode *node = list->head;
  memcpy(element, node->ls_data, list->elementSize);

  if(removeFromList) {
    list->head = node->next;
    list->logicalLength--;

    free((node->ls_data));
    free(node);
  }
}

void list_tail(linked_list *list, void *element)
{
  assert(list->tail != NULL);
  listNode *node = list->tail;
  memcpy(element, &(node->ls_data), list->elementSize);
}

int list_size(linked_list *list)
{
  return list->logicalLength;
}

void free_int(int data)
{
  free(data);
}
