#include "account.h"
// Example function to create a new account
struct Account* createAccount(const char* folder, int favorite, const char* type, const char* name,
                              const char* notes, const char* fields, const char* reprompt, const char* login_uri, const char* login_username,
                              const char* login_password, const char* login_totp) {
    struct Account* acc = (struct Account*)malloc(sizeof(struct Account));
    if (!acc) {
        return NULL; // Memory allocation failed
    }
    acc->folder = strdup(folder);
    acc->favorite = favorite;
    acc->type = type;
    acc->name = strdup(name);
    acc->notes = strdup(notes);
    acc->fields = strdup(fields);
    acc->reprompt = strdup(reprompt);
    acc->login_uri = strdup(login_uri);
    acc->login_username = strdup(login_username);
    acc->login_password = strdup(login_password);
    acc->login_totp = strdup(login_totp); // Default to empty string if NULL

    // Compute the hash directly into the fixed-size array field
    sha1_hash(login_password, acc->hashed);
    acc->hashed[sizeof(acc->hashed) - 1] = '\0';

    return acc;
}

void freeAcc(struct Account* acc) {
    if (!acc) return;

    // Free dynamically allocated memory for strings
    free((void*)acc->folder);
    free((void*)acc->name);
    free((void*)acc->notes);
    free((void*)acc->fields);
    free((void*)acc->reprompt);
    free((void*)acc->login_uri);
    free((void*)acc->login_username);
    free((void*)acc->login_password);
    free((void*)acc->login_totp);

    // Free the Account structure itself
    free(acc);
}

void printAccount(const struct Account* acc) {
    if (!acc) {
        printf("Invalid account.\n");
        return;
    }
    printf("Folder: %s", acc->folder);
    printf("\nFavorite: %d", acc->favorite);
    printf("\nType: %s", acc->type);
    printf("\nName: %s", acc->name);
    printf("\nNotes: %s", acc->notes);
    printf("\nFields: %s", acc->fields);
    printf("\nReprompt: %s", acc->reprompt);
    printf("\nLogin URI: %s", acc->login_uri);
    printf("\nUsername: %s", acc->login_username);
    printf("\nPassword: %s", acc->login_password);
    printf("\nTOTP: %s", acc->login_totp);
    printf("\nHashed Password: %s", acc->hashed);
    printf("\n==================================\n");
}
