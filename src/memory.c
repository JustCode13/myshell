#include "../include/memory.h"
#include <stddef.h>
#include <stdint.h>

static size_t allocated_bytes; // keeps track of total allocated memory

static size_t allocation_count; // keeps track of total allocation count

static size_t freed_count; // keeps track of total released blocks

static void *allocation_table[1024]; // stores pointers to void memory

static void track_allocation(void *ptr, size_t size) {
    if (ptr == NULL || size == 0) {
        return;
    }

    if (allocation_count > 1024) {
        return;
    }

    if (allocated_bytes > SIZE_MAX - size) {
        return;
    }

    allocated_bytes += size;
    allocation_count++;

    allocation_table[allocation_count] = ptr;
}

static void untrack_allocation(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    for (size_t i = 0; i < allocation_count; i++) {
        if (ptr == allocation_table[i]) {
            allocation_table[i] = NULL;
            allocation_count--;
            return;
        }
    }
    return;
}
