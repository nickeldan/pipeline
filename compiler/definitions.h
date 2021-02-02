#ifndef PIPELINE_COMPILER_DEFINITIONS_H
#define PIPELINE_COMPILER_DEFINITIONS_H

#include "vasq/logger.h"

#include "plUtil.h"

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof((A)[0]))

#define ERROR_STRING   "\e[0;31mError: \e[0m"    // red
#define WARNING_STRING "\e[0;34mWarning: \e[0m"  // blue

#endif  // PIPELINE_COMPILER_DEFINITIONS_H
