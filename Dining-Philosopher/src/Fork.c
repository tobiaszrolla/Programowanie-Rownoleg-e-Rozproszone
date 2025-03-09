#include "Fork.h"

void Fork_init(Fork* fork) {
    pthread_mutex_init(&fork->mutex, NULL);
};

void Fork_destroy(Fork* fork) {
    pthread_mutex_destroy(&fork->mutex);
};