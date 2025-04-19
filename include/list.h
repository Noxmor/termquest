#ifndef TQ_LIST_H
#define TQ_LIST_H

#include "types.h"

typedef struct List
{
    void** items;
    usize size;
    usize capacity;
} List;

void list_init(List* list);

void list_add(List* list, void* item);

void* list_get(const List* list, usize index);

usize list_size(const List* list);

#endif
