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

typedef struct TreeDepthMap {
    Node *key;
    int value;
} TreeDepthMap;

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

typedef struct TreeState {
    Node *tree;
    TreeMap *tree_map;
    Edge *edges;
    EdgeCoordinates *edge_coords;
    float max_radius;
} TreeState;

Node* create_node (int data);
void tree_insert(Node **tree, int data);
void tree_print_preorder(Node* tree);
NodePos get_node_pos(Node* tree, TreeMap **tree_map, float layer, float x_offset);
int get_depth(Node* tree, TreeDepthMap **tree_depth_map);
void normalize_values(TreeMap *tree_map, float max_x, float max_y);
void get_edges(Node* tree, Edge** edges);
EdgeCoordinates* translate_edges_to_coordinates(Edge* edges, TreeMap* tree_map);
void print_hash_map(TreeMap *tree_map);
float max_layer(TreeMap *tree_map);
TreeMap* calc_tree_poses(Node* tree, float layer, float* max_radius);

#endif // TREE_H_
