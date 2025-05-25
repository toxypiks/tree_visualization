#include <stdio.h>
#include "raylib.h"
#include "tree.h"

float dist_xl(Node* tree);

float dist_xr(Node* tree){
    //basisfall
    if(tree->right == NULL) {
        return 0.0f;
    }
    //rekursionsschritt
    return dist_xl(tree->right) + dist_xr(tree->right) + 2.0f;
}

float dist_xl(Node* tree){
    //basisfall
    if(tree->left == NULL) {
        return 0.0f;
    }
    //rekursionsschritt
    return dist_xl(tree->left) + dist_xr(tree->left) + 2.0f;
}

void visualize_tree(Node* tree, float layer) {
    float tree_x = dist_xl(tree);
    float tree_y = layer;
    // ...
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
    printf("---------");
    float left10 = dist_xl(tree);
    float left2 = dist_xl(tree->left);
    float left1 = dist_xl(tree->left->left);
    float left5 = left2 + 1 + dist_xl(tree->left->right);

    printf("left10: %f,left2: %f, left1: %f, left5: %f \n", left10, left2, left1, left5);

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
