#ifndef PIPELINE_MODULE_H
#define PIPELINE_MODULE_H

#include "table.h"
#include "util.h"

typedef struct plModule {
    plRefTable *export_table;
} plModule;

void
plModuleInit(plModule *module);

int
plModuleLoad(const char *module_name, plModule **module);

void
plModuleFree(plModule *module);

#endif  // PIPELINE_MODULE_H
