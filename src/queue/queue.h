#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <sys/time.h>

// Forward declaration of struct Account
struct Account;

typedef struct Queue* Queue;

// Queue operations that work with struct Account
Queue queueCreate(int capacity);
bool isQueueEmpty(Queue q);
bool isQueueFull(Queue q);
int getQueueSize(Queue q);
bool enqueue(Queue q, struct Account* account, struct timeval arrival);
struct Account* dequeue(Queue q);
struct timeval getHeadArrivalTime(Queue q);
int find(Queue q, struct Account* account);
struct Account* dequeueAtIndex(Queue q, int index);
void queueDestroy(Queue q);
struct timeval getArrivalTimeAtIndex(Queue q, int index);


#endif // QUEUE_H
