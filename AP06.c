/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

/************ METODOS E VARIAVEIS GLOBAIS ************/

const int RED = 1;
const int BLACK = 0;
int avl_rotations = 0, color_changes = 0, avp_rotations = 0;

int max(int a, int b) {
    return a > b ? a : b;
}

/******************* ARVORE AVL  *******************/
typedef struct AVLnode {
    int value;
    int height;
    struct AVLnode* left;
    struct AVLnode* right;
} AVLnode;

int get_avl_height(AVLnode* node) {
    return node ? node->height : 0;
}

int get_balance_factor(AVLnode* node) {
    return node ? get_avl_height(node->left) - get_avl_height(node->right) : 0;
}

AVLnode* right_rotate_avl(AVLnode* y) {
    AVLnode* x = y->left;
    AVLnode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(get_avl_height(y->left), get_avl_height(y->right)) + 1;
    x->height = max(get_avl_height(x->left), get_avl_height(x->right)) + 1;
    return x;
}

AVLnode* left_rotate_avl(AVLnode* x) {
    AVLnode* y = x->right;
    AVLnode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(get_avl_height(x->left), get_avl_height(x->right)) + 1;
    y->height = max(get_avl_height(y->left), get_avl_height(y->right)) + 1;
    return y;
}

AVLnode* rebalance_avl(AVLnode* root) {
    if (!root) return NULL;
    root->height = 1 + max(get_avl_height(root->left), get_avl_height(root->right));
    int balance = get_balance_factor(root);
    if (balance > 1 && get_balance_factor(root->left) >= 0){
        avl_rotations++;
        return right_rotate_avl(root);
    }
    if (balance > 1 && get_balance_factor(root->left) < 0) {
        avl_rotations++;
        root->left = left_rotate_avl(root->left);
        return right_rotate_avl(root);
    }
    if (balance < -1 && get_balance_factor(root->right) <= 0){
        avl_rotations++;
        return left_rotate_avl(root);
    }
    if (balance < -1 && get_balance_factor(root->right) > 0) {
        avl_rotations++;
        root->right = right_rotate_avl(root->right);
        return left_rotate_avl(root);
    }
    return root;
}

AVLnode* create_avl_node(int value) {
    AVLnode* node = (AVLnode*)malloc(sizeof(AVLnode));
    node->value = value;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

AVLnode* insert_avl_node(AVLnode* root, int value) {
    if (!root) return create_avl_node(value);
    if (value < root->value)
        root->left = insert_avl_node(root->left, value);
    else if (value > root->value)
        root->right = insert_avl_node(root->right, value);
    return rebalance_avl(root);
}

void print_avl_tree_heights(AVLnode* root) {
    if (!root) {
        printf("0, 0, 0\n");
    } else {
        int hl = get_avl_height(root->left);
        int hr = get_avl_height(root->right);
        int h = max(hl, hr);
        printf("%d, %d, %d\n", h, hl, hr);
    }
}

void free_avl(AVLnode* root) {
    if (root) {
        free_avl(root->left);
        free_avl(root->right);
        free(root);
    }
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

int get_avp_height(AVPNode* node) {
    return node ? node->height : 0;
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

void left_rotate_avp(AVPNode** root, AVPNode* x) {
    avp_rotations++;
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

void right_rotate_avp(AVPNode** root, AVPNode* y) {
    avp_rotations++;
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
                color_changes += 3;
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
                color_changes += 2;
                z = parent_z;
            }
        } else { // Caso B: Pai é filho direito do avô
            AVPNode* uncle_pt = grand_parent_z->left;

            // Caso 1: Tio é vermelho
            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle_pt->color = BLACK;
                color_changes += 3;
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
                color_changes += 2;
                z = parent_z;
            }
        }
    }

    (*root)->color = BLACK;
    if ((*root)->color != BLACK) color_changes++;
}

AVPNode* create_avp_node(int value) {
    AVPNode* node = (AVPNode*)malloc(sizeof(AVPNode));
    node->value = value;
    node->left = node->right = node->parent = NULL;
    node->color = RED;
    node->height = 1;
    return node;
}

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

int get_black_height(AVPNode* node) {
    if (!node) return 0;
    int hl = get_black_height(node->left);
    int hr = get_black_height(node->right);
    int maxh = max(hl, hr);
    return maxh + (node->color == BLACK ? 1 : 0);
}

void print_avp_tree_heights(AVPNode* root) {
    if (!root) {
        printf("0, 0, 0\n");
    } else {
        int hl = get_avp_height(root->left);
        int hr = get_avp_height(root->right);
        int h = max(hl, hr);
        printf("%d, %d, %d\n", h, hl, hr);
    }
}

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
    AVLnode* avl_root = NULL;
    AVPNode* avp_root = NULL;

    while (scanf("%d", &n) && n >= 0) {
        avl_root = insert_avl_node(avl_root, n);
        avp_root = insert_avp_node(avp_root, n);
    }

    print_avl_tree_heights(avl_root);
    print_avp_tree_heights(avp_root);
    printf("%d\n", get_black_height(avp_root));
    printf("%d, %d, %d\n", color_changes, avp_rotations, avl_rotations);

    free_avl(avl_root);
    free_avp(avp_root);
    return 0;
}