/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include<stdio.h>
#include<stdlib.h>

typedef struct AVLnode {
    int value;
    int height;
    struct AVLnode* left;
    struct AVLnode* right;
} AVLnode;

// Libera recursivamente toda a memória alocada para a árvore AVL
void frees_allocated_memory(AVLnode* root) {
    if (root != NULL) {
        frees_allocated_memory(root->left);
        frees_allocated_memory(root->right);
        free(root);
    }
}

// Retorna a altura de um nó (0 se o nó for NULL)
int get_height(AVLnode* node) {
    return node != NULL ? node->height : 0;
}

// Retorna o maior entre dois valores inteiros
int max(int a, int b) { 
    return a > b ? a : b;
}

// Calcula e retorna o fator de balanceamento de um nó
int get_balance_factor(AVLnode* node) {
    if (node == NULL) return 0;
    return get_height(node->left) - get_height(node->right);
}

// Executa uma rotação simples para a direita
AVLnode* right_rotate(AVLnode* y) {
    AVLnode* x = y->left;
    AVLnode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

// Executa uma rotação simples para a esquerda
AVLnode* left_rotate(AVLnode* x) {
    AVLnode* y = x->right;
    AVLnode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

// Rebalanceia um nó AVL e atualiza sua altura
AVLnode* rebalance(AVLnode* root) {
    if (root == NULL) return NULL;

    root->height = 1 + max(get_height(root->left), get_height(root->right));
    int balance = get_balance_factor(root);

    if (balance > 1 && get_balance_factor(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && get_balance_factor(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance_factor(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && get_balance_factor(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

// Cria e retorna um novo nó AVL com o valor fornecido
AVLnode* create_node(int value) {
    AVLnode* node = (struct AVLnode*)malloc(sizeof(struct AVLnode));
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Insere um valor na árvore AVL e realiza balanceamentos se necessário
AVLnode* insert_node(AVLnode* root, int value) {
    if (root == NULL) return create_node(value);

    if (value < root->value)
        root->left = insert_node(root->left, value);
    else if (value > root->value)
        root->right = insert_node(root->right, value);
    else
        return root;

    return rebalance(root);
}

// Encontra o nó sucessor (menor valor da subárvore à direita)
AVLnode* finds_successor(AVLnode* node) {
    while (node->left != NULL)
        node = node->left;

    return node;
}

// Remove um valor da árvore AVL e realiza balanceamentos se necessário
AVLnode* delete_node(AVLnode* root, int value) {
    if (root == NULL) return root;

    if (value < root->value)
        root->left = delete_node(root->left, value);
    else if (value > root->value)
        root->right = delete_node(root->right, value);
    else {
        if (root->left == NULL || root->right == NULL) {
            AVLnode* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            AVLnode* temp = finds_successor(root->right);
            root->value = temp->value;
            root->right = delete_node(root->right, temp->value);
        }
    }

    if (root == NULL) return NULL;
    return rebalance(root);
}


// Procura um nó com determinado valor e retorna seu endereço ou NULL
AVLnode* search_node(AVLnode* root, int value) {
    if (root == NULL)
        return NULL;

    if (root->value == value)
        return root;

    if (value < root->value) 
        return search_node(root->left, value);

    return search_node(root->right, value);
}

// Insere o valor se ele não estiver na árvore, ou remove se já estiver
void insert_or_remove_value(AVLnode** root, int value) {
    AVLnode* temp = search_node(*root, value);

    if (temp != NULL)
        *root = delete_node(*root, value);
    else
        *root = insert_node(*root, value);
}

// Imprime altura da raiz, da subárvore esquerda e da subárvore direita
void print_tree_heights(AVLnode* root) {
    if (root == NULL) {
        printf("ARVORE VAZIA\n");
    } else {
        int hl = get_height(root->left);
        int hr = get_height(root->right);
        int h = max(hl, hr);
        printf("%d, %d, %d\n", h, hl, hr);
    }
}

// Imprime os valores dentro de uma faixa [start, end] em ordem crescente
void print_values_in_range(AVLnode* root, int start, int end, int* found) {
    if (root == NULL) return;

    if (root->value > start)
        print_values_in_range(root->left, start, end, found);

    if (root->value >= start && root->value <= end) {
        if (*found == 0) {
            *found = 1;
            printf("%d", root->value);
        } else {
            printf(", %d", root->value);
        }
    }

    if (root->value < end)
        print_values_in_range(root->right, start, end, found);
}

// Imprime os valores da faixa encontrados e suas alturas,
// ou "NADA A EXIBIR" se não houver nenhum
void print_range_and_heights(AVLnode* root, int low, int high) {
    int found = 0;
    print_values_in_range(root, low, high, &found);

    if (!found) printf("NADA A EXIBIR");
    printf("\n");

    if (found) {
        for (int i = low; i <= high; i++) {
            AVLnode* temp = search_node(root, i);
            if (temp != NULL)
                print_tree_heights(temp);
        }
    }
}

int main() {
    int n, startRange, endRange;
    AVLnode* root = NULL;

    // 1ª linha: inserção inicial
    while (scanf("%d", &n) && n >= 0) {
        root = insert_node(root, n);
    }
    print_tree_heights(root);

    // 2ª linha: inserções/remoções
    while (scanf("%d", &n) && n >= 0) {
        insert_or_remove_value(&root, n);
    }
    print_tree_heights(root);

    // 3ª linha: faixa de busca
    scanf("%d %d", &startRange, &endRange);
    print_range_and_heights(root, startRange, endRange);

    frees_allocated_memory(root);

    return 0;
}
