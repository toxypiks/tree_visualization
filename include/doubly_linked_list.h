#ifndef DOUBLY_LINKED_LIST_H_
#define DOUBLY_LINKED_LIST_H_

#include "tree.h"

typedef struct TreeLE {
    TreeState tree_state;
    struct TreeLE *prev;
    struct TreeLE *next;
} TreeLE;

typedef struct TreeList {
    TreeLE *first;
    TreeLE *last;
    size_t length;
} TreeList;

TreeList* create_list();
bool list_is_empty(TreeList *list);
int list_len(TreeList *list);
void list_push_first(TreeList *list, TreeState tree_state);
void list_push_last(TreeList *list, TreeState tree_state);
void list_push_to_position(TreeList *list, TreeState tree_state, int position);
void list_delete_first(TreeList *list);
void list_delete_last(TreeList *list);
void list_delete_at_position(TreeList *list, int position);
void print_list(TreeList *list);

#endif // DOUBLY_LINKED_LIST_H_
