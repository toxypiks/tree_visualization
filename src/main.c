#include <stdio.h>
#include "raylib.h"
#include "tree.h"
#include "stack.h"
#include "doubly_linked_list.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "ui_stuff.h"

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
        .edges = edges,
        .edge_coords = edge_coords,
        .max_radius = max_radius
    };
    list_push_first(tree_list, tree_state);

    size_t screen_width = 800;
    size_t screen_height = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "tree_visualization");

    UiStuff* ui_stuff = create_ui_stuff(screen_width, screen_height);
    LayoutStack ls = {0};

    float w = GetRenderWidth();
    float h = GetRenderHeight();

    while (!WindowShouldClose()) {
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

            list_push_last(tree_list, new_tree_state);
        }
        TreeLE *current = get_current(tree_list);
        TreeState tree_state_print = current->tree_state;
        BeginDrawing();
        layout_stack_push(&ls, LO_VERT, ui_rect(0, 0, w, h), 2, 0);
        current_tree_widget(layout_stack_slot(&ls), tree_list, background);
        widget(layout_stack_slot(&ls), PINK);
        ClearBackground(background);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
