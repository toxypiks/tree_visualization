#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Node* create_node (int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void tree_insert(Node **tree, int data) {
    Node *tmp = *tree;
    bool found = false;

    while(!found)
    {
        if (data < tmp->data) {
            if (tmp->left != NULL) {
                tmp = tmp->left;
            } else {
                found = true;
            }
        }
        else if (data == tmp->data) {
            found = true;
        }
        else {
            if (tmp->right != NULL) {
                tmp = tmp->right;
            }
            else {
                found = true;
            }
        }
    }
    if (data != tmp->data) {
        Node *new_node = create_node(data);
        if (data < tmp->data) {
            tmp->left = new_node;
        } else {
            tmp->right = new_node;
        }
    }
}

void tree_print_preorder(Node* tree)
{
    if (tree != NULL) {
       printf("%d\n", tree->data);
       tree_print_preorder(tree->left);
       tree_print_preorder(tree->right);
    }
}
