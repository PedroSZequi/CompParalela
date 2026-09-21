#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long long inicio;
    long long fim;
    float *A;
    float *B;
    float *C;
} ThreadArgs;

void* somar_bloco(void* arg) {
    ThreadArgs* dados = (ThreadArgs*) arg;

    for (long long i = dados->inicio; i < dados->fim; i++) {
        dados->C[i] = dados->A[i] + dados->B[i];
    }

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

    float* A = malloc(N * sizeof(float));
    float* B = malloc(N * sizeof(float));
    float* C = malloc(N * sizeof(float));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(A);
        free(B);
        free(C);

        return 1;
    }

    /* Inicialização dos vetores */
    for (long long i = 0; i < N; i++) {
        A[i] = 1.5f;
        B[i] = 2.5f;
    }

    pthread_t* threads =
        malloc(num_threads * sizeof(pthread_t));

    ThreadArgs* argumentos =
        malloc(num_threads * sizeof(ThreadArgs));

    if (threads == NULL || argumentos == NULL) {
        printf("Erro ao alocar estruturas das threads.\n");

        free(A);
        free(B);
        free(C);
        free(threads);
        free(argumentos);

        return 1;
    }

    long long base = N / num_threads;
    long long resto = N % num_threads;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    long long posicao = 0;

    for (int i = 0; i < num_threads; i++) {

        long long tamanho =
            base + (i < resto ? 1 : 0);

        argumentos[i].inicio = posicao;
        argumentos[i].fim = posicao + tamanho;

        argumentos[i].A = A;
        argumentos[i].B = B;
        argumentos[i].C = C;

        pthread_create(
            &threads[i],
            NULL,
            somar_bloco,
            &argumentos[i]
        );

        posicao += tamanho;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    int correto = 1;

    for (int i = 0; i < 10 && i < N; i++) {
        if (C[i] != 4.0f) {
            correto = 0;
            break;
        }
    }

    printf(
        "[Soma Vetores] N: %lld | Threads: %d | Resultado: %s | Tempo: %.6f s\n",
        N,
        num_threads,
        correto ? "correto" : "incorreto",
        tempo
    );

    free(A);
    free(B);
    free(C);

    free(threads);
    free(argumentos);

    return 0;
}