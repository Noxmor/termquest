#include "prototype_registry.h"

#define TQ_PROTOTYPE_REGISTRY_CAPACITY 512

typedef struct PrototypeRegistryEntry
{
    void* proto;
    struct PrototypeRegistryEntry* next;
} PrototypeRegistryEntry;

typedef struct PrototypeRegistry
{
    PrototypeRegistryEntry* entries[TQ_PROTOTYPE_REGISTRY_CAPACITY];
} PrototypeRegistry;

static PrototypeRegistry interface_registry;

static void prototype_registry_insert(void* prototype, const char* name)
{
    // TODO: Implement
}

static void* prototype_registry_find(PrototypeRegistry* registry, const char* name)
{
    // TODO: Implement
    return NULL;
}

void interface_registry_insert(Interface* inf)
{
    prototype_registry_insert(&interface_registry, inf->name);
}

Interface* interface_registry_find(const char* name)
{
    return prototype_registry_find(&interface_registry, name);
}
