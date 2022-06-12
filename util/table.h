#ifndef PIPELINE_TABLE_H
#define PIPELINE_TABLE_H

#include "util.h"

typedef struct plWordTable plWordTable;

typedef struct plRefTable plRefTable;

typedef struct plLineTable plLineTable;

typedef struct plRefTableIterator {
    const plRefTable *table;
    const void *opaque;
    size_t idx;
} plRefTableIterator;

plWordTable *
plWordTableNew(void);

plRefTable *
plRefTableNew(void);

plLineTable *
plLineTableNew(void);

void
plWordTableFree(plWordTable *table);

void
plRefTableFree(plRefTable *table);

void
plLineTableFree(plLineTable *table);

const char *
plRegisterWord(plWordTable *table, const char *word, unsigned int length);

void
plUnregisterWord(plWordTable *table, const char *word);

bool
plLookupRef(const plRefTable *table, const char *word, unsigned int length, void **ctx);

bool
plUpdateRef(plRefTable *table, const char *word, void *new_ctx);

void
plRefTableIteratorInit(plRefTableIterator *iterator, const plRefTable *table);

const char *
plRefTableIterate(plRefTableIterator *iterator, void **ctx);

int
plRegisterLine(plLineTable *table, unsigned int line_no, const char *line, unsigned int length);

const char *
plLookupLine(const plLineTable *table, unsigned int line_no);

#endif  // PIPELINE_TABLE_H
