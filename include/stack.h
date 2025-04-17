#ifndef TQ_STACK_H
#define TQ_STACK_H

#include "types.h"

typedef struct Stack
{
    void** items;
    usize size;
    usize capacity;
} Stack;

void stack_init(Stack* stack);

void stack_push(Stack* stack, void* item);

void* stack_peek(const Stack* stack);

void* stack_pop(Stack* stack);

#endif
