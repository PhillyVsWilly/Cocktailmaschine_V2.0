#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Linked_List.h"

void list_new(linked_list *list)
{

  list->logicalLength = 0;
  //list->elementSize = NODE_SIZE;
  list->head = list->tail = NULL;
}

void list_destroy(linked_list *list)
{
  listNode *current;
  while(list->head != NULL) {
    current = list->head;
    list->head = current->next;
    free(current->next);
    free(current);
  }
}

void list_prepend(linked_list *list, ingredient_t ingredient)
{
  listNode *node = malloc(sizeof(listNode) + sizeof(void*));
  node->next = malloc(sizeof(void*));
  node->ingredient = ingredient;

  node->next = list->head;
  list->head = node;

  // first node?
  if(!list->tail) {
    list->tail = list->head;
  }

  list->logicalLength++;
}

void list_append(linked_list *list, ingredient_t ingredient)
{
  listNode *node = malloc(sizeof(listNode) + sizeof(void*));
  node->next = malloc(sizeof(void*));
  node->ingredient = ingredient;

  node->next = NULL;


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
    result = iterator(node->ingredient);
    node = node->next;
  }
}

void list_head(linked_list *list, listNode *currentNode, listbool removeFromList)
{
  if (list->head == NULL) {
	  return;
  }

  currentNode = list->head;

  if(removeFromList) {
    list->head = currentNode->next;
    list->logicalLength--;
    free(currentNode->next);
    free(currentNode);
  }
}

void list_tail(linked_list *list, listNode *node)
{
  assert(list->tail != NULL);
  node = list->tail;
}

int list_size(linked_list *list)
{
  return list->logicalLength;
}
