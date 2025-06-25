#include "Person.h"

void Person_init(Person* person, Book* book, unsigned short id) {
    person->book = book;
    person->id = id;
};

void reader(Person* person) {
    Book* book = person->book;
    while (book->read_count >= 3 || !(book->data_written)) {
    }

    sem_wait(&book->rmutex);
    book->read_count++;
    sem_post(&book->rmutex);

    printf("Czytelnik %d odczytał: %d jest %d \n", person->id, book->data, book->read_count);
    fflush(stdout);
    usleep(500000 + rand() % 500000);




}
void writer(Person* person) {
    Book* book = person->book;
    while (book->read_count < 3 && book->data_written) {

    }
    
    sem_wait(&book->rmutex);
    book->read_count = 0;
    sem_post(&book->rmutex);

    sem_wait(&book->resource);
    usleep(1000000 + rand() % 500000);
    int num = rand() % 100 + 1;
    book->data = num;
    book->data_written = 1;
    book->last_writer = person->id;
    printf("Pisarz %d zapisał: %d \n", person->id, book->data);
    fflush(stdout);

    sem_post(&book->resource);
    while(book->last_writer == person->id) {
        
    }
    
}
