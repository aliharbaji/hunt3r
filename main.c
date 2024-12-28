#include "hash/hash.h"
#include "utility/globals.h"
#include "utility/helper.h"

bool run = true;
Queue passwordQueue;
pthread_mutex_t m_queue;
pthread_cond_t queueAvailable;
pthread_mutex_t m_stdout;


int main(int argc, char* argv[]) {
    // Print welcome message
    printStart();

    // Initialize variables
    char csvFilePath[MAX_PATH_SIZE];
    char pwnedDbPath[MAX_PATH_SIZE];
    int thread_pool_size;

    // Get the arguments provided by the user
    getargs(csvFilePath, pwnedDbPath, &thread_pool_size, argc, argv);

    // Prepare the queue with the CSV file
    prepareQueue(csvFilePath);

    // Initialize thread-pool
    pthread_t* threads[thread_pool_size];
    struct threadArgs* thread_args[thread_pool_size];

    // Update thread-pool
    for (int i = 0; i < thread_pool_size; ++i) {
        threads[i] = (pthread_t*)malloc(sizeof(pthread_t));
        if (threads[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for thread %d.\n", i);
            exit(1);
        }

        thread_args[i] = (struct threadArgs*)malloc(sizeof(struct threadArgs));
        if (thread_args[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for thread arguments %d.\n", i);
            exit(1);
        }

        thread_args[i]->thread_id = i;
        thread_args[i]->folder_path = pwnedDbPath;

        if (pthread_create(threads[i], NULL, threadRoutine, (void *) thread_args[i]) != 0) {
            fprintf(stderr, "Failed to create thread %d.\n", i);
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_pool_size; ++i) {
        if (pthread_join(*threads[i], NULL)) {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }

    // Print finish message
    printEnd();

    // Checks if all accounts were processed
    pthread_mutex_lock(&m_queue);
    while (!isQueueEmpty(passwordQueue)) {
        struct Account* deq = dequeue(passwordQueue);
        if (deq == NULL) {
            continue;
        }
        printf("The following account wasn't checked against the data base:\n");
        printAccount(deq);
    }
    pthread_mutex_unlock(&m_queue);

    // Signal threads to exit and join them
    pthread_mutex_lock(&m_queue);
    queueDestroy(passwordQueue);
    passwordQueue = NULL;
    pthread_cond_broadcast(&queueAvailable);
    pthread_mutex_unlock(&m_queue);

    // Free allocated memory for threads and thread arguments
    for (int i = 0; i < thread_pool_size; ++i) {
        free(threads[i]);
        free(thread_args[i]);
    }

    // Destroy mutex
    pthread_mutex_destroy(&m_queue);

    // Destroy conditional variable
    pthread_cond_destroy(&queueAvailable);
    return 0;
}
