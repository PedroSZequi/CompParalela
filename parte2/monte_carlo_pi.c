#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int id;
    long long pontos;
    long long dentro_circulo;
} ThreadArgs;

void* calcular_pontos(void* arg) {
    ThreadArgs* dados = (ThreadArgs*) arg;

    struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);

unsigned int seed =
    (unsigned int) ts.tv_nsec ^
    (unsigned int) ts.tv_sec ^
    (unsigned int) (dados->id * 2654435761u);

    long long dentro = 0;

    for (long long i = 0; i < dados->pontos; i++) {

        double x = (double) rand_r(&seed) / RAND_MAX;
        double y = (double) rand_r(&seed) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            dentro++;
        }
    }

    dados->dentro_circulo = dentro;

    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    if (N <= 0 || num_threads <= 0) {
        printf("Parametros invalidos.\n");
        return 1;
    }

    pthread_t* threads =
        malloc(num_threads * sizeof(pthread_t));

    ThreadArgs* argumentos =
        malloc(num_threads * sizeof(ThreadArgs));

    if (threads == NULL || argumentos == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(threads);
        free(argumentos);
        return 1;
    }

    long long base = N / num_threads;
    long long resto = N % num_threads;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < num_threads; i++) {

        argumentos[i].id = i;

        argumentos[i].pontos =
            base + (i < resto ? 1 : 0);

        argumentos[i].dentro_circulo = 0;

        pthread_create(
            &threads[i],
            NULL,
            calcular_pontos,
            &argumentos[i]
        );
    }

    long long total_dentro = 0;

    for (int i = 0; i < num_threads; i++) {

        pthread_join(threads[i], NULL);

        total_dentro +=
            argumentos[i].dentro_circulo;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi =
        4.0 * (double) total_dentro / (double) N;

    printf(
        "[Monte Carlo] N: %lld | Threads: %d | Pi: %.8f | Tempo: %.6f s\n",
        N,
        num_threads,
        pi,
        tempo
    );

    free(threads);
    free(argumentos);

    return 0;
}