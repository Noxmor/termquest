#include "stack.h"

#include <stdlib.h>

void stack_init(Stack* stack)
{
    stack->items = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

void stack_push(Stack* stack, void* item)
{
    if (stack->size >= stack->capacity)
    {
        stack->capacity = stack->capacity > 0 ? stack->capacity * 2 : 1;
        stack->items = realloc(stack->items, sizeof(void*) * stack->capacity);
    }

    stack->items[stack->size++] = item;
}

void* stack_peek(const Stack* stack)
{
    if (stack->size == 0)
    {
        return NULL;
    }

    return stack->items[stack->size - 1];
}

void* stack_pop(Stack* stack)
{
    void* item = stack_peek(stack);

    if (stack->size > 0)
    {
        --stack->size;
    }

    return item;
}
