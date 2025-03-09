#include "Philosopher.h"
void Philosopher_init(Philosopher* philosopher, Fork* left, Fork* right, unsigned int id) {
    philosopher->id = id;
    philosopher->dining_counter = 0;
    philosopher->left = left;
    philosopher->right = right;
};

void Philosopher_eat(Philosopher* philosopher, sem_t* philosopher_semaphore) {
    sem_wait(philosopher_semaphore);
    if (philosopher->id % 2 == 0) {
        pthread_mutex_lock(&philosopher->left->mutex);
        pthread_mutex_lock(&philosopher->right->mutex);
    } 
    else {
        pthread_mutex_lock(&philosopher->right->mutex);
        pthread_mutex_lock(&philosopher->left->mutex);
    }
    usleep(5000 + rand() % 10000);
    philosopher->dining_counter++;
    pthread_mutex_unlock(&philosopher->right->mutex);
    pthread_mutex_unlock(&philosopher->left->mutex);
    sem_post(philosopher_semaphore);
};

void Philosopher_think(Philosopher* philosopher) {
    usleep(10000 + rand() % 20000);
};