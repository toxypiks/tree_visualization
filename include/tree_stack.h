#ifndef STACK_H_
#define STACK_H_

#include "tree.h"

typedef struct TreeStackElement {
    Node* value;
    struct TreeStackElement *prev;
} TreeStackElement;

typedef struct TreeStack {
    TreeStackElement *top;
} TreeStack;

TreeStackElement* create_stack_element(Node* value);
TreeStack* create_stack();
bool is_empty(TreeStack *stack);
void stack_push(TreeStack *stack, Node* value);
Node* stack_pop(TreeStack *stack);
void stack_print(TreeStack* stack);

#endif // STACK_H_
