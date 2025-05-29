#include <stdio.h>
#include "raylib.h"
#include "tree.h"
#include "stack.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

/*
function ->
                     root
<-----distl---->(pos_x, pos_y)<---distr---->
                  /      \
                links   right
                          /  \
                         rr   rl
*/

typedef struct NodePos {
    float x;
    float y;
    float distl;
    float distr;
} NodePos;

// Hashmap stuff:
// key Node*
// value Pos
// hmput(my_hmap, key, NodePos)
//                 |         |
//                 v         v
//                 data      (x,y,...

typedef struct TreeMap {
    Node *key;
    NodePos value;
} TreeMap;

NodePos get_node_pos(Node* tree, TreeMap **tree_map,float layer, float x_offset)
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

void visualize_tree(Node* tree, float layer) {
    TreeMap *tree_map = NULL;
    NodePos tree_pos = get_node_pos(tree, &tree_map, layer, 0);
    float max_x = tree_pos.distl + 1.0f + tree_pos.distr;
    float max_y = max_layer(tree_map);
    normalize_values(tree_map, max_x, max_y);

    // hash_map is ready with all points
    // lets print it!
    print_hash_map(tree_map);
}

int main(void)
{
    int root_data = 10;
    Node *tree = create_node(root_data);

    tree_insert(&tree, 2);
    tree_insert(&tree, 5);
    tree_insert(&tree, 17);
    tree_insert(&tree, 1);
    tree_insert(&tree, 4);

    // 10, 2, 1, 5, 17
    tree_print_preorder(tree);
    printf("---------------------\n");
    visualize_tree(tree, 0);

    /*
    size_t screen_width = 800;
    size_t screen_height = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "tree_visualization");

    float w = GetRenderWidth();
    float h = GetRenderHeight();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        EndDrawing();
    }
    CloseWindow();
    */
    return 0;
}
