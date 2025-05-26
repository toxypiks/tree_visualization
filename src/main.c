#include <stdio.h>
#include "raylib.h"
#include "tree.h"
#include "stack.h"


float dist_xl(Node* tree, Stack* stack, float layer, float x_offset);

float dist_xr(Node* tree, Stack* stack, float layer, float x_offset){
    //basisfall
    if(tree->right == NULL) {
        return 0.0f;
    }
    //rekursionsschritt
    float dist_xl_calc = dist_xl(tree->right, stack, layer+1.0f, x_offset);
    float dist_xr_calc = dist_xr(tree->right, stack, layer+1.0f, x_offset + dist_xl_calc);
    float dist = dist_xl_calc + dist_xr_calc + 1.0f;
    stack_push(stack, CLITERAL(Vector3) {
        .x = dist + x_offset,
        .y = layer,
        .z = (float)tree->data,
    });
    return dist;
}

float dist_xl(Node* tree, Stack* stack, float layer, float x_offset){
    //basisfall
    if(tree->left == NULL) {
        return 0.0f;
    }
    //rekursionsschritt
    float dist_xl_calc = dist_xl(tree->left, stack, layer+1.0f, x_offset);
    float dist_xr_calc = dist_xr(tree->left, stack, layer+1.0f, x_offset + dist_xl_calc + 1.0f);
    float dist = dist_xl_calc + dist_xr_calc + 1.0f;
    stack_push(stack, CLITERAL(Vector3) {
        .x = dist + x_offset,
        .y = layer,
        .z = (float)tree->data,
    });
    return dist;
}

void visualize_tree(Node* tree, float layer) {
    Stack* stack = create_stack();
    float tree_x = dist_xl(tree, stack, layer, 0);
    float tree_y = layer;
    float egal = dist_xr(tree, stack, layer, tree_x);
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
