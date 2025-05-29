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

TreeMap* calc_tree_poses(Node* tree, float layer, float* max_radius) {
    TreeMap *tree_map = NULL;
    NodePos tree_pos = get_node_pos(tree, &tree_map, layer, 0);
    float max_x = tree_pos.distl + 1.0f + tree_pos.distr;
    float max_y = max_layer(tree_map);
    normalize_values(tree_map, max_x, max_y);
    *max_radius = 1.0/max_x;
    return tree_map;
}

int main(void)
{
    Color background = GetColor(0x181818FF);
    int root_data = 10;
    Node *tree = create_node(root_data);
    TreeMap *tree_map = NULL;

    tree_insert(&tree, 2);
    tree_insert(&tree, 5);
    tree_insert(&tree, 17);
    tree_insert(&tree, 1);
    tree_insert(&tree, 4);
    tree_insert(&tree, 21);
    tree_insert(&tree, 12);
    tree_insert(&tree, 14);
    tree_insert(&tree, 3);


    // 10, 2, 1, 5, 17
    tree_print_preorder(tree);
    printf("---------------------\n");

    float max_radius = 0.0f;
    tree_map = calc_tree_poses(tree, 0, &max_radius);

    size_t screen_width = 800;
    size_t screen_height = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "tree_visualization");

    float w = GetRenderWidth();
    float h = GetRenderHeight();

    while (!WindowShouldClose()) {
        BeginDrawing();
        for (size_t i = 0; i < hmlen(tree_map); ++i) {
            int circle_x = (int)(tree_map[i].value.x * w);
            int circle_y = (int)(tree_map[i].value.y * h);
            float radius = 0.4f*max_radius*w*0.5f;
            DrawCircle(circle_x, circle_y, radius, GREEN);
            DrawCircle(circle_x, circle_y, 0.9f*radius, background);

            char text[8];
            sprintf(text, "%d",tree_map[i].key->data);
            int len_pos = strlen(text)*0.2 + 0.3f;
            float font_size = 0.8f * radius;
            DrawText(text, circle_x - radius*len_pos, circle_y - radius*0.3f, font_size, GREEN);
        }
        ClearBackground(background);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
