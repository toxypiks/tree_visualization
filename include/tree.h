#ifndef TREE_H_
#define TREE_H_

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node* create_node (int data);
void tree_insert(Node **tree, int data);
void tree_print_preorder(Node* tree);

#endif // TREE_H_
