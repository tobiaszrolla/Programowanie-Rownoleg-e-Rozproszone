#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H
#include <time.h>
#include <semaphore.h>
#include "Fork.h"

typedef struct {
    unsigned int id;
    unsigned int dining_counter;
    Fork* left;
    Fork* right;
}Philosopher;

void Philosopher_init(Philosopher* philosopher, Fork* left, Fork* right, unsigned int id);
void Philosopher_eat(Philosopher* philosopher, sem_t* philosopher_semaphore);
void Philosopher_think(Philosopher* philosopher);


#endif