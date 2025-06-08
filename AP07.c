/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

/************ MÉTODOS E VARIÁVEIS GLOBAIS ************/

const int RED = 1;
const int BLACK = 0;

// Retorna o maior entre dois valores inteiros
int max(int a, int b) {
    return a > b ? a : b;
}

/******************* ÁRVORE RUBRO-NEGRA (AVP) *******************/
typedef struct AVPNode {
    int value;
    int color;
    int height;
    struct AVPNode* left;
    struct AVPNode* right;
    struct AVPNode* parent;
} AVPNode;

// Busca e retorna um nó na árvore
AVPNode* search(AVPNode* root, int value) {
    if (root == NULL) return NULL;
    if (root->value == value) return root;
    
    if (value < root->value) 
        return search(root->left, value);

    return search(root->right, value);
}

// Retorna o sucessor de um nó
AVPNode* tree_successor(AVPNode* node) {
    if (node->right) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
    }
    return node;
}

// Retorna a altura de um nó (0 se for NULL)
int get_height(AVPNode* node) {
    return node ? node->height : 0;
}

// Printa a altura da árvore e das subárvores da esquerda e direita
void print_avp_tree_heights(AVPNode* root) {
    int hl = get_height(root->left);
    int hr = get_height(root->right);
    int h = max(hl, hr);
    printf("%d, %d, %d\n", h, hl, hr);
}

// Calcula e atualiza a altura de um nó
void update_height(AVPNode* node) {
    if (node) {
        int lh = get_height(node->left);
        int rh = get_height(node->right);
        node->height = max(lh, rh) + 1;
    }
}

// Atualiza alturas subindo pela árvore
void fix_heights_upward(AVPNode* node) {
    while (node != NULL) {
        update_height(node);
        node = node->parent;
    }
}

// Retorna altura vermelha (rubro) de um nó
int get_red_height(AVPNode* node) {
    if (!node) return 0;
    int hl = get_red_height(node->left);
    int hr = get_red_height(node->right);
    int maxh = max(hl, hr);
    return maxh + (node->color == RED ? 1 : 0);
}

// Exibe a altura vermelha de um nó ou "Valor nao encontrado"
void print_avp_red_height(AVPNode* root, int value) {
    AVPNode* temp = search(root, value);

    if (temp) {
        printf("%d\n", get_red_height(temp));
    } else {
        printf("Valor nao encontrado\n");
    }
}

// Rotação à esquerda
void left_rotate(AVPNode** root, AVPNode* x) {
    AVPNode* y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
    
    update_height(x);
    update_height(y);
}

