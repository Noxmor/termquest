#include "prototype_registry.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>

#define TQ_PROTOTYPE_REGISTRY_CAPACITY 512

typedef struct PrototypeRegistryEntry
{
    void* proto;
    const char* name;
    struct PrototypeRegistryEntry* next;
} PrototypeRegistryEntry;

typedef struct PrototypeRegistry
{
    PrototypeRegistryEntry* entries[TQ_PROTOTYPE_REGISTRY_CAPACITY];
} PrototypeRegistry;

static PrototypeRegistry interface_registry;

static void* prototype_registry_find(PrototypeRegistry* registry,
                                     const char* name)
{
    u64 index = hash_djb2(name) % TQ_PROTOTYPE_REGISTRY_CAPACITY;
    PrototypeRegistryEntry* entry = registry->entries[index];

    while (entry != NULL)
    {
        if (strcmp(entry->name, name) == 0)
        {
            return entry->proto;
        }
    }

    return NULL;
}

static void prototype_registry_insert(PrototypeRegistry* registry,
                                      void* proto, const char* name)
{
    void* found = prototype_registry_find(registry, name);
    if (found != NULL)
    {
        // TODO: Overwrite existing prototype
        return;
    }

    u64 index = hash_djb2(name) % TQ_PROTOTYPE_REGISTRY_CAPACITY;
    PrototypeRegistryEntry* entry = registry->entries[index];

    PrototypeRegistryEntry* new_entry = malloc(sizeof(PrototypeRegistryEntry));
    new_entry->proto = proto;
    new_entry->name = name;
    new_entry->next = entry;

    registry->entries[index] = new_entry;
}

void interface_registry_insert(Interface* inf)
{
    prototype_registry_insert(&interface_registry, inf, inf->name);
}

Interface* interface_registry_find(const char* name)
{
    return prototype_registry_find(&interface_registry, name);
}
