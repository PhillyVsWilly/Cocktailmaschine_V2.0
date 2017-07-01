/*
 * http://pseudomuto.com/development/2013/05/02/implementing-a-generic-linked-list-in-c/
 *
 */

#ifndef LINKED_LIST_
#define LINKED_LIST_
#define NODE_SIZE sizeof(list_node);
#define INGREDIENT_SIZE sizeof(ingredient_t);



// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);

typedef enum { FALSE, TRUE } listbool;

typedef listbool (*listIterator)(ingredient_t);



typedef struct {
	int bottleID;
	float amount;
	listbool lastInstruction;
}ingredient_t;

typedef struct _listNode {
  ingredient_t ingredient;
  struct _listNode *next;
} listNode;

typedef struct {
  int logicalLength;
  int elementSize;
  listNode *head;
  listNode *tail;
  freeFunction freeFn;
} linked_list;



void list_new(linked_list *list);
void list_destroy(linked_list *list);

void list_prepend(linked_list *list, ingredient_t ingredient);
void list_append(linked_list *list, ingredient_t ingredient);
int list_size(linked_list *list);
void free_int(ingredient_t ingredient);

void list_for_each(linked_list *list, listIterator iterator);
void list_head(linked_list *list, listNode *currentNode, listbool removeFromList);
void list_tail(linked_list *list, listNode *node);


#endif
