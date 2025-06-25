#ifndef PERSON_H
#define PERSON_H
#include <time.h>
#include <stdio.h>
#include "Book.h"

typedef struct {
    unsigned short id;
    Book* book;
}Person;

void Person_init(Person* person, Book* book, unsigned short id);
void writer(Person* person);
void reader(Person* person);

#endif
