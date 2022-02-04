#ifndef PIPELINE_UTIL_H
#define PIPELINE_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#include "vasq/logger.h"
#include "vasq/placeholder.h"

#define HIDDEN_SYMBOL __attribute__((visibility("hidden")))

#define ERROR_STRING   "\e[0;31mError: \e[0m"    // red
#define WARNING_STRING "\e[0;34mWarning: \e[0m"  // blue

#ifdef DEBUG
#define LIKELY(expr)   expr
#define UNLIKELY(expr) expr
#else
#define LIKELY(expr)   true
#define UNLIKELY(expr) false
#endif

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

void *
plSafeMalloc(size_t size);

void *
plSafeRealloc(void *ptr, size_t size);

#if LL_USE >= 0
#define PL_LOGGER_PREAMBLE "[%L]%_ %F:%f:%l: "
#else
#define PL_LOGGER_PREAMBLE ""
#endif

int
plSetupDebuggingLogger(vasqLogLevel_t level);

extern vasqLogger *debug_logger;

#endif  // PIPELINE_UTIL_H
