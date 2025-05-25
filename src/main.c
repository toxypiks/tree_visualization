#include <stdio.h>
#include "raylib.h"
#include "tree.h"

int main(void)
{
    int root_data = 10;
    Node *tree = create_node(root_data);

    tree_insert(&tree, 2);
    tree_insert(&tree, 5);
    tree_insert(&tree, 17);
    tree_insert(&tree, 1);

    // 10, 2, 1, 5, 17
    tree_print_preorder(tree);

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
    return 0;
}
