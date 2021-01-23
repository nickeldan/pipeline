#pragma once

#include "plObject.h"
#include "plEnvironment.h"

typedef int (*plSourceSetupFunc)(const plEnvironment *env, void **ctx, ...);

typedef int (*plSourceFunc)(const plEnvironment *env, void *ctx, plObject **produced);

typedef int (*plPipeFunc)(const plEnvironment *env, const plObject *arg, const plObject *attached, plObject **produced);

typedef int (*plSinkFunc)(const plEnvironment *env, const plObject *arg, const plObject *attached, plObject **store);
