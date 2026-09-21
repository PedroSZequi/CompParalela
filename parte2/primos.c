#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long long K;
int num_threads;

long long total_primos = 0;

pthread_mutex_t mutex_total = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int id;
} ThreadArgs;

int eh_primo(long long n) {
    if (n < 2)
        return 0;

    if (n == 2)
        return 1;

    if (n % 2 == 0)
        return 0;

    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

void* contar_primos(void* arg) {
    ThreadArgs* dados = (ThreadArgs*) arg;

    int id = dados->id;
    long long quantidade_local = 0;

    for (long long n = 3 + 2LL * id;
         n <= K;
         n += 2LL * num_threads) {

        if (eh_primo(n)) {
            quantidade_local++;
        }
    }

    if (id == 0 && K >= 2) {
        quantidade_local++;
    }

    pthread_mutex_lock(&mutex_total);
    total_primos += quantidade_local;
    pthread_mutex_unlock(&mutex_total);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <K> <num_threads>\n", argv[0]);
        return 1;
    }

    K = atoll(argv[1]);
    num_threads = atoi(argv[2]);

    if (K < 2 || num_threads <= 0) {
        printf("Parametros invalidos.\n");
        return 1;
    }

    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* argumentos = malloc(num_threads * sizeof(ThreadArgs));

    if (threads == NULL || argumentos == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < num_threads; i++) {
        argumentos[i].id = i;

        pthread_create(
            &threads[i],
            NULL,
            contar_primos,
            &argumentos[i]
        );
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf(
        "[Primos] K: %lld | Threads: %d | Quantidade: %lld | Tempo: %.6f s\n",
        K,
        num_threads,
        total_primos,
        tempo
    );

    pthread_mutex_destroy(&mutex_total);

    free(threads);
    free(argumentos);

    return 0;
}