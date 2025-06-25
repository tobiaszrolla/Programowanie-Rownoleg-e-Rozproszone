#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Fork.h"
#include "Philosopher.h"


#define N 5
#define SIMULATION_TIME 10

sem_t philosopher_semaphore;

void* Philosopher_thread(void* arg) {
    Philosopher* philosopher = (Philosopher*)arg;
    time_t start_time = time(NULL);

    while ((time(NULL) - start_time) < SIMULATION_TIME) {
        Philosopher_eat(philosopher, &philosopher_semaphore);
        Philosopher_think(philosopher);
    }

    return NULL;
}

void conclusion(Philosopher philosopher[]) {
    for(int i = 0; i < N; i++) {
        printf("Filozof %d je! Zjadł %d razy.\n", philosopher[i].id, philosopher[i].dining_counter);
    }
}

int main() {
    Fork forks[N];
    Philosopher philosophers[N];
    sem_init(&philosopher_semaphore, 0, N - 1);
    pthread_t threads[N];
    srand(time(NULL));

    for(int i = 0; i < N; i++) {
        Fork_init(&forks[i]);
    }

    for (int i = 0; i < N; i++) {
        Philosopher_init(&philosophers[i], &forks[i], &forks[(i + 1) % N], i);
        pthread_create(&threads[i], NULL, Philosopher_thread, (void*)&philosophers[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    conclusion(philosophers);

    for(int i = 0; i < N; i++) {
        Fork_destroy(&forks[i]);
    }

    sem_destroy(&philosopher_semaphore);

    return 0;
}
