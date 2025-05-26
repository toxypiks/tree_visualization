#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"
#include "raylib.h"

StackElement* create_stack_element(Vector3 value)
{
    StackElement *new_stack_element = malloc(sizeof(StackElement));
    new_stack_element->value = value;
    new_stack_element->prev = NULL;
    return new_stack_element;
}

Stack* create_stack()
{
    Stack *new_stack = malloc(sizeof(Stack));
    new_stack->top = NULL;
    return new_stack;
}

bool is_empty(Stack *stack)
{
    return stack->top == NULL ? true : false;
}

void stack_push(Stack *stack, Vector3 value)
{
    StackElement *new_element = create_stack_element(value);
    new_element->prev = stack->top;
    stack->top = new_element;
}

StackElement* stack_pop(Stack *stack)
{
    if (is_empty(stack)) {
        return NULL;
    }
    StackElement *remove = stack->top;
    stack->top = remove->prev;
    return remove;
}

void stack_print(Stack* stack)
{
    StackElement* tmp = stack->top;
    while(tmp !=NULL) {
        printf("(%f, %f, %f)\n", tmp->value.x, tmp->value.y, tmp->value.z);
        tmp = tmp->prev;
    }
}
