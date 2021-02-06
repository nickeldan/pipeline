#ifndef PIPELINE_COMPILER_MODULE_H
#define PIPELINE_COMPILER_MODULE_H

#include "definitions.h"
#include "table.h"

typedef struct plModule {
    plRefTable *export_table;
} plModule;

int
plModuleLoad(const char *module_name, plModule **module);

void
plModuleFree(plModule *module);

#endif  // PIPELINE_COMPILER_MODULE_H
