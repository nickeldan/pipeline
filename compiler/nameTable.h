#pragma once

#include <stdbool.h>

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
