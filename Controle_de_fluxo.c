#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define INTERVALO_CARROS 1000000
#define CAPACIDADE_MAX 20
#define REPETICOES 5

// Thread 1: Soma de pares até limite
void* contar_carros(void* arg) {
    int limite = *(int*)arg;
    long long soma = 0;
    for (int i = 0; i <= limite; i++) {
        if (i % 2 == 0) soma += i;
    }
    printf("Thread 1 - Carros que passaram (soma pares até %d): %lld\n", limite, soma);
    pthread_exit(NULL);
}

// Thread 2: Fatorial
void* verificar_capacidade(void* arg) {
    int n = *(int*)arg;
    long long fatorial = 1;
    for (int i = 2; i <= n; i++) {
        fatorial *= i;
    }
    printf("Thread 2 - Capacidade do cruzamento (fatorial de %d): %lld\n", n, fatorial);
    pthread_exit(NULL);
}

// Função que roda em cada processo (cruzamento)
void cruzamento(int limite_carros, int capacidade, int repeticoes) {
    for (int i = 0; i < repeticoes; i++) {
        printf("\n--- Repetição %d ---\n", i + 1);

        pthread_t t1, t2;

        pthread_create(&t1, NULL, contar_carros, &limite_carros);
        pthread_create(&t2, NULL, verificar_capacidade, &capacidade);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
}

int main() {
    int pid = fork();

    if (pid == 0) {
        // Processo filho: Cruzamento A
        printf("\n[Processo Filho] Cruzamento A iniciando...\n");
        cruzamento(INTERVALO_CARROS, CAPACIDADE_MAX, REPETICOES);
        printf("[Processo Filho] Cruzamento A finalizado.\n\n");
    } else if (pid > 0) {
        // Processo pai: Cruzamento B
        printf("\n[Processo Pai] Cruzamento B iniciando...\n");
        cruzamento(INTERVALO_CARROS, CAPACIDADE_MAX, REPETICOES);
        printf("[Processo Pai] Cruzamento B finalizado.\n\n");
    } else {
        perror("Erro ao criar processo");
        return 1;
    }

    return 0;
}
