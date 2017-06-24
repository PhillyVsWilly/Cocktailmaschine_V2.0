/*
 * FIFO_Queue.h
 *
 *  Created on: 24 Jun 2017
 *      Author: Jacques
 */

#ifndef FIFO_QUEUE_H_
#define FIFO_QUEUE_H_

typedef struct FIFO_Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements[];
}FIFO_Queue;

FIFO_Queue * createFIFO_Queue(int maxElements);
void Dequeue(FIFO_Queue *Q);
int front(FIFO_Queue *Q);
void Enqueue(FIFO_Queue *Q,int element[]);




#endif /* FIFO_QUEUE_H_ */
