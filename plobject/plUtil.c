#include <stdlib.h>
#include <unistd.h>

#include "vasq/definitions.h"

#include "plUtil.h"

vasqLogger *debug_logger;

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

int
plSetupDebuggingLogger(vasqLogLevel_t level)
{
#if LL_USE >= 0
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
