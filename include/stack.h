#ifndef STACK_H_
#define STACK_H_

#include "raylib.h"

typedef struct StackElement {
    Vector3 value;
    struct StackElement *prev;
} StackElement;

typedef struct Stack {
    StackElement *top;
} Stack;

StackElement* create_stack_element(Vector3 value);
Stack* create_stack();
bool is_empty(Stack *stack);
void stack_push(Stack *stack, Vector3 value);
StackElement* stack_pop(Stack *stack);
void stack_print(Stack* stack);

#endif // STACK_H_
