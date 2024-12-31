#ifndef HELPER_H
#define HELPER_H

#include "headers.h"
#include "globals.h"
#include "../read/read.h"
#include "../queue/queue.h"
#include <dirent.h>

void prepareQueue(const char* filename);
void getargs(char *csvFilePath, char *pwnedDbPath, int* pool_size,int argc, char *argv[]);
void printStart();
void printEnd();

void* threadRoutine(void* args);

struct threadArgs{
    int thread_id;
    const char* folder_path;
};

#endif