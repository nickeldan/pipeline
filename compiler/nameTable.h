#ifndef PIPELINE_COMPILER_NAME_TABLE_H
#define PIPELINE_COMPILER_NAME_TABLE_H

#include "definitions.h"

typedef struct plNameTable plNameTable;

plNameTable *
plNameTableNew(void);

void
plNameTableFree(plNameTable *table);

const char *
plRegisterName(plNameTable *table, const char *name, unsigned int length, void *ctx);

void
plUnregisterName(plNameTable *table, const char *name);

bool
plLookupName(const plNameTable *table, const char *name, void **ctx);

bool
plUpdateNameContext(const plNameTable *table, const char *name, void *new_ctx);

#endif  // PIPELINE_COMPILER_NAME_TABLE_H
