#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "doubly_linked_list.h"
#include "tree.h"

TreeLE* _create_node(TreeState tree_state)
{
    TreeLE *node = malloc(sizeof(TreeLE));
    node->tree_state = tree_state;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

TreeList* create_list()
{
    TreeList *new_list = malloc(sizeof(TreeList));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->current = NULL;
    new_list->length = 0;
    return new_list;
}

bool list_is_empty(TreeList *list)
{
    if (!list->length) {
        return true;
    }
    return false;
}

int list_len(TreeList *list)
{
    return list->length;
}

TreeLE* get_current(TreeList *list)
{
    return list->current;
}

void increment_current(TreeList *list)
{
    list->current = list->current->next ? list->current->next : list->current;
}

void decrement_current(TreeList *list)
{
    list->current = list->current->prev ? list->current->prev : list->current;
}

void list_push_first(TreeList *list, TreeState tree_state)
{
    TreeLE *new_node = _create_node(tree_state);
    TreeLE *first_node = list->first;

    if (first_node == NULL) {
        list->last = new_node;
        list->current = new_node;
    } else {
        first_node->prev = new_node;
    }
    list->first = new_node;
    list->first->next = first_node;
    list->length++;
}

void list_push_last(TreeList *list, TreeState tree_state)
{
    TreeLE *new_node = _create_node(tree_state);
    TreeLE *first_node = list->first;
    TreeLE *current = list->last;
    list->last = new_node;

    if(first_node == NULL) {
        list->first = new_node;
        list->length++;
        return;
    }
    current->next = new_node;
    list->last->prev = current;
    list->length++;
}

void list_push_to_position(TreeList *list, TreeState tree_state, int position)
{
    TreeLE *new_node = _create_node(tree_state);
    TreeLE *current = list->first;
    TreeLE *tmp_current = list->first;

    if (position == list->length) {
        list_push_last(list, tree_state);
        return;
    }
    if (list->length == 0 && position > 0 || list->length > 0 && position > list->length)  {
        printf("ERROR: position out of range\n");
        return;
    }
    if (list->length == 0 && position == 0 || list->length > 0 && position == 0) {
        list_push_first(list, tree_state);
        return;
    }
    else {
        for (int i = 0; i < position - 2; ++i) {
            current = current->next;
        }
    }
    tmp_current = current;
    current = current->next;
    tmp_current->next = new_node;
    new_node->next = current;
    current->prev = new_node;
    new_node->prev = tmp_current;
    list->length++;
}

void list_delete_first(TreeList *list)
{
    TreeLE *first = list->first;
    if (list->length == 0) {
        printf("ERROR: List already empty\n");
        return;
    }
    else if (list->length == 1) {
        free(list->last);
        list->last = NULL;
        list->first = NULL;
        list->current = NULL;
        list->length--;
        return;
    }
    if (list->first == list->current) {
        list->current = list->first->next;
    }

    TreeLE *second = list->first->next;
    list->first = second;
    list->first->prev = NULL;
    free(first);
    list->length--;
}

void list_delete_last(TreeList *list)
{
    TreeLE *last = list->last;
    if (list->length == 0) {
        printf("ERROR: TreeList already empty\n");
        return;
    }
    else if (list->length == 1) {
        free(list->last);
        list->last = NULL;
        list->first = NULL;
        list->current = NULL;
        list->length--;
        return;
    }
    if (list->last == list->current) {
        list->current = list->last->prev;
    }

    TreeLE *penultimate = last->prev;
    penultimate->next = NULL;
    free(last);
    last = NULL;
    list->length--;
}

void list_delete_at_position(TreeList *list, int position)
{
    TreeLE *tmp = list->first;

    if (tmp == NULL) {
        printf("ERROR: List already empty\n");
        return;
    }
    if (position == list->length) {
        list_delete_last(list);
        return;
    }
    if (list->length == 0 && position > 0 || list->length > 0 && position > list->length)  {
        printf("ERROR: Position out of range\n");
        return;
    }
    if (list->length == 0 && position == 0 || list->length > 0 && position == 0) {
        list_delete_first(list);
        return;
    }
    else {
        for (int i = 0; i < position - 2; ++i) {
            tmp = tmp->next;
        }
    }

    if (tmp == list->current) {
        list->current = tmp->next;
    }
    TreeLE *tmp_next_next = tmp->next->next;
    free(tmp->next);
    tmp->next = tmp_next_next;
    tmp_next_next->prev = tmp;
    list->length--;
}

void print_list(TreeList *list)
{
    TreeLE *tmp = list->first;
    while (tmp != NULL) {
        printf("%d,", tmp->tree_state);
        tmp = tmp->next;
    }
    printf("\n");
}
