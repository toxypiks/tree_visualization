#ifndef AVL_TREE_H_
#define AVL_TREE_H_

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

AVLNode* create_avl_node(int key);
int get_height(AVLNode *node);
int get_height_recursive(AVLNode* node);
AVLNode* right_rotate(AVLNode *y);
AVLNode* left_rotate(AVLNode *x);
int get_balance(AVLNode *n);
AVLNode* avl_insert(AVLNode *root, int key);
AVLNode* find_min_value_node(AVLNode* node);
AVLNode* delete_node(AVLNode* root, int key);
void print_preorder(AVLNode *root);

#endif // AVL_TREE_H_
