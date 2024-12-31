#ifndef HAVEIBEENPWNED_OFFLINE_HASH_H
#define HAVEIBEENPWNED_OFFLINE_HASH_H

#include "../utility/headers.h"
#include <openssl/sha.h>
#include "../queue/queue.h"
void sha1_hash(const char *password, char *outputBuffer);
// Extract the first 5 characters of the SHA-1 hash
void getPwnedPasswordPrefix(const char* password, char* prefix) ;
#endif //HAVEIBEENPWNED_OFFLINE_HASH_H
