/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

// Flags para espaços vazios e removidos 
const int REMOVED = -2;
const int EMPTY = -1;

// Calcula h1(k) = k mod m
int hash1(int key, int m) {
    return key % m;
}

// Calcula h2(k) = 1 + (k mod (m - 1))
int hash2(int key, int m) {
    return 1 + (key % (m - 1));
}

// Calcula h(k, i) = (h1(k) + i * h2(k)) % m
int hash_duplo(int key, int i, int m) {
    return (hash1(key, m) + i * hash2(key, m)) % m;
}

// Insere uma chave na table hash usando sondagem com duplo hashing
void inserir(int* table, int m, int key) {
    for (int i = 0; i < m; i++) {
        int pos = hash_duplo(key, i, m);
        if (table[pos] == EMPTY || table[pos] == REMOVED) {
            table[pos] = key;
            return;
        }
    }
}

// Pesquisa se a chave está presente na table hash
int search(int* table, int m, int key) {
    for (int i = 0; i < m; i++) {
        int pos = hash_duplo(key, i, m);
        if (table[pos] == EMPTY) {
            return 0;
        }
        if (table[pos] == key) {
            return 1;
        }
    }
    return 0;
}

// Remove todas as ocorrências da chave da table, marcando com REMOVED
void remove_key(int* table, int m, int key) {
    for (int i = 0; i < m; i++) {
        int pos = hash_duplo(key, i, m);
        if (table[pos] == EMPTY) {
            return;
        }
        if (table[pos] == key) {
            table[pos] = REMOVED;
        }
    }
}

// Imprime a table hash formatada com \ para posições vazias e D para removidas
void print_table(int* table, int m) {
    for (int i = 0; i < m; i++) {
        if (table[i] == EMPTY) {
            printf("\\");
        } else if (table[i] == REMOVED) {
            printf("D");
        } else {
            printf("%d", table[i]);
        }
        printf(" ");
    }
    printf("\n");
}

// Centraliza a função de remoção e impressão das saídas
void remove_and_print_table(int* table, int m, int search_key) {
    if (search(table, m, search_key)) {
        remove_key(table, m, search_key);
        print_table(table, m);
    } else {
        printf("Valor nao encontrado\n");
        print_table(table, m);
    }
}

// Módulo principal
int main() {
    int m;
    scanf("%d", &m);

    int* table = (int*)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) {
        table[i] = EMPTY;
    }

    int n;
    while (scanf("%d", &n) && n >= 0) {
        inserir(table, m, n);
    }

    int search_key;
    scanf("%d", &search_key);

    remove_and_print_table(table, m, search_key);

    free(table);
    return 0;
}
