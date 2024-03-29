#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "vasq/definitions.h"

#include "util.h"

vasqLogger *debug_logger = NULL;

#if LL_USE >= 0

static void
freeDebuggingLogger(void)
{
    vasqLoggerFree(debug_logger);
}

#endif  // LL_USE >= 0

int
plTranslateVasqRet(int value)
{
    switch (value) {
    case VASQ_RET_OK: return PL_RET_OK;
    case VASQ_RET_USAGE: return PL_RET_USAGE;
    case VASQ_RET_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return -1;
    }
}

void *
plSafeMalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr && size > 0) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        abort();
    }
    return ptr;
}

void *
plSafeRealloc(void *ptr, size_t size)
{
    void *new_ptr;

    new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0) {
        fprintf(stderr, "Failed to reallocate %zu bytes.\n", size);
        abort();
    }
    return new_ptr;
}

int
plSetupDebuggingLogger(vasqLogLevel_t level)
{
#if LL_USE >= 0
    int ret;

    ret = vasqLoggerCreate(STDOUT_FILENO, level, PL_LOGGER_PREAMBLE "%M\n", NULL, &debug_logger);
    if (ret != VASQ_RET_OK) {
        return plTranslateVasqRet(ret);
    }
    atexit(freeDebuggingLogger);
#else
    (void)level;
#endif

    return PL_RET_OK;
}
