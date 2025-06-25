#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NR_THREADS 5

// Struktura dla danych wątku
typedef struct {
    int start;
    int end;
    double result;
} ThreadData;

// Funkcja obliczająca część sumy harmonicznej
void *calculate_partial_sum(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    double partial_sum = 0.0;

    // Obliczanie sumy harmonicznej dla zakresu od start do end
    for (int i = data->start; i <= data->end; i++) {
        partial_sum += 1.0 / i;
    }

    data->result = partial_sum;
    pthread_exit(NULL);
};

int main() {
    int n = 0;

    // Wczytanie liczby elementów
    printf("Podaj n (liczba elementów): ");
    scanf("%d", &n);

    // Inicjalizacja tablic wątków i danych dla wątków
    pthread_t *threads = malloc(NR_THREADS * sizeof(pthread_t));
    ThreadData *thread_data = malloc(NR_THREADS * sizeof(ThreadData));

    // Obliczanie rozmiaru dla każdego wątku
    int chunk_size = n / NR_THREADS;  // Ilość elementów na każdy wątek
    int remaining = n % NR_THREADS;   // Pozostałe elementy
    int start = 1;

    // Tworzenie wątków
    for (int i = 0; i < NR_THREADS; i++) {
        thread_data[i].start = start;
        thread_data[i].end = start + chunk_size - 1;

        if (remaining > 0) {
            thread_data[i].end++;
            remaining--;
        }

        start = thread_data[i].end + 1;

        pthread_create(&threads[i], NULL, calculate_partial_sum, (void *)&thread_data[i]);
    }

    double total_sum = 0.0;
    for (int i = 0; i < NR_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    double result = total_sum - log(n);

    printf("Przybliżenie stałej Eulera dla n = %d: %lf\n", n, result);

    free(threads);
    free(thread_data);

    return 0;
}
