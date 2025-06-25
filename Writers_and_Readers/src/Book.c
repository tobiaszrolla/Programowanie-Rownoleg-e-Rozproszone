#include "Book.h"


void Book_init(Book* book) {
    book->data = 0; 
    book->read_count = 0; 
    book->last_writer = 0;
    book->data_written = 0;
    book->writers_stop = 0;    

    sem_init(&book->resource, 0, 1);
    sem_init(&book->rmutex, 0, 1);
    sem_init(&book->wmutex, 0, 1);
    sem_init(&book->read_try, 0, 1);

    
}

void Book_destroy(Book* book) {
    sem_destroy(&book->resource);
    sem_destroy(&book->rmutex);
    sem_destroy(&book->wmutex);
    sem_destroy(&book->read_try);
}
