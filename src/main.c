#include <stdio.h>
#include "raylib.h"
#include "tree.h"
#include "tree_stack.h"
#include "tree_queue.h"
#include "doubly_linked_list.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "ui_stuff.h"


void tree_algo_step(Node **tree)
{
    int rand_num = rand() % 30;
    tree_insert(tree, rand_num);
}

void calc_tree_state(TreeState* tree_state)
{
    tree_state->tree_map = calc_tree_poses(tree_state->tree, 0, &(tree_state->max_radius));

    get_edges(tree_state->tree, &(tree_state->edges));

    tree_state->edge_coords = translate_edges_to_coordinates(tree_state->edges, tree_state->tree_map);
}

Node* tree_copy(Node *tree)
{
    Node *new_tree = create_node(tree->data);

    if (tree->left) {
        new_tree->left = tree_copy(tree->left);
    }
    if (tree->right) {
        new_tree->right = tree_copy(tree->right);
    }
    return new_tree;
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

    TreeQueue* test = create_queue();
    queue_enqueue(test, tree);
    queue_enqueue(test, tree->left);
    queue_enqueue(test, tree->right);
    print_queue(test);

    printf("\n");
    queue_dequeue(test);
    queue_dequeue(test);
    print_queue(test);

    TreeDepthMap *tree_depth_map = NULL;
    get_depth(tree, &tree_depth_map);

    for (size_t i = 0; i < hmlen(tree_depth_map); ++i) {
        printf("node:%d depth: %d balance: %d\n",
               tree_depth_map[i].key->data,
               tree_depth_map[i].value,
               get_balance(tree_depth_map[i].key,
                           tree_depth_map));
    }
    printf("\n");
    // 10, 2, 1, 5, 17
    tree_print_preorder(tree);
    printf("---------------------\n");

    float max_radius = 0.0f;
    tree_map = calc_tree_poses(tree, 0, &max_radius);

    Edge *edges = NULL;
    get_edges(tree, &edges);

    EdgeCoordinates *edge_coords = translate_edges_to_coordinates(edges, tree_map);

    TreeList *tree_list = create_list();

    // create first element:
    TreeState tree_state = {
        .tree = tree,
        .tree_map = tree_map,
        .tree_depth_map = tree_depth_map,
        .edges = edges,
        .edge_coords = edge_coords,
        .max_radius = max_radius
    };
    list_push_first(tree_list, tree_state);

    size_t screen_width = 800;
    size_t screen_height = 600;

    /*SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "tree_visualization");

    UiStuff* ui_stuff = create_ui_stuff(screen_width, screen_height);
    LayoutStack ls = {0};

    float w = GetRenderWidth();
    float h = GetRenderHeight();*/

    /*while (!WindowShouldClose()) {
        if(IsKeyPressed(KEY_RIGHT)) {
            increment_current(tree_list);
        }
        if(IsKeyPressed(KEY_LEFT)) {
            decrement_current(tree_list);
        }
        if(IsKeyPressed(KEY_N)) { // TODO later Emacs keybindings
            // TODO push new TreeLE to list

            TreeState new_tree_state = {0};
            new_tree_state.tree = tree_copy(tree_list->last->tree_state.tree);
            // copy old tree in new state
            tree_algo_step(&(new_tree_state.tree));
            calc_tree_state(&new_tree_state);
            get_depth(new_tree_state.tree, &(new_tree_state.tree_depth_map));

            for (size_t i = 0; i < hmlen(new_tree_state.tree_depth_map); ++i) {
                printf("node:%d depth: %d\n",
                       new_tree_state.tree_depth_map[i].key->data,
                       new_tree_state.tree_depth_map[i].value);
            }


            list_push_last(tree_list, new_tree_state);
        }
        TreeLE *current = get_current(tree_list);
        TreeState tree_state_print = current->tree_state;
        BeginDrawing();
        layout_stack_push(&ls, LO_VERT, ui_rect(0, 0, w, h), 3, 0);
        UiRect rect0 = layout_stack_slot(&ls);
        UiRect rect1 = layout_stack_slot(&ls);
        UiRect rect_total = rect_combine(rect0, rect1, LO_VERT);
        tree_widget(rect_total, &tree_state_print, background);
        tree_list_widget(layout_stack_slot(&ls), tree_list, background);
        ClearBackground(background);
        EndDrawing();
    }
    CloseWindow();*/
    return 0;
}
