#include "hash.h"

u64 hash_djb2(const char* str)
{
    u64 hash = 5381;
    char c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
