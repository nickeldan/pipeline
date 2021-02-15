#include <stdlib.h>
#include <unistd.h>

#include "vasq/definitions.h"

#include "plUtil.h"

vasqLogger *debug_logger;

#ifdef PL_FULL_LOGGING

static void
freeDebuggingLogger(void)
{
    vasqLoggerFree(debug_logger);
}

#endif  // PL_FULL_LOGGING

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

int
plSetupDebuggingLogger(vasqLogLevel_t level)
{
#ifdef PL_FULL_LOGGING
    int ret;

    ret = vasqLoggerCreate(STDOUT_FILENO, level, PL_LOGGER_PREAMBLE "%M\n", NULL, NULL, &debug_logger);
    if (ret != VASQ_RET_OK) {
        return plTranslateVasqRet(ret);
    }
    atexit(freeDebuggingLogger);
#else
    (void)level;
#endif

    return PL_RET_OK;
}
