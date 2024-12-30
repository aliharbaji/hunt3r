#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include "queue.h"
#include "../account/account.h"

// Define the Node structure
struct Node {
    struct Account* data;  // Now this will point to an Account struct
    struct timeval arrival_time;
    struct Node* next;
};

// Define the Queue structure
struct Queue {
    int size;
    int capacity;
    struct Node* front;
    struct Node* rear;
};

// Function to create the queue
Queue queueCreate(int capacity) {
    Queue q = (Queue)malloc(sizeof(struct Queue));
    if (!q) return NULL;

    q->size = 0;
    q->capacity = capacity;
    q->front = q->rear = NULL;
    return q;
}

// Function to check if the queue is empty
bool isQueueEmpty(Queue q) {
    return (q->size == 0);
}

// Function to check if the queue is full
bool isQueueFull(Queue q) {
    return (q->size == q->capacity);
}

// Function to get the size of the queue
int getQueueSize(Queue q) {
    return q->size;
}

// Function to enqueue an account into the queue
bool enqueue(Queue q, struct Account* account, struct timeval arrival) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) return false;

    newNode->data = account;
    newNode->arrival_time = arrival;
    newNode->next = NULL;

    if (isQueueEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->size++;
    return true;
}

// Function to dequeue an account from the queue
struct Account* dequeue(Queue q) {
    if (isQueueEmpty(q)) return NULL;

    struct Node* temp = q->front;
    struct Account* account = temp->data;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return account;
}

// Function to get the arrival time of the front node
struct timeval getHeadArrivalTime(Queue q) {
    if (isQueueEmpty(q)) {
        return (struct timeval){0};
    }
    return q->front->arrival_time;
}

// Function to find an account in the queue
int find(Queue q, struct Account* account) {
    if (isQueueEmpty(q)) return -1;

    struct Node* current = q->front;
    int index = 0;

    while (current != NULL) {
        if (current->data == account) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

// Function to dequeue an account at a specific index
struct Account* dequeueAtIndex(Queue q, int index) {
    if (isQueueEmpty(q) || index < 0 || index >= q->size) {
        return NULL;
    }

    if (index == 0) {
        return dequeue(q);
    }

    struct Node* prev = q->front;
    for (int i = 0; i < index - 1; i++) {
        prev = prev->next;
    }

    struct Node* toRemove = prev->next;
    struct Account* account = toRemove->data;

    prev->next = toRemove->next;
    if (toRemove == q->rear) {
        q->rear = prev;
    }

    free(toRemove);
    q->size--;
    return account;
}

// Function to destroy the queue and free memory
void queueDestroy(Queue q) {
    while (!isQueueEmpty(q)) {
        dequeue(q);
    }
    free(q);
}

// Function to get the arrival time of the account at a specific index
struct timeval getArrivalTimeAtIndex(Queue q, int index) {
    if (isQueueEmpty(q) || index < 0 || index >= q->size) {
        return (struct timeval){0};
    }

    struct Node* current = q->front;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->arrival_time;
}


