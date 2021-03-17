#include <stdlib.h>

void *zmalloc(size_t size) {
    return malloc(size);
}

void zfree(void *ptr) {
    free(ptr);
}

void *zrealloc(void *ptr, size_t size) {
    return realloc(ptr,size);
}