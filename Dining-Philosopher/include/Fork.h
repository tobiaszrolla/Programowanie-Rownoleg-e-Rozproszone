#ifndef FORK_H
#define FORK_H
#include <pthread.h>

typedef struct{
    pthread_mutex_t mutex;
}Fork;

void Fork_init(Fork* fork);
void Fork_destroy(Fork* fork);

#endif