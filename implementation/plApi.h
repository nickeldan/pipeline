#ifndef PIPELINE_API_H
#define PIPELINE_API_H

#include "plObject.h"
#include "plEnvironment.h"

typedef int (*plSourceSetupFunc)(const plEnvironment *env, void **ctx, ...);

typedef int (*plSourceFunc)(const plEnvironment *env, void *ctx, plObject **produced);

typedef int (*plPipeFunc)(const plEnvironment *env, const plObject *arg, const plObject *attached, plObject **produced);

typedef int (*plSinkFunc)(const plEnvironment *env, const plObject *arg, const plObject *attached, plObject **store);

#endif // PIPELINE_API_H
