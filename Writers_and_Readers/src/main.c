#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Book.h"
#include "Person.h"

#define N_Writers 2
#define N_Readers 4
#define SIMULATION_TIME 30

void* Reader_thread(void* arg) {
    Person* person = (Person*)arg;
    time_t start_time = time(NULL); 

    while ((time(NULL) - start_time) < SIMULATION_TIME) {
        reader(person);
    }

    return NULL;
}
void* Writer_thread(void* arg) {
    Person* person = (Person*)arg;
    time_t start_time = time(NULL);

    while ((time(NULL) - start_time) < SIMULATION_TIME) {
        writer(person);
    }

    return NULL;
}
int main() {
    Book book;
    Book_init(&book);
    int all_peaople = N_Readers + N_Writers;
    Person peaople[all_peaople];
    pthread_t threads[all_peaople];

    for (int i = 0; i < N_Readers; i++) {
        Person_init(&peaople[i], &book, i);
        pthread_create(&threads[i], NULL, Reader_thread, (void*)&peaople[i]);
    }
    for(int i = N_Readers; i < all_peaople; i++) {
        Person_init(&peaople[i], &book, i);
        pthread_create(&threads[i], NULL, Writer_thread, (void*)&peaople[i]);
    }
    for (int i = 0; i < all_peaople; i++) {
        pthread_join(threads[i], NULL);
    }
    Book_destroy(&book);


    return 0;
}
