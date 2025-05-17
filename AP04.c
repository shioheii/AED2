/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const TRUE = 1;
int const FALSE = 0;
int const MAX_WORD_LEN = 21;
int const ALPHABET_SIZE = 27;

// ******** METODOS GERAIS ******** //
// Converte a palavra para minúsculo usando ASCII
void to_lower(char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] >= 'A' && word[i] <= 'Z')
            word[i] += 32;
    }
}

// Imprime palavras originais com ou sem ponto final
void print_words(char **w, int n, int with_dot) {
    for (int i = 0; i < n; i++) {
        printf("%s", w[i]);
        if (with_dot) printf(".");
        printf("\n");
    }
}

// Imprime vetor C após soma prefixada
void print_array_C(int *C) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        printf("%d", C[i]);
        if (i != ALPHABET_SIZE - 1) printf(" ");
    }
    printf("\n");
}

// Preenche palavras com espaços à direita
void readjust_word_size_and_fill_blanks_with_space(char **w, int n, int d) {
    for (int i = 0; i < n; i++) {
        int j = 0;
        char *padded = (char*)malloc((d + 1) * sizeof(char));
    
        // Copia caractere por caractere da palavra original
        for (j = 0; w[i][j] != '\0'; j++) padded[j] = w[i][j];
    
        // Preenche o restante com espaços
        for (; j < d; j++) padded[j] = ' ';
    
        padded[d] = '\0';
    
        // Libera a original e substitui pela nova
        free(w[i]);
        w[i] = padded;
    }
}

// Retorna o índice do caractere c na chave personalizada
int get_index(char c, char *key) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (key[i] == c) return i;
    }
    return 0; // Espaço, se não encontrado
}
// ******** FIM METODOS GERAIS ******** //

// ******** METODOS DE ORDENAÇÃO ******** //
void counting_sort(char **A, int n, int pos, char *key) {
    int C[ALPHABET_SIZE];
    char **output = (char**)malloc(n * sizeof(char*));

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        C[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int idx = get_index(A[i][pos], key);
        C[idx]++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        C[i + 1] += C[i];
    }

    print_array_C(C);

    for (int i = n - 1; i >= 0; i--) {
        int idx = get_index(A[i][pos], key);
        output[C[idx] - 1] = A[i];
        C[idx]--;
    }

    for (int i = 0; i < n; i++) {
        A[i] = output[i];
    }

    free(output);
}

void radix_sort(char **A, int n, int d, char *key) {
    for (int i = d - 1; i >= 0; i--) {
        counting_sort(A, n, i, key);
    }
}
// ******** FIM METODOS DE ORDENAÇÃO ******** //

int main() {
    int n, d, len;
    scanf("%d", &n);

    // Lê chave e prepara array de chave (com espaço na frente)
    char key_input[27], key[28];
    key[0] = ' ';
    scanf("%s", key_input);
    for (int i = 0; i < 26; i++) {
        key[i + 1] = key_input[i];
    }
    key[27] = '\0';

    // Aloca vetor de palavras
    char **words = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        words[i] = (char*)malloc(21 * sizeof(char));
        scanf("%s", words[i]);

        to_lower(words[i]);

        len = strlen(words[i]);
        if (len > d) d = len;
    }

    print_words(words, n, TRUE);

    // Imprime d (comprimento da maior palavra)
    printf("%d\n", d);

    readjust_word_size_and_fill_blanks_with_space(words, n, d);
    
    radix_sort(words, n, d, key);

    print_words(words, n, FALSE);

    // Libera memória
    for (int i = 0; i < n; i++) free(words[i]);
    free(words);

    return 0;
}
