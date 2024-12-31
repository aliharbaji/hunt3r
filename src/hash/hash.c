#include "hash.h"

// hash the input password using SHA-1
void sha1_hash(const char *password, char *outputBuffer) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)password, strlen(password), hash);

    // Convert the hash to a hex string
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        sprintf(outputBuffer + (i * 2), "%02X", hash[i]);
    }
    outputBuffer[SHA_DIGEST_LENGTH * 2] = '\0';
}