// Rotação à direita
void right_rotate(AVPNode** root, AVPNode* y) {
    AVPNode* x = y->left;
    y->left = x->right;
    
    if (x->right != NULL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
    
    update_height(y);
    update_height(x);
}

// Corrige propriedades da AVP após inserção
void insert_avp_fixup(AVPNode** root, AVPNode* z) {
    AVPNode* parent_z = NULL;
    AVPNode* grand_parent_z = NULL;

    while ((z != *root) && (z->color != BLACK) && (z->parent->color == RED)) {
        parent_z = z->parent;
        grand_parent_z = parent_z->parent;

        // Caso A: Pai é filho esquerdo do avô
        if (parent_z == grand_parent_z->left) {
            AVPNode* uncle = grand_parent_z->right;

            // Caso 1: Tio é vermelho
            if (uncle != NULL && uncle->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle->color = BLACK;
                z = grand_parent_z;
            } else {
                // Caso 2: z é filho direito
                if (z == parent_z->right) {
                    left_rotate(root, parent_z);
                    z = parent_z;
                    parent_z = z->parent;
                }

                // Caso 3: z é filho esquerdo
                right_rotate(root, grand_parent_z);
                int color_temp = parent_z->color;
                parent_z->color = grand_parent_z->color;
                grand_parent_z->color = color_temp;
                z = parent_z;
            }
        } else { // Caso B: Pai é filho direito do avô
            AVPNode* uncle = grand_parent_z->left;

            // Caso 1: Tio é vermelho
            if (uncle != NULL && uncle->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle->color = BLACK;
                z = grand_parent_z;
            } else {
                // Caso 2: z é filho esquerdo
                if (z == parent_z->left) {
                    right_rotate(root, parent_z);
                    z = parent_z;
                    parent_z = z->parent;
                }

                // Caso 3: z é filho direito
                left_rotate(root, grand_parent_z);
                int color_temp = parent_z->color;
                parent_z->color = grand_parent_z->color;
                grand_parent_z->color = color_temp;
                z = parent_z;
            }
        }
    }
    (*root)->color = BLACK;
}

// Cria um novo nó
AVPNode* create_node(int value) {
    AVPNode* node = (AVPNode*)malloc(sizeof(AVPNode));
    node->value = value;
    node->color = RED;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// Insere um nó na árvore
AVPNode* insert_avp_node(AVPNode* root, int value) {
    AVPNode* z = create_node(value);
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

    insert_avp_fixup(&root, z);
    fix_heights_upward(z);
    return root;
}

// Corrige propriedades da AVP após remoção
void avp_delete_fixup(AVPNode** root, AVPNode* x, AVPNode* x_parent) {
    while (x != *root && (x == NULL || x->color == BLACK)) {
        if (x == (x_parent != NULL ? x_parent->left : NULL)) {
            AVPNode* w = x_parent != NULL ? x_parent->right : NULL;
            
            // Caso 1
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                left_rotate(root, x_parent);
                w = x_parent->right;
            }
            
            // Caso 2
            if ((w == NULL || (w->left == NULL || w->left->color == BLACK)) && 
                (w == NULL || (w->right == NULL || w->right->color == BLACK))) {
                if (w != NULL) w->color = RED;
                x = x_parent;
                x_parent = x != NULL ? x->parent : NULL;
            } else {
                // Caso 3
                if (w != NULL && (w->right == NULL || w->right->color == BLACK)) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(root, w);
                    w = x_parent->right;
                }
                // Caso 4
                if (w != NULL) {
                    w->color = x_parent->color;
                    if (w->right != NULL) w->right->color = BLACK;
                }
                x_parent->color = BLACK;
                left_rotate(root, x_parent);
                x = *root;
            }
        } else { // SIMÉTRICO
            AVPNode* w = x_parent != NULL ? x_parent->left : NULL;
            
            // Caso 1
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x_parent->color = RED;
                right_rotate(root, x_parent);
                w = x_parent->left;
            }
            
            // Caso 2
            if ((w == NULL || (w->right == NULL || w->right->color == BLACK)) && 
                (w == NULL || (w->left == NULL || w->left->color == BLACK))) {
                if (w != NULL) w->color = RED;
                x = x_parent;
                x_parent = x != NULL ? x->parent : NULL;
            } else {
                // Caso 3
                if (w != NULL && (w->left == NULL || w->left->color == BLACK)) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(root, w);
                    w = x_parent->left;
                }
                // Caso 4
                if (w != NULL) {
                    w->color = x_parent->color;
                    if (w->left != NULL) w->left->color = BLACK;
                }
                x_parent->color = BLACK;
                right_rotate(root, x_parent);
                x = *root;
            }
        }
    }
    if (x != NULL) x->color = BLACK;
}

// Remove um nó na árvore
AVPNode* avp_delete(AVPNode** root, AVPNode* z) {
    AVPNode* y;
    AVPNode* x;
    AVPNode* x_parent;
    
    if (z->left == NULL || z->right == NULL) {
        y = z;
    } else {
        y = tree_successor(z);
    }
    
    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }
    
    x_parent = y->parent;
    if (x != NULL) {
        x->parent = y->parent;
    }
    
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    if (y != z) {
        z->value = y->value;
    }
    
    if (x != NULL) {
        fix_heights_upward(x);
    } else if (x_parent != NULL) {
        fix_heights_upward(x_parent);
    }
    
    if (y->color == BLACK) {
        avp_delete_fixup(root, x, x_parent);
    }
    
    return y;
}

// Insere ou remove um valor, exibindo altura antes da remoção
void insert_or_remove_value(AVPNode** root, int value) {
    AVPNode* temp = search(*root, value);

    if (temp != NULL) {
        print_avp_tree_heights(temp);
        AVPNode* deleted = avp_delete(root, temp);
        free(deleted);
    } else {
        *root = insert_avp_node(*root, value);
    }
}

// Libera memória da árvore
void free_avp(AVPNode* root) {
    if (root != NULL) {
        free_avp(root->left);
        free_avp(root->right);
        free(root);
    }
}

/******************* MÓDULO PRINCIPAL *******************/
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