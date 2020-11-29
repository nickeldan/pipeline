#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "plUtil.h"

void
errorQuit(const char* functionName, const char* format, ...) {
    va_list args;

    fprintf(stderr, "%s: ", functionName);

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");

    exit(1);
}

void*
safeMalloc(const char* functionName, size_t size) {
    void* ptr;

    ptr = malloc(size);
    if (!ptr && size > 0) {
        errorQuit(functionName, "Failed to allocate %zu bytes", size);
    }

    return ptr;
}

void*
safeCalloc(const char* functionName, size_t nmemb, size_t size) {
    void* ptr;

    ptr = calloc(nmemb, size);
    if (!ptr && nmemb * size > 0) {
        errorQuit(functionName, "Failed to allocate %zu bytes", nmemb * size);
    }

    return ptr;
}