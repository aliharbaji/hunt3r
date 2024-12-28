#ifndef HAVEIBEENPWNED_OFFLINE_READ_H
#define HAVEIBEENPWNED_OFFLINE_READ_H
#include "../utility/headers.h"
#include "../queue/queue.h"
#include "../utility/globals.h"

void trim(char* str);

int parseCSV(const char* filename, Queue accounts);

#endif //HAVEIBEENPWNED_OFFLINE_READ_H
