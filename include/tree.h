#ifndef TREE_H_
#define TREE_H_

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct NodePos {
    float x;
    float y;
    float distl;
    float distr;
} NodePos;

typedef struct TreeMap {
    Node *key;
    NodePos value;
} TreeMap;

typedef struct Edge {
    Node* start;
    Node* end;
} Edge;

typedef struct EdgeCoordinates {
    float start_x;
    float start_y;
    float end_x;
    float end_y;
} EdgeCoordinates;

Node* create_node (int data);
void tree_insert(Node **tree, int data);
void tree_print_preorder(Node* tree);

#endif // TREE_H_
