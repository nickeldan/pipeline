#ifndef PIPELINE_UTIL_H
#define PIPELINE_UTIL_H

#include <stdint.h>

#include "vasq/logger.h"
#include "vasq/placeholder.h"

#define HIDDEN_SYMBOL __attribute__((visibility("hidden")))

#define ERROR_STRING   "\e[0;31mError: \e[0m"    // red
#define WARNING_STRING "\e[0;34mWarning: \e[0m"  // blue

enum plRetValue {
    PL_RET_OK = 0,
    PL_RET_USAGE,
    PL_RET_OUT_OF_MEMORY,
    PL_RET_BAD_DATA,
    PL_RET_NO_ACCESS,
    PL_RET_IO,
    PL_RET_OVERFLOW,
};

int
plTranslateVasqRet(int value);

#if LL_USE >= 0
#define PL_LOGGER_PREAMBLE "[%L]%_ %F:%f:%l: "
#else
#define PL_LOGGER_PREAMBLE ""
#endif

int
plSetupDebuggingLogger(vasqLogLevel_t level);

extern vasqLogger *debug_logger;

#endif  // PIPELINE_UTIL_H
