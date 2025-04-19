#include "event_listener.h"

#include <stdlib.h>

EventListener* event_listener_create(const char* event_name, int callback)
{
    EventListener* listener = malloc(sizeof(EventListener));

    listener->event_name = event_name;
    listener->callback = callback;

    return listener;
}
