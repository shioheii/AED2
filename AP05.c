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

void frees_allocated_memory(AVLnode* root) {
    if (root != NULL) {
        frees_allocated_memory(root->left);
        frees_allocated_memory(root->right);
        free(root);
    }
}

// Function to perform preorder traversal of AVL tree
void print_in_order(AVLnode* root) {
    if (root != NULL) {
        print_in_order(root->left);
        printf("%d ", root->value);
        print_in_order(root->right);
    }
}

// Function to get height of the node
int get_height(AVLnode* n) {
    if (n == NULL) return 0;
    return n->height;
}

// Function to create a new node
AVLnode* create_node(int value) {
    AVLnode* node = (struct AVLnode*)malloc(sizeof(struct AVLnode));
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Utility function to get the maximum of two integers
int max(int a, int b) { 
    return a > b ? a : b;
}

// Function to get balance factor of a node
int get_balance_factor(AVLnode* n) {
    if (n == NULL) return 0;
    return get_height(n->left) - get_height(n->right);
}

// Right rotation function
AVLnode* right_rotate(AVLnode* y) {
    AVLnode* x = y->left;
    AVLnode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

// Left rotation function
AVLnode* left_rotate(AVLnode* x) {
    AVLnode* y = x->right;
    AVLnode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

// Function to insert a value into AVL tree
AVLnode* insert_node(AVLnode* node, int value) {
    if (node == NULL) return create_node(value);

    if (value < node->value) {
        node->left = insert_node(node->left, value);
    } else if (value > node->value) {
        node->right = insert_node(node->right, value);
    } else {
        return node;
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance_factor(node);

    if (balance > 1 && value < node->left->value)
        return right_rotate(node);

    if (balance < -1 && value > node->right->value)
        return left_rotate(node);

    if (balance > 1 && value > node->left->value) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && value < node->right->value) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

AVLnode* finds_successor(AVLnode* node) {
    while (node->left != NULL)
        node = node->left;

    return node;
}

// Function to delete a value in AVL tree
AVLnode* delete_node(AVLnode* root, int value) {
    if (root == NULL) return root;

    if (value < root->value) {
        root->left = delete_node(root->left, value);
    } else if (value > root->value) {
        root->right = delete_node(root->right, value);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
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

    if (root == NULL) return root;
    root->height = 1 + max(get_height(root->left), get_height(root->right));
    int balance = get_balance_factor(root);

    if (balance > 1 && get_balance_factor(root->left) >= 0){
        return right_rotate(root);
    }

    if (balance > 1 && get_balance_factor(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance_factor(root->right) <= 0){
        return left_rotate(root);
    }

    if (balance < -1 && get_balance_factor(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

AVLnode* search_node(AVLnode* root, int value) {
    if (root == NULL)
        return NULL;

    if (root->value == value)
        return root;

    if (value < root->value) 
        return search_node(root->left, value);

    return search_node(root->right, value);
}

void insert_or_remove_value(AVLnode** root, int value) {
    AVLnode* temp = search_node(*root, value);

    if (temp != NULL)
        *root = delete_node(*root, value);
    else
        *root = insert_node(*root, value);
}

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

void print_values_in_range(AVLnode* root, int low, int high, int* found) {
    if (root == NULL) return;

    if (root->value > low)
        print_values_in_range(root->left, low, high, found);

    if (root->value >= low && root->value <= high) {
        if (*found == 0) {
            *found = 1;
            printf("%d", root->value);
        } else {
            printf(", %d", root->value);
        }
    }

    if (root->value < high)
        print_values_in_range(root->right, low, high, found);
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

    int found = 0;
    print_values_in_range(root, startRange, endRange, &found);
    if (!found) printf("NADA A EXIBIR");
    printf("\n");

    if (found) {
        AVLnode* temp;
        for (int i = startRange; i <= endRange; i++) {
            temp = search_node(root, i);
            if (temp != NULL) {
                print_tree_heights(temp);
            }
        }
        free(temp);
    }

    // frees_allocated_memory(root);

    return 0;
}
