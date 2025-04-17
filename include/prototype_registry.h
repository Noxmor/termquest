#ifndef TQ_PROTOTYPE_REGISTRY_H
#define TQ_PROTOTYPE_REGISTRY_H

#include "interface.h"

void interface_registry_insert(Interface* inf);

Interface* interface_registry_find(const char* name);

#endif
