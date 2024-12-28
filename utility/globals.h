#ifndef HAVEIBEENPWNED_OFFLINE_GLOBALS_H
#define HAVEIBEENPWNED_OFFLINE_GLOBALS_H

#include "headers.h"

#define MAX_QUEUE_SIZE 1024
#define DEFAULT_POOL_SIZE 8
#define MAX_PATH_SIZE 256
#define TRUNCATED_AMOUNT 5

extern Queue passwordQueue;
extern pthread_mutex_t m_queue;
extern pthread_cond_t queueAvailable;
extern bool run;

extern pthread_mutex_t m_stdout;
#endif //HAVEIBEENPWNED_OFFLINE_GLOBALS_H
