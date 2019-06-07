#ifndef __PIPELINE_MODULE_H__
#define __PIPELINE_MODULE_H__

#include <stdbool.h>

#include "frames.h"
#include "../pipeline/object.h"

typedef struct _plModule_t plModule_t;

bool set_module_path(plModule_t *module, const char *path);
char *module_path(const plModule_t *module);
plNameReference_t *resolve_exported_name(const char *name, const plModule_t *module);
plModule_t *get_exported_module(const char *name, const plModule_t *module);
void clear_module(plModule_t *module);

#endif // __PIPELINE_MODULE_H__