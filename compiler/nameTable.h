#ifndef PIPELINE_COMPILER_NAME_TABLE_H
#define PIPELINE_COMPILER_NAME_TABLE_H

#include "definitions.h"

typedef struct plNameTable plNameTable;

typedef struct plNameTableIterator {
    const plNameTable *table;
    const void *opaque;
    size_t idx;
} plNameTableIterator;

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

void
plNameTableIteratorInit(plNameTableIterator *iterator, const plnameTable *table);

const char *
plNameTableIterate(plNameTableIterator *iterator, void **ctx);

#endif  // PIPELINE_COMPILER_NAME_TABLE_H
