#pragma once

#include "plObject.h"

typedef int (*plSourceSetupFunc)(void **ctx, ...);

typedef int (*plSourceFunc)(void *ctx, plObject **produced);

typedef int (*plPipeFunc)(const plObject *arg, const plObject *attached, plObject **produced);

typedef int (*plSinkFunc)(const plObject *arg, const plObject *attached, plObject **store);
