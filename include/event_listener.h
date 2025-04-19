#ifndef TQ_EVENT_LISTENER
#define TQ_EVENT_LISTENER

typedef struct EventListener
{
    const char* event_name;
    int callback;
} EventListener;

EventListener* event_listener_create(const char* event_name, int callback);

#endif
