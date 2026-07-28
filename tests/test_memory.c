#include "../include/memory.h"


int test_memory(void) {
    void *memory = shell_malloc(100);

    if (memory == NULL) {
        return -1;
    }

    size_t allocated_bytes = memory_allocated_bytes();

    size_t allocated_count = memory_allocated_count();

    size_t free_count = memory_freed_count();

    printf("allocated bytes %zu\n", allocated_bytes);
    printf("allocated count %zu\n", allocated_count);
    printf("freed count %zu\n", free_count);
    
    void *memory_sec = shell_malloc(100);

    if (memory_sec == NULL) {
        return -1;
    }
    
    allocated_bytes = memory_allocated_bytes();

    allocated_count = memory_allocated_count();

    free_count = memory_freed_count();

    printf("allocated bytes %zu\n", allocated_bytes);
    printf("allocated count %zu\n", allocated_count);
    printf("freed count %zu\n", free_count);
    
    shell_free(memory);
    
    allocated_bytes = memory_allocated_bytes();

    allocated_count = memory_allocated_count();

    free_count = memory_freed_count();

    printf("allocated bytes %zu\n", allocated_bytes);
    printf("allocated count %zu\n", allocated_count);
    printf("freed count %zu\n", free_count);

    return 0;
}
