#include <stdio.h>
#include "raylib.h"
#include "tree.h"
#include "stack.h"

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

NodePos get_node_pos(Node* tree, Stack* stack,float layer, float x_offset)
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
        node_pos_left = get_node_pos(tree->left, stack, layer + 1.0f, x_offset);
        dist_left = node_pos_left.distl + 1.0f + node_pos_left.distr;
    }

    NodePos node_pos_right = {0};
    float dist_right = 0.0f;
    if (tree->right) {
        node_pos_right = get_node_pos(tree->right, stack, layer + 1.0f, x_offset + 1.0f + dist_left);
        dist_right = node_pos_right.distl + 1.0f + node_pos_right.distr;
    }
    node_pos.x = x_offset + dist_left;
    node_pos.distl = dist_left;
    node_pos.distr = dist_right;

    stack_push(stack, CLITERAL(Vector3) {
        .x = node_pos.x,
        .y = node_pos.y,
        .z = (float)tree->data,
    });

    return node_pos;
}


void visualize_tree(Node* tree, float layer) {
    Stack* stack = create_stack();
    NodePos tree_pos = get_node_pos(tree, stack, layer, 0);
    // stack is ready with all points
    // lets print it!
    stack_print(stack);
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
