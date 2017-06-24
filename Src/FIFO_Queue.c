#include<stdio.h>
#include<stdlib.h>
#include<Debug.h>
#include"FIFO_Queue.h"
/*Queue has five properties. capacity stands for the maximum number of elements Queue can hold.
  Size stands for the current size of the Queue and elements is the array of elements. front is the
 index of first element (the index at which we remove the element) and rear is the index of last element
 (the index at which we insert the element) */


/* crateFIFO_Queue function takes argument the maximum number of elements the FIFO_Queue can hold, creates
   a FIFO_Queue according to it and returns a pointer to the FIFO_Queue. */
FIFO_Queue * createFIFO_Queue(int maxElements)
{
        /* Create a FIFO_Queue */
        FIFO_Queue *Q;
        Q = (FIFO_Queue *)malloc(sizeof(FIFO_Queue));
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        /* Return the pointer */
        return Q;
}
void Dequeue(FIFO_Queue *Q)
{
        /* If FIFO_Queue size is zero then it is empty. So we cannot pop */
        if(Q->size==0)
        {
                DPRINT_MESSAGE("FIFO_Queue is Empty\n");
                return;
        }
        /* Removing an element is equivalent to incrementing index of front by one */
        else
        {
                Q->size--;
                Q->front++;
                /* As we fill elements in circular fashion */
                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        return;
}
int front(FIFO_Queue *Q)
{
        if(Q->size==0)
        {
                return NULL;
        }
        /* Return the element which is at the front*/
        return Q->elements[Q->front];
}
void Enqueue(FIFO_Queue *Q,int element[])
{
        /* If the FIFO_Queue is full, we cannot push an element into it as there is no space for it.*/
        if(Q->size == Q->capacity)
        {
                DPRINT_WARNING("FIFO_Queue is Full\n");
        }
        else
        {
                Q->size++;
                Q->rear = Q->rear + 1;
                /* As we fill the queue in circular fashion */
                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }
                /* Insert the element in its rear side */
                Q->elements[Q->rear] = element;
        }
        return;
}
