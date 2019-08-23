#ifndef __MESSAGEFUNCTIONS_H__
#define __MESSAGEFUNCTIONS_H__

#include <stdint.h>

#include "csapp.h"
#include "miniRSA.h"

#define MAX_LINE 256

typedef struct arguments
{
    int connfd;
    long key;
    long c;
} arguments;

void *send_message(void *arg);
void *read_message(void *arg);

#endif
