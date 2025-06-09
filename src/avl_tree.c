#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

AVLNode* create_avl_node(int key)
{
    AVLNode *node = malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int max (int a, int b)
{
    return (a > b) ? a : b;
}

int get_height(AVLNode *node)
{
    return node == NULL ? 0 : node->height;
}

int get_height_recursive(AVLNode* node)
{
    int height_right = 0;
    if (node->right)
    {
        height_right = get_height_recursive(node->right);
    }

    int height_left = 0;
    if (node->left)
    {
        height_left = get_height_recursive(node->left);
    }

    int height = max (height_right, height_left) + 1;

    return height;
}

AVLNode* right_rotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // right rotation
    x->right = y;
    y->left = T2;

    // update heights
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVLNode* left_rotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // left rotate
    y->left = x;
    x->right = T2;

    // update heights
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

int get_balance(AVLNode *n)
{
    return n == NULL ? 0 : get_height(n->left) - get_height(n->right);
}

AVLNode* avl_insert(AVLNode *root, int key)
{
    // Insertion of new node
    if (root == NULL) {
        return create_avl_node(key);
    }

    if (key < root->key) {
        root->left = avl_insert(root->left, key);
    }
    else if (key > root->key) {
        root->right = avl_insert(root->right, key);
    } else {
        return root; // equal keys are not allowed
    }

    // Update height of ancestor node
    root->height = max(get_height(root->left), get_height(root->right)) + 1;

    // Get balance factor of ancestor to check if its unbalanced
    int balance = get_balance(root);

    // 4 unbalanced cases

    if (balance > 1 && key < root->left->key) {
        return right_rotate(root);
    }

    if (balance < -1 && key > root->right->key) {
        return left_rotate(root);
    }

    if (balance > 1 && key > root->left->key) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && key < root->right->key) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

AVLNode* find_min_value_node(AVLNode* node)
{
    AVLNode* current = node;

    while(current->left != NULL) {
        current = current->left;
    }
    return current;
}

AVLNode* delete_node(AVLNode* root, int key)
{
    if (root == NULL) {
        return root;
    }

    if (key < root->key) {
        root->left = delete_node(root->left, key);
    }
    else if (key > root->key) {
        root->right = delete_node (root->right, key);
    }
    else {
        // key same as root -> root needs to be deleted
        if (root->left == NULL || root->right == NULL) {
            AVLNode* tmp = root->left? root->left : root->right;

            if (tmp == NULL)
            {
                tmp = root;
                root = NULL;
            }
            else {
                // one child
                *root = *tmp; // value of child becomes root value
                free(tmp);
            }
        }
        else { // node with two children
            AVLNode* tmp = find_min_value_node(root->right);
            root->key = tmp->key;
            root->right = delete_node(root->right, tmp->key);
        }
    }

    if (root == NULL) {
        return root;
    }

    root->height = 1 + max(get_height(root->left), get_height(root->right));
    int balance = get_balance(root);

    // 4 unbalanced cases

    if (balance > 1 && get_balance(root->left) >= 0) {
        return right_rotate(root);
    }

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) {
        return left_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void print_preorder(AVLNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        print_preorder(root->left);
        print_preorder(root->right);
    }
}
