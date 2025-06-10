#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stb_ds.h"
#include "tree_stack.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

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

void tree_print_preorder_interative(Node* tree)
{
    TreeStack* tree_stack = create_stack();
    stack_push(tree_stack, tree);

    while (!is_empty(tree_stack)) {
        Node* tmp = stack_pop(tree_stack);
        printf("%d\n", tmp->data);
        if(tmp->right) {
            stack_push(tree_stack, tmp->right);
        }
        if(tmp->left) {
            stack_push(tree_stack, tmp->left);
        }

    }
}

NodePos get_node_pos(Node* tree, TreeMap **tree_map, float layer, float x_offset)
{
    NodePos node_pos = {
        .x = 0.0 + x_offset,
        .y = layer,
        .distl = 0.0f,
        .distr = 0.0f,
    };
    if (!tree) {
        return node_pos;
    }

    NodePos node_pos_left = {0};
    float dist_left = 0.0f;
    if (tree->left) {
        node_pos_left = get_node_pos(tree->left, tree_map, layer + 1.0f, x_offset);
        dist_left = node_pos_left.distl + 1.0f + node_pos_left.distr;
    }

    NodePos node_pos_right = {0};
    float dist_right = 0.0f;
    if (tree->right) {
        node_pos_right = get_node_pos(tree->right, tree_map, layer + 1.0f, x_offset + 1.0f + dist_left);
        dist_right = node_pos_right.distl + 1.0f + node_pos_right.distr;
    }
    node_pos.x = x_offset + dist_left;
    node_pos.distl = dist_left;
    node_pos.distr = dist_right;

    hmput(*tree_map, tree, node_pos);

    return node_pos;
}

int get_depth(Node* tree, TreeDepthMap **tree_depth_map)
{
   if (!tree) {
       return 0;
   }
   int depth_left = 0;
   if (tree->left) {
      depth_left = get_depth(tree->left, tree_depth_map);
   }
   int depth_right = 0;
   if (tree->right) {
      depth_right = get_depth(tree->right, tree_depth_map);
   }
   int depth = MAX(depth_left, depth_right) +1;
   hmput(*tree_depth_map, tree, depth);

   return depth;
}

int get_balance(Node *tree, TreeDepthMap *tree_depth_map)
{
    if(!tree) {
        return 0;
    }
    if(tree->left == NULL && tree->right == NULL) {
        return 0;
    }
    int depth_left_child = 0;
    if (tree->left) {
        depth_left_child = hmget(tree_depth_map, tree->left);
    }
    int depth_right_child = 0;
    if (tree->right) {
        depth_right_child = hmget(tree_depth_map, tree->right);
    }
    return depth_right_child - depth_left_child;
}

void normalize_values(TreeMap *tree_map, float max_x, float max_y)
{
    if(!tree_map){
        return;
    }
    for (size_t i = 0; i < hmlen(tree_map); ++i) {
        tree_map[i].value.x += 1.0f;
        tree_map[i].value.x /= (max_x + 1.0f);
        tree_map[i].value.y += 1.0f;
        tree_map[i].value.y /= (max_y + 1.0f);
    }
}

// position not pose
TreeMap* calc_tree_poses(Node* tree, float layer, float* max_radius) {
    TreeMap *tree_map = NULL;
    NodePos tree_pos = get_node_pos(tree, &tree_map, layer, 0);
    float max_x = tree_pos.distl + 1.0f + tree_pos.distr;
    float max_y = max_layer(tree_map);
    normalize_values(tree_map, max_x, max_y);
    *max_radius = 1.0/max_x;
    return tree_map;
}

void get_edges(Node* tree, Edge** edges)
{
    if(tree == NULL) {
        return;
    }
    if(tree->left != NULL) {
        arrput(*edges, ((Edge){
            .start=tree,
            .end=tree->left}
            ));
        get_edges(tree->left, edges);
    }
    if(tree->right != NULL) {
        arrput(*edges, ((Edge){
            .start=tree,
            .end=tree->right}
            ));
        get_edges(tree->right, edges);
    }
}

EdgeCoordinates* translate_edges_to_coordinates(Edge* edges, TreeMap* tree_map)
{
    EdgeCoordinates* edge_coords = NULL;
    for (size_t i = 0; i < arrlen(edges); ++i) {
        NodePos start_pos = hmget(tree_map, edges[i].start);
        NodePos end_pos = hmget(tree_map,edges[i].end);
        arrput(edge_coords, ((EdgeCoordinates){
            .start_x = start_pos.x,
            .start_y = start_pos.y,
            .end_x = end_pos.x,
            .end_y = end_pos.y
        }));
    }
    return edge_coords;
}

void print_hash_map(TreeMap *tree_map)
{
    for (size_t i = 0; i < hmlen(tree_map); ++i) {
        printf("key: %d, NodePos.x: %f, NodePos.y: %f\n", tree_map[i].key->data, tree_map[i].value.x, tree_map[i].value.y);
    }
}

float max_layer(TreeMap *tree_map)
{
    float max = 0.0f;
    for (size_t i = 0; i < hmlen(tree_map); ++i) {
        max = tree_map[i].value.y > max ? tree_map[i].value.y : max;
    }
    return max + 1.0f;
}
