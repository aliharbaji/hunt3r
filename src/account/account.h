#ifndef ACCOUNT_H
#define ACCOUNT_H


#include "../utility/headers.h"

// Define the account struct that holds information like password, folder, etc.
// "folder","name","url","username","password","notes","favorite"
struct Account {
    const char* folder;              // Folder (e.g., githubfolder)
    int favorite;                    // 1 if favorite, 0 otherwise
    const char* type;                // Account type (e.g., "login", "social", etc.)
    const char* name;                // Name of the account
    const char* notes;               // Notes for the account
    const char* fields;              // Additional fields (if needed)
    const char* reprompt;            // A field for reprompt, if any
    const char* login_uri;           // URL for login (e.g., for OAuth or websites)
    const char* login_username;      // Username for login
    const char* login_password;      // Password for login
    const char* login_totp;          // Optional TOTP (Two-Factor Authentication) field
    char hashed[SHA_DIGEST_LENGTH * 2 + 1]; // Fixed-size array for storing hashed password
};
// Create an account object with the given fields
struct Account* createAccount(const char* folder, int favorite, const char* type, const char* name,
                              const char* notes, const char* fields, const char* reprompt, const char* login_uri, const char* login_username,
                              const char* login_password, const char* login_totp);

void printAccount(const struct Account* acc);

void freeAcc(struct Account* acc);

#endif // ACCOUNT_H
