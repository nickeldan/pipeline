#ifndef PIPELINE_COMPILER_TABLE_H
#define PIPELINE_COMPILER_TABLE_H

#include "definitions.h"

typedef struct plWordTable plWordTable;

typedef struct plRefTable plRefTable;

typedef struct plRefTableIterator {
    const plRefTable *table;
    const void *opaque;
    size_t idx;
} plRefTableIterator;

plWordTable *
plWordTableNew(void);

plRefTable *
plRefTableNew(void);

void
plWordTableFree(plWordTable *table);

void
plRefTableFree(plRefTable *table);

const char *
plRegisterWord(plWordTable *table, const char *word, unsigned int length);

void
plUnregisterWord(plWordTable *table, const char *word);

bool
plLookupRef(const plRefTable *table, const char *word, void **ctx);

bool
plUpdateRef(plRefTable *table, const char *word, void *new_ctx);

void
plRefTableIteratorInit(plRefTableIterator *iterator, const plRefTable *table);

const char *
plRefTableIterate(plRefTableIterator *iterator, void **ctx);

#endif  // PIPELINE_COMPILER_TABLE_H
