/*
 * AP06 - AED2 - Bruno Shiohei
 * Comparação entre Árvores AVL e Árvores Rubro-Negras (AVP)
 */

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return a > b ? a : b;
}

/******************* AVL TREE DEFINITIONS *******************/

typedef struct AVLnode {
    int value;
    int height;
    struct AVLnode* left;
    struct AVLnode* right;
} AVLnode;

int avl_rotations = 0;

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

/******************* AVP TREE DEFINITIONS *******************/

const int BLACK = 0;
const int RED = 1;

typedef struct AVPNode {
    int value;
    int height;
    int color;
    struct AVPNode *left, *right, *parent;
} AVPNode;

int color_changes = 0, avp_rotations = 0;

int get_avp_height(AVPNode* node) {
    return node ? node->height : 0;
}

AVPNode* update_avp_height(AVPNode* node) {
    if (node) {
        int lh = get_avp_height(node->left);
        int rh = get_avp_height(node->right);
        node->height = 1 + (lh > rh ? lh : rh);
    }
    return node;
}

AVPNode* create_avp_node(int value) {
    AVPNode* node = (AVPNode*)malloc(sizeof(AVPNode));
    node->value = value;
    node->color = RED;
    node->height = 1;
    node->left = node->right = node->parent = NULL;
    return node;
}

AVPNode* left_rotate_avp(AVPNode* root, AVPNode* x) {
    AVPNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
    update_avp_height(x);
    update_avp_height(y);
    avp_rotations++;
    return root;
}

AVPNode* right_rotate_avp(AVPNode* root, AVPNode* y) {
    AVPNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
    update_avp_height(y);
    update_avp_height(x);
    avp_rotations++;
    return root;
}

AVPNode* fix_avp(AVPNode* root, AVPNode* z) {
    while (z->parent && z->parent->color == RED) {
        AVPNode* g = z->parent->parent;
        if (z->parent == g->left) {
            AVPNode* y = g->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                g->color = RED;
                color_changes += 3;
                z = g;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    root = left_rotate_avp(root, z);
                }
                z->parent->color = BLACK;
                g->color = RED;
                color_changes += 2;
                root = right_rotate_avp(root, g);
            }
        } else {
            AVPNode* y = g->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                g->color = RED;
                color_changes += 3;
                z = g;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    root = right_rotate_avp(root, z);
                }
                z->parent->color = BLACK;
                g->color = RED;
                color_changes += 2;
                root = left_rotate_avp(root, g);
            }
        }
    }
    if (root->color == RED) {
        root->color = BLACK;
        color_changes++;
    }
    return root;
}

void fix_heights_upward(AVPNode* node) {
    while (node) {
        update_avp_height(node);
        node = node->parent;
    }
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
    root = fix_avp(root, z);
    fix_heights_upward(z);
    return update_avp_height(root);
}

int get_black_height(AVPNode* node) {
    if (!node) return 0;
    int lh = get_black_height(node->left);
    int rh = get_black_height(node->right);
    int maxh = lh > rh ? lh : rh;
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

/******************* MAIN *******************/

int main() {
    int x;
    AVLnode* avl_root = NULL;
    AVPNode* avp_root = NULL;

    while (scanf("%d", &x) && x >= 0) {
        avl_root = insert_avl_node(avl_root, x);
        avp_root = insert_avp_node(avp_root, x);
    }

    print_avl_tree_heights(avl_root);
    print_avp_tree_heights(avp_root);
    printf("%d\n", get_black_height(avp_root));
    printf("%d, %d, %d\n", color_changes, avp_rotations, avl_rotations);

    free_avl(avl_root);
    free_avp(avp_root);
    return 0;
}