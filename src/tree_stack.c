#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tree_stack.h"
#include "tree.h"

TreeStackElement* create_stack_element(Node* value)
{
    TreeStackElement *new_stack_element = malloc(sizeof(TreeStackElement));
    new_stack_element->value = value;
    new_stack_element->prev = NULL;
    return new_stack_element;
}

TreeStack* create_stack()
{
    TreeStack *new_stack = malloc(sizeof(TreeStack));
    new_stack->top = NULL;
    return new_stack;
}

bool is_empty(TreeStack *stack)
{
    return stack->top == NULL ? true : false;
}

void stack_push(TreeStack *stack, Node* value)
{
    TreeStackElement *new_element = create_stack_element(value);
    new_element->prev = stack->top;
    stack->top = new_element;
}

Node* stack_pop(TreeStack *stack)
{
    if (is_empty(stack)) {
        return NULL;
    }
    TreeStackElement *remove = stack->top;
    stack->top = remove->prev;
    Node* node_remove = remove->value;
    free(remove);
    return node_remove;
}

void stack_print(TreeStack* stack)
{
    TreeStackElement* tmp = stack->top;
    while(tmp !=NULL) {
        printf("(%d)\n", tmp->value->data);
        tmp = tmp->prev;
    }
}
