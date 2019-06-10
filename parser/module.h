#ifndef __PIPELINE_MODULE_H__
#define __PIPELINE_MODULE_H__

#include <stdbool.h>

#include "reference.h"
#include "../pipeline/object.h"

typedef struct _plModule_t plModule_t;

bool generate_module_id(plModule_t *module);
bool set_module_path(plModule_t *module, const char *path);
char *module_path(const plModule_t *module);
plError_t *resolve_exported_name(const char *name, const plModule_t *module, plNameReference_t *reference);
plError_t *resolve_imported_module(const char *name, const plModule_t *module, plModule_t **importedModule);
void clear_module(plModule_t *module);

#endif // __PIPELINE_MODULE_H__