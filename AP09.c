/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

/************************ CONSTANTES *************************/
const int true = 1;
const int false = 0;

/******************* ESTRUTURA DA ÁRVORE B *******************/
typedef struct BTreeNode {
    int *keys;            // Vetor de chaves
    int t;                // Grau mínimo
    int n;                // Número atual de chaves
    int leaf;             // 1 se o nó é folha, 0 caso contrário
    struct BTreeNode **c; // Vetor de ponteiros para os filhos
} BTreeNode;
 
/******************* FUNÇÕES DA ÁRVORE B *******************/

BTreeNode* create_btree_node(int t, int leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int*)malloc(sizeof(int) * (2 * t + 1));
    node->c = (BTreeNode**)malloc(sizeof(BTreeNode*) * (2 * t + 2));
    node->n = 0;

    // Inicializando os filhos como NULL
    for (int i = 0; i < (2 * t + 2); i++) {
        node->c[i] = NULL;
    }
    return node;
}
 
// Função para buscar uma chave em uma subárvore enraizada em um nó
BTreeNode* search(BTreeNode* node, int k) {
    int i = 0;
    while (i < node->n && k > node->keys[i])
        i++;

    if (i < node->n && node->keys[i] == k)
        return node;

    if (node->leaf)
        return NULL;

    return search(node->c[i], k);
}

// Função auxiliar para dividir um filho y do nó x que está cheio
void split_child(BTreeNode* parent, int index) {
    BTreeNode* fullChild = parent->c[index];
    int t = fullChild->t;

    BTreeNode* newChild = create_btree_node(t, fullChild->leaf);

    // O índice da chave que vai subir
    int middle = t; // A chave na posição t sobe

    // newChild recebe as últimas t chaves de fullChild
    for (int j = 0; j < t; j++) {
        newChild->keys[j] = fullChild->keys[j + t + 1];
    }

    // Se não for folha, move também os filhos
    if (!fullChild->leaf) {
        for (int j = 0; j < t + 1; j++) {
            newChild->c[j] = fullChild->c[j + t + 1];
            fullChild->c[j + t + 1] = NULL;
        }
    }

    newChild->n = t;
    fullChild->n = t;

    // Faz espaço no parent para inserir nova chave e filho
    for (int j = parent->n; j >= index + 1; j--) {
        parent->c[j + 1] = parent->c[j];
    }
    parent->c[index + 1] = newChild;

    for (int j = parent->n - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[index] = fullChild->keys[middle];

    parent->n++;
    // printf("chave %d sobe\n", parent->keys[index]);
}

// Função auxiliar para inserir uma nova chave em um nó que não está cheio
void insert_non_full(BTreeNode* node, int k) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && k < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = k;
        node->n++;

    } else {
        while (i >= 0 && k < node->keys[i])
            i--;

        i++;

        insert_non_full(node->c[i], k);

        // Se após a inserção o filho ficou cheio, faz split
        if (node->c[i]->n > 2 * node->t) {
            split_child(node, i);
        }
    }
}

// Função para inserir uma nova chave na B-tree
void insert(BTreeNode** r, int k, int t) {
    BTreeNode* root = *r;

    // Não insere chaves repetidas 
    if (search(root, k) != NULL) return;

    insert_non_full(root, k);

    if (root->n > 2 * t) {
        BTreeNode* newRoot = create_btree_node(t, false);
        newRoot->c[0] = root;
        split_child(newRoot, 0);
        *r = newRoot;
    }
}

// Função para imprimir o tamanho da página
void print_length_page(BTreeNode* node) {
    if (node == NULL) return;
    printf("%d\n", node->n);
}

// Função para imprimir as keys de uma página
void print_page(BTreeNode* node) {
    for (int i = 0; i < node->n; i++) {
        printf("%d", node->keys[i]);
        if (i != node->n - 1)
            printf(" ");
    }
    printf("\n");
}

// Função para buscar uma chave k na B-tree e imprimir a quantidade de elementos
// da página deste número e todos os elementos do mesmo vetor
void search_and_print_page_info(BTreeNode* root, int k) {
    BTreeNode* node = search(root, k);
    if (node) {
        print_length_page(node);
        print_page(node);
    } else {
        printf("Valor nao encontrado\n");
    }
}

// Função para imprimir a arvore B de baixo para cima (debug)
void print_btree(BTreeNode* node) {
    if (node) {
        if (!node->leaf) {
            for (int i = 0; i <= node->n; i++) {
                print_btree(node->c[i]);
            }
        }
        printf("tam %d t %d ", node->n, node->t); 
        printf("page ");
        print_page(node);
    }
}

// Libera a memória da árvore B
void free_btree(BTreeNode* node) {
    if (node) {
        if (!node->leaf) {
            for (int i = 0; i <= node->n; i++) {
                free_btree(node->c[i]);
            }
        }
        free(node->keys);
        free(node->c);
        free(node);
    }
}

int main () {
    int m;
    scanf("%d", &m);

    BTreeNode* root = create_btree_node(m, true);

    int n;
    while (scanf("%d", &n) && n >= 0) {
        // printf("inserindo %d\n", n);
        insert(&root, n, m);
        // print_btree(root);
        // printf("\n\n\n");
    }
    
    // print_btree(root);
    print_length_page(root);

    int search_key;
    scanf("%d", &search_key);
    search_and_print_page_info(root, search_key);

    // print_btree(root);
    // printf("\nFREE\n\n");
    free_btree(root);
    
    return 0;
}