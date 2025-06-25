#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct {
    sem_t resource;
    sem_t rmutex;
    sem_t wmutex;
    sem_t read_try;
    
    int read_count;
    int last_writer;
    int data;
    int data_written;
    int writers_stop;
} Book;

void Book_init(Book* book);

void Book_destroy(Book* book);

#endif