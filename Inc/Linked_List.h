/*
 * List
 *
 *  Created on: 25.06.2017
 *      Author: Stephan
 */

#ifndef LINKED_LIST_
#define LINKED_LIST_



// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);

typedef enum { FALSE, TRUE } listbool;

typedef listbool (*listIterator)(void *);

typedef struct _listNode {
  int data[3];
  struct _listNode *next;
} listNode;

typedef struct {
  int logicalLength;
  int elementSize;
  listNode *head;
  listNode *tail;
  freeFunction freeFn;
} linked_list;

void list_new(linked_list *list, int elementSize);
void list_destroy(linked_list *list);

void list_prepend(linked_list *list, int[3]);
void list_append(linked_list *list, int[3]);
int list_size(linked_list *list);
void free_int(int data []);

void list_for_each(linked_list *list, listIterator iterator);
void list_head(linked_list *list, void *element, listbool removeFromList);
void list_tail(linked_list *list, void *element);


#endif
