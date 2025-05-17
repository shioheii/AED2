#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const long int TAM = 100000000;
const int REPEATS = 20;

long int linearSearch(long int size, long int v[], long int t) {
    for (long int index = 0; index < size; index++) {
        if (v[index] == t) return index;
    }
    return -1;
}

long int binarySearchIterative(long int size, long int v[], long int t) {
    long int start = 0, end = size - 1, m;
    while (start <= end) {
        m = (start + end) / 2;
        if (v[m] == t) return m;
        if (v[m] < t) start = m + 1;
        else end = m - 1;
    }
    return -1;
}

long int binarySearchRecursive(long int v[], long int start, long int end, long int t) {
    if (start > end) return -1;
    long int m = (start + end) / 2;
    if (v[m] == t) return m;
    if (v[m] < t)
        return binarySearchRecursive(v, m + 1, end, t);
    else
        return binarySearchRecursive(v, start, m - 1, t);
}

double calculateElapsedTime(struct timespec startTime, struct timespec endTime) {
    double time = (endTime.tv_sec - startTime.tv_sec) * 1000.0;
    time += (endTime.tv_nsec - startTime.tv_nsec) / 1000000.0;
    return time;
}

void measureSearches(long int* v, long int t, char* description) {
    struct timespec startTime, endTime;
    long int result;
    double elapsedTime;

    printf("----- %s -----\n", description);

    clock_gettime(CLOCK_MONOTONIC, &startTime);
    result = linearSearch(TAM, v, t);
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    elapsedTime = calculateElapsedTime(startTime, endTime);
    printf("[Busca Linear]             => Tempo: %lf ms\n", elapsedTime);

    clock_gettime(CLOCK_MONOTONIC, &startTime);
    result = binarySearchIterative(TAM, v, t);
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    elapsedTime = calculateElapsedTime(startTime, endTime);
    printf("[Busca Binária Iterativa]  => Tempo: %lf ms\n", elapsedTime);

    clock_gettime(CLOCK_MONOTONIC, &startTime);
    result = binarySearchRecursive(v, 0, TAM - 1, t);
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    elapsedTime = calculateElapsedTime(startTime, endTime);
    printf("[Busca Binária Recursiva]  => Tempo: %lf ms\n\n", elapsedTime);
}

int main() {
    long int* v = malloc(TAM * sizeof(long int));
    for (long int i = 0; i < TAM; i++) v[i] = i;

    srand(time(NULL));

    measureSearches(v, 0, "Melhor Caso");
    measureSearches(v, TAM - 1, "Pior Caso");

    double totalTimeLinear = 0, totalTimeBinaryIterative = 0, totalTimeBinaryRecursive = 0;
    struct timespec startTime, endTime;
    long int t, result;

    printf("----- Caso Médio - 20 execuções -----\n");
    for (int i = 0; i < REPEATS; i++) {
        t = rand() % TAM;

        clock_gettime(CLOCK_MONOTONIC, &startTime);
        result = linearSearch(TAM, v, t);
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        totalTimeLinear += calculateElapsedTime(startTime, endTime);

        clock_gettime(CLOCK_MONOTONIC, &startTime);
        result = binarySearchIterative(TAM, v, t);
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        totalTimeBinaryIterative += calculateElapsedTime(startTime, endTime);

        clock_gettime(CLOCK_MONOTONIC, &startTime);
        result = binarySearchRecursive(v, 0, TAM - 1, t);
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        totalTimeBinaryRecursive += calculateElapsedTime(startTime, endTime);
    }

    printf("[Busca Linear - Média]             => Tempo médio: %lf ms\n", totalTimeLinear / REPEATS);
    printf("[Busca Binária Iterativa - Média]  => Tempo médio: %lf ms\n", totalTimeBinaryIterative / REPEATS);
    printf("[Busca Binária Recursiva - Média]  => Tempo médio: %lf ms\n", totalTimeBinaryRecursive / REPEATS);

    free(v);
    return 0;
}
