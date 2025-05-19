#include<stdio.h>
#include<stdlib.h>

typedef struct node {
    int value;
    int height;
    struct node *left;
    struct node *right;
} node;

void printInOrder(node* r) {
    if (r != NULL) {
        printInOrder(r->left);
        printf("%d ", r->value);
        printInOrder(r->right);
    }
}

// Function to get height of the node
int getHeight(struct node* n) {
    if (n == NULL) return 0;
    return n->height;
}

// Function to create a new node
struct node* createNode(int value) {
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Utility function to get the maximum of two integers
int max(int a, int b) { 
    return (a > b) ? a : b;
}

// Function to get balance factor of a node
int getBalanceFactor(struct node* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Right rotation function
struct node* rightRotate(struct node* y) {
    struct node* x = y->left;
    struct node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Left rotation function
struct node* leftRotate(struct node* x) {
    struct node* y = x->right;
    struct node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Function to insert a value into AVL tree
struct node* insert_node(struct node* node, int value) {
    // 1. Perform standard BST insertion
    if (node == NULL) return createNode(value);

    if (value < node->value) {
        node->left = insert_node(node->left, value);
    } else if (value > node->value) {
        node->right = insert_node(node->right, value);
    } else {  // Equal values are not allowed in BST
        return node;
    }

    // 2. Update height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get the balance factor of this ancestor node to
    // check whether this node became unbalanced
    int balance = getBalanceFactor(node);

    // 4. If the node becomes unbalanced, then there are 4
    // cases

    // Left Left Case
    if (balance > 1 && value < node->left->value)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && value > node->right->value)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && value > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && value < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

// Function to perform preorder traversal of AVL tree
void inOrder(struct node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->value);
        inOrder(root->right);
    }
}

int main() {
    int n;
    node *root = NULL;

    while (scanf("%d", &n) && n > 0) {
        root = insert_node(root, n);
    }

    printInOrder(root);

    return 0;
}