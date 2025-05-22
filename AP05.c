#include<stdio.h>
#include<stdlib.h>

int const true = 1;
int const false = 0;

typedef struct node {
    int value;
    int height;
    struct node *left;
    struct node *right;
} node;

// Function to perform preorder traversal of AVL tree
void print_in_order(node* r) {
    if (r != NULL) {
        print_in_order(r->left);
        printf("%d ", r->value);
        print_in_order(r->right);
    }
}

// Function to get height of the node
int get_height(struct node* n) {
    if (n == NULL) return 0;
    return n->height;
}

// Function to create a new node
struct node* create_node(int value) {
    struct node* node = (struct node*)malloc(sizeof(struct node));
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
int get_balance_factor(struct node* n) {
    if (n == NULL) return 0;
    return get_height(n->left) - get_height(n->right);
}

// Right rotation function
struct node* right_rotate(struct node* y) {
    struct node* x = y->left;
    struct node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

// Left rotation function
struct node* left_rotate(struct node* x) {
    struct node* y = x->right;
    struct node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

// Function to insert a value into AVL tree
struct node* insert_node(struct node* node, int value) {
    // 1. Perform standard BST insertion
    if (node == NULL) return create_node(value);

    if (value < node->value) {
        node->left = insert_node(node->left, value);
    } else if (value > node->value) {
        node->right = insert_node(node->right, value);
    } else {  // Equal values are not allowed in BST
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

node* finds_successor(node* node) {
    while (node->left != NULL)
        node = node->left;

    return node;
}

// Function to delete a value in AVL tree
node* delete_node(node* root, int value) {
    if (root == NULL) return root;

    if (value < root->value) {
        root->left = delete_node(root->left, value);
    } else if (value > root->value) {
        root->right = delete_node(root->right, value);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp; 
            }
            free(temp);
        } else {
            node* temp = finds_successor(root->right);
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

int search(node* root, int value) {
    if (root == NULL)
        return false;

    if (root->value == value)
        return true;

    if (value < root->value) 
        return search(root->left, value);

    return search(root->right, value);
}

void insert_or_remove_value(node** root, int value) {
    int value_found = search(*root, value);

    if (value_found)
        *root = delete_node(*root, value);
    else
        *root = insert_node(*root, value);
}

int main() {
    int n;
    node *root = NULL;

    while (scanf("%d", &n) && n >= 0) {
        root = insert_node(root, n);
    }
    
    print_in_order(root);
    printf("\n");

    while (scanf("%d", &n) && n >= 0) {
        insert_or_remove_value(&root, n);
        print_in_order(root);
        printf("\n");
    }

    // print_in_order(root);

    return 0;
}