#include "helper.h"

// Gets user arguments
void getargs(char *csvFilePath, char *pwnedDbPath, int* pool_size,int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <path_to_csv_file> <path_to_pwned_db> <thread_pool_size = defaults to 8>\n", argv[0]);
        exit(1);
    }

    strcpy(csvFilePath, argv[1]);
    strcpy(pwnedDbPath, argv[2]);
    *pool_size = DEFAULT_POOL_SIZE;
    if(argc < 4) return;
    *pool_size = atoi(argv[3]);
}

// Prints welcome message
void printStart() {
    printf("   ___                             _           ___  _                  _    \n");
    printf("  / _ \\__      __ _ __    ___   __| |         / __\\| |__    ___   ___ | | __\n");
    printf(" / /_)/\\ \\ /\\ / /| '_ \\  / _ \\ / _` | _____  / /   | '_ \\  / _ \\ / __|| |/ /\n");
    printf("/ ___/  \\ V  V / | | | ||  __/| (_| ||_____|/ /___ | | | ||  __/| (__ |   < \n");
    printf("\\/       \\_/\\_/  |_| |_| \\___| \\__,_|       \\____/ |_| |_| \\___| \\___||_|\\_\\\n\n");
}

// Prints goodbye message
void printEnd(){
    printf("   __ _       _     _              _            _               _    \n");
    printf("  / _(_)     (_)   | |            | |          | |             | |   \n");
    printf(" | |_ _ _ __  _ ___| |__   ___  __| |______ ___| |__   ___  ___| | __\n");
    printf(" |  _| | '_ \\| / __| '_ \\ / _ \\/ _` |______/ __| '_ \\ / _ \\/ __| |/ /\n");
    printf(" | | | | | | | \\__ \\ | | |  __/ (_| |     | (__| | | |  __/ (__|   < \n");
    printf(" |_| |_|_| |_|_|___/_| |_|\\___|\\__,_|      \\___|_| |_|\\___|\\___|_|\\_\\\n");
    printf("                                                                     \n");
    printf("                                                                     \n");
}

// Checks if given password was pwned, returns the number of appearances in data breaches
int isPasswordPwned(char password[SHA_DIGEST_LENGTH * 2 + 1], const char* folder_path) {
    static pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
    // For debugging purposes
//    printf("Password /%s/ is being checked against the database\n", password);
    DIR *dir;
    FILE *file;
    char line[1024];
    char file_path[MAX_PATH_SIZE];
    int appearances = 0;

    const char* hashed_password = password + TRUNCATED_AMOUNT;

    // Extract the first 5 characters of the hashed password to determine the file name
    char prefix[TRUNCATED_AMOUNT + 1]; // 5 characters + null terminator
    strncpy(prefix, password, TRUNCATED_AMOUNT);  // Take the first 5 characters
    prefix[TRUNCATED_AMOUNT] = '\0';  // Null terminate the prefix

    // Open the directory
    dir = opendir(folder_path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    // Construct the file path based on the prefix
    snprintf(file_path, sizeof(file_path), "%s/%s", folder_path, prefix);

    // Lock the mutex before accessing the file
    pthread_mutex_lock(&file_mutex);

    // Open the file for reading
    file = fopen(file_path, "r");
    if (file == NULL) {
        pthread_mutex_unlock(&file_mutex);
        closedir(dir);  // Don't forget to close the directory
        return -1;
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the newline character from the line if it exists
        line[strcspn(line, "\n")] = '\0';

        // Split the line by ':' to get the hash and number of occurrences
        char *token = strtok(line, ":");
        if (token == NULL) {
            continue; // Skip invalid lines
        }

        // Compare the hashed password with the stored hash in the file
        if (strcmp(token, hashed_password) == 0) {
            // For debugging purposes
//            printf("token = %s hashed_password = %s\n", token, hashed_password);
            // If found, retrieve the number of times and add to appearances
            token = strtok(NULL, ":");
            if (token != NULL) {
                appearances += atoi(token);  // Add the number of times to the total
                break;
            }
        }
    }

    // Close the file and directory
    fclose(file);
    pthread_mutex_unlock(&file_mutex);
    closedir(dir);

    return appearances;
}

// Worker thread routine function
void* threadRoutine(void* args) {
    // Cast args to struct threadArgs
    struct threadArgs *args_struct = (struct threadArgs*) args;

    while (true) {
        pthread_mutex_lock(&m_queue);
        // NULL CHECK
        if (passwordQueue == NULL) {
            pthread_mutex_unlock(&m_queue);
            break;
        }

        if (isQueueEmpty(passwordQueue)) {
            run = false;
            pthread_mutex_unlock(&m_queue);
            break;
        }

        struct Account* acc = dequeue(passwordQueue);

        pthread_mutex_unlock(&m_queue);

        if (acc == NULL) {
            continue;
        }
        int pwnedAmount = isPasswordPwned(acc->hashed, args_struct->folder_path);

        if (pwnedAmount > 0) {
            printf("PASSWORD '%s' PWNED -%d- TIMES!\n"
                   "WE ADVISE YOU CHANGE IT ON THE FOLLOWING ACCOUNTS..."
                   "(this mechanism is not yet supported)\n",
                   acc->login_password, pwnedAmount);
        }
        freeAcc(acc);
    }
    return NULL;
}

// Initializes the queue and enqueues all accounts from CSV file
void prepareQueue(const char* filename) {
    // Initialize the queue and mutex/condition
    passwordQueue = queueCreate(MAX_QUEUE_SIZE);  // Define a max size for the queue (100 in this case)
    if (passwordQueue == NULL) {
        fprintf(stderr, "Failed to create queue.\n");
        exit(1);
    }

    if (pthread_mutex_init(&m_queue, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex.\n");
        exit(1);
    }

    if (pthread_mutex_init(&m_stdout, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex.\n");
        exit(1);
    }

    if (pthread_cond_init(&queueAvailable, NULL) != 0) {
        fprintf(stderr, "Failed to initialize condition variable.\n");
        exit(1);
    }

    // Parse the CSV file and enqueue Account objects
    int accountCount = parseCSV(filename, passwordQueue);

    if (accountCount < 0) {
        printf("Failed to parse CSV file.\n");
        return;
    }

    printf("Successfully parsed and enqueued %d accounts.\n", accountCount);
}