#include <stdio.h>
#include <stdlib.h>

/************ METODOS E VARIAVEIS GLOBAIS ************/

const int RED = 1;
const int BLACK = 0;

// Retorna o maior entre dois valores inteiros
int max(int a, int b) {
    return a > b ? a : b;
}

/******************* ARVORE AVP  *******************/
typedef struct AVPNode {
    int value;
    int height;
    int color;
    struct AVPNode *left;
    struct AVPNode *right;
    struct AVPNode *parent;
} AVPNode;

// Retorna a altura de um nó (0 se o nó for NULL)
int get_avp_height(AVPNode* node) {
    return node ? node->height : 0;
}

void print_avp_tree_heights(AVPNode* root) {
    int hl = get_avp_height(root->left);
    int hr = get_avp_height(root->right);
    int h = max(hl, hr);
    printf("%d, %d, %d\n", h, hl, hr);
}

AVPNode* update_avp_height(AVPNode* node) {
    if (node) {
        int lh = get_avp_height(node->left);
        int rh = get_avp_height(node->right);
        node->height = max(lh, rh) + 1;
    }
    return node;
}

void fix_heights_upward(AVPNode* node) {
    while (node) {
        update_avp_height(node);
        node = node->parent;
    }
}

// Executa uma rotação simples para a esquerda
void left_rotate_avp(AVPNode** root, AVPNode* x) {
    AVPNode* y = x->right;
    x->right = y->left;
    if (x->right != NULL) {
        x->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    update_avp_height(x);
    update_avp_height(y);
    y->left = x;
    x->parent = y;
}

// Executa uma rotação simples para a direita
void right_rotate_avp(AVPNode** root, AVPNode* y) {
    AVPNode* x = y->left;
    y->left = x->right;
    if (y->left != NULL) {
        y->left->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    update_avp_height(y);
    update_avp_height(x);
    x->right = y;
    y->parent = x;
}

void fix_avp(AVPNode** root, AVPNode* z) {
    AVPNode* parent_z = NULL;
    AVPNode* grand_parent_z = NULL;

    while ((z != *root) && (z->color != BLACK) && (z->parent->color == RED)) {
        parent_z = z->parent;
        grand_parent_z = z->parent->parent;

        // Caso A: Pai é filho esquerdo do avô
        if (parent_z == grand_parent_z->left) {
            AVPNode* uncle_pt = grand_parent_z->right;

            // Caso 1: Tio é vermelho
            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle_pt->color = BLACK;
                z = grand_parent_z;
            } else {
                // Caso 2: z é filho direito
                if (z == parent_z->right) {
                    left_rotate_avp(root, parent_z);
                    z = parent_z;
                    parent_z = z->parent;
                }

                // Caso 3: z é filho esquerdo
                right_rotate_avp(root, grand_parent_z);
                int color_temp = parent_z->color;
                parent_z->color = grand_parent_z->color;
                grand_parent_z->color = color_temp;
                z = parent_z;
            }
        } else { // Caso B: Pai é filho direito do avô
            AVPNode* uncle_pt = grand_parent_z->left;

            // Caso 1: Tio é vermelho
            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle_pt->color = BLACK;
                z = grand_parent_z;
            } else {
                // Caso 2: z é filho esquerdo
                if (z == parent_z->left) {
                    right_rotate_avp(root, parent_z);
                    z = parent_z;
                    parent_z = z->parent;
                }

                // Caso 3: z é filho direito
                left_rotate_avp(root, grand_parent_z);
                int color_temp = parent_z->color;
                parent_z->color = grand_parent_z->color;
                grand_parent_z->color = color_temp;
                z = parent_z;
            }
        }
    }

    (*root)->color = BLACK;
}

// Encontra o nó sucessor (menor valor da subárvore à direita)
AVPNode* finds_successor(AVPNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Remove um valor da árvore AVL e realiza balanceamentos se necessário
// AVPNode* delete_avp_node(AVPNode* root, int value) {
//     if (root == NULL) return root;

//     if (value < root->value)
//         root->left = delete_avp_node(root->left, value);
//     else if (value > root->value)
//         root->right = delete_avp_node(root->right, value);
//     else {
//         if (root->left == NULL || root->right == NULL) {
//             AVPNode* temp = root->left ? root->left : root->right;

//             if (temp == NULL) {
//                 temp = root;
//                 root = NULL;
//             } else {
//                 *root = *temp;
//             }
//             free(temp);
//         } else {
//             AVPNode* temp = finds_successor(root->right);
//             root->value = temp->value;
//             root->right = delete_avp_node(root->right, temp->value);
//         }
//     }

//     fix_avp(&root, z);
//     fix_heights_upward(z);
//     return root;
// }

// Cria e retorna um novo nó AVP com o valor fornecido
AVPNode* create_avp_node(int value) {
    AVPNode* node = (AVPNode*)malloc(sizeof(AVPNode));
    node->value = value;
    node->left = node->right = node->parent = NULL;
    node->color = RED;
    node->height = 1;
    return node;
}

// Insere um valor na árvore AVP e realiza balanceamentos se necessário
AVPNode* insert_avp_node(AVPNode* root, int value) {
    AVPNode* z = create_avp_node(value);
    AVPNode* y = NULL;
    AVPNode* x = root;
    while (x) {
        y = x;
        if (value < x->value) x = x->left;
        else if (value > x->value) x = x->right;
        else return root;
    }
    z->parent = y;
    if (!y) root = z;
    else if (value < y->value) y->left = z;
    else y->right = z;

    fix_avp(&root, z);
    fix_heights_upward(z);
    return root;
}

// Procura um nó com determinado valor e retorna seu endereço ou NULL
AVPNode* search_node(AVPNode* root, int value) {
    if (root == NULL)
        return NULL;

    if (root->value == value)
        return root;

    if (value < root->value) 
        return search_node(root->left, value);

    return search_node(root->right, value);
}

// Insere o valor se ele não estiver na árvore, ou remove se já estiver
void insert_or_remove_value(AVPNode** root, int value) {
    AVPNode* temp = search_node(*root, value);
    if (temp != NULL) {
        print_avp_tree_heights(temp);
        // *root = delete_avp_node(*root, value);
    } else {
        *root = insert_avp_node(*root, value);
    }
}

// Retorna a altura vermelha de um nó
int get_red_height(AVPNode* node) {
    if (!node) return 0;
    int hl = get_red_height(node->left);
    int hr = get_red_height(node->right);
    int maxh = max(hl, hr);
    return maxh + (node->color == RED ? 1 : 0);
}

// Printa o valor da altura vermelha do nó ou "Valor nao encontrado"
void print_avp_red_height(AVPNode* root, int value) {
    AVPNode* temp = search_node(root, value);
    if (temp) {
        printf("%d\n", get_red_height(temp));
    } else {
        printf("Valor nao encontrado\n");
    }
}

// Libera recursivamente toda a memória alocada para a árvore AVP
void free_avp(AVPNode* root) {
    if (root) {
        free_avp(root->left);
        free_avp(root->right);
        free(root);
    }
}

/******************* MODULO PRINCIPAL  *******************/
int main() {
    int n;
    AVPNode* avp_root = NULL;

    while (scanf("%d", &n) && n >= 0) {
        avp_root = insert_avp_node(avp_root, n);
    }

    print_avp_tree_heights(avp_root);

    while (scanf("%d", &n) && n >= 0) {
        insert_or_remove_value(&avp_root, n);
    }

    scanf("%d", &n);
    print_avp_red_height(avp_root, n);

    free_avp(avp_root);
    return 0;
}