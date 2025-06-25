#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NR_SMOKERS 5     
#define NR_TEMPLE 2      
#define NR_MATCHES 3     
#define SIMULATION_TIME 10 

sem_t temple_semaphore;     
sem_t matchbox_semaphore;   
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

int stop = 0;

void safe_print(const char* msg, int id) {
    pthread_mutex_lock(&print_mutex);
    printf("Palacz %d: %s\n", id, msg);
    pthread_mutex_unlock(&print_mutex);
}

void* smoker(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (!stop) {
        safe_print("czeka na ubijacz...", id);
        sem_wait(&temple_semaphore);

        safe_print("ubija fajkę...", id);
        safe_print("oddaje ubijacz.", id);
        sem_post(&temple_semaphore);

        safe_print("czeka na zapałki...", id);
        sem_wait(&matchbox_semaphore);

        safe_print("zapala fajkę...", id);
        safe_print("oddaje zapałki.", id);
        sem_post(&matchbox_semaphore);

        safe_print("pali fajkę...", id);
        sleep(2);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NR_SMOKERS];

    sem_init(&temple_semaphore, 0, NR_TEMPLE);
    sem_init(&matchbox_semaphore, 0, NR_MATCHES);

    for (int i = 0; i < NR_SMOKERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, smoker, id);
    }

    sleep(SIMULATION_TIME);
    stop = 1;

    for (int i = 0; i < NR_SMOKERS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&temple_semaphore);
    sem_destroy(&matchbox_semaphore);
    pthread_mutex_destroy(&print_mutex);

    printf("Symulacja zakończona.\n");
    return 0;
}
