#pragma once

#include "vasq/logger.h"

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof((A)[0]))
