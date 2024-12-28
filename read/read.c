#include "read.h"

// Function to trim leading/trailing spaces from a string
void trim(char* str) {
    char* end;

    // Trim leading spaces
    while (*str == ' ') str++;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n')) end--;
    *(end + 1) = '\0';
}

// Function to parse the CSV file and enqueue Account objects gradually
int parseCSV(const char* filename, Queue q) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    char line[1024];  // Buffer to hold each line
    int count = 0;

    bool skip_first_line = true;

    while (fgets(line, sizeof(line), file)) {
        if (skip_first_line) {
            skip_first_line = false;
            continue;
        }

        pthread_mutex_lock(&m_queue);

        // Array to hold parsed tokens
        char* tokens[12] = {NULL}; // Adjusted to 12 fields (including new ones)
//        int token_index = 0;

        // Parse the line manually to handle empty fields
        char* start = line;
        char* end = NULL;

        for (int i = 0; i < 12; i++) {
            end = strchr(start, ','); // Find the next comma
            if (end) {
                *end = '\0';           // Replace comma with null terminator
                tokens[i] = strdup(start); // Save token
                start = end + 1;       // Move to the next field
            } else {
                tokens[i] = strdup(start); // Last field
                break;
            }
        }

        // Assign tokens to respective variables
        char* folder = tokens[0];
        char* favorite = tokens[1];
        char* type = tokens[2];
        char* name = tokens[3];
        char* notes = tokens[4];
        char* fields = tokens[5];
        char* reprompt = tokens[6];
        char* login_uri = tokens[7];
        char* login_username = tokens[8];
        char* login_password = tokens[9];
        char* login_totp = tokens[10];

        // Handle missing or required fields
        if (!folder || !name || !login_uri || !login_username || !login_password) {
            fprintf(stderr, "Invalid or missing required fields. Skipping line.\n");
            for (int i = 0; i < 12; i++) {
                free(tokens[i]);
            }
            pthread_mutex_unlock(&m_queue);
            continue;
        }

        // Trim spaces from each field (optional)
        trim(folder);
        trim(favorite);
        trim(type);
        trim(name);
        trim(notes);
        trim(fields);
        trim(reprompt);
        trim(login_uri);
        trim(login_username);
        trim(login_password);
        trim(login_totp);

        // Convert "favorite" field to integer (1 if favorite, 0 otherwise)
        int is_favorite = (favorite && strcmp(favorite, "1") == 0) ? 1 : 0;

        // Create a new Account object
        struct Account* account = createAccount(
                folder ? strdup(folder) : "0\n",            // Folder (ensure folder is not NULL)
                is_favorite,                                // Favorite (already an integer)
                type ? strdup(type) : "0\n",                // Type (default to "0\n" if NULL)
                name ? strdup(name) : "0\n",                // Name (default to "0\n" if NULL)
                notes ? strdup(notes) : "0\n",              // Notes (default to "0\n" if NULL)
                fields ? strdup(fields) : "0\n",            // Fields (default to "0\n" if NULL)
                reprompt ? strdup(reprompt) : "0\n",        // Reprompt (default to "0\n" if NULL)
                login_uri ? strdup(login_uri) : "0\n",      // Login URI (default to "0\n" if NULL)
                login_username ? strdup(login_username) : "0\n",  // Username (default to "0\n" if NULL)
                login_password ? strdup(login_password) : "0\n",  // Password (default to "0\n" if NULL)
                login_totp ? strdup(login_totp) : "0\n"     // TOTP (default to "0\n" if NULL)
        );

        if (!account) {
            fprintf(stderr, "Failed to create account object.\n");
            for (int i = 0; i < 12; i++) {
                free(tokens[i]);
            }
            pthread_mutex_unlock(&m_queue);
            continue;
        }
        // For debugging
//        printAccount(account);

        // Enqueue the Account object to the queue
        struct timeval arrival_time = {0};  // dummy value
        enqueue(q, account, arrival_time);

        // Signal that the queue is available and increment count
        pthread_cond_signal(&queueAvailable);
        count++;

        // Clean up memory for the line tokens
        for (int i = 0; i < 12; i++) {
            free(tokens[i]);
        }

        pthread_mutex_unlock(&m_queue);
    }

    fclose(file);
    return count;  // Return the number of accounts parsed
}