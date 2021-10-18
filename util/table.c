#include <stdlib.h>
#include <string.h>

#include "table.h"

#define NUM_RECORDS 97

typedef struct plAbstractRecord {
    struct plAbstractRecord *next;
    char *string;
    unsigned int length;
} plAbstractRecord;

typedef struct plWordRecord {
    plAbstractRecord abstract;
    unsigned int num_references;
} plWordRecord;

typedef struct plRefRecord {
    plAbstractRecord abstract;
    void *ctx;
} plRefRecord;

typedef struct plAbstractTable {
    plAbstractRecord *records[NUM_RECORDS];
} plAbstractTable;

struct plWordTable {
    plWordRecord *records[NUM_RECORDS];
};

struct plRefTable {
    plRefRecord *records[NUM_RECORDS];
};

static unsigned int
wordHash(const char *word, unsigned int length)
{
    unsigned int hash = 0;

    for (int k = 0; k < 5; k++) {
        for (unsigned int j = 0; j < length; j++) {
            unsigned char c = word[j];

            hash *= 9;
            hash ^= c;
        }
    }

    return hash % NUM_RECORDS;
}

static plAbstractRecord *
findRecord(const plAbstractTable *table, const char *word, unsigned int length, unsigned int *hash,
           plAbstractRecord **prev)
{
    unsigned int h;
    plAbstractRecord *p = NULL;

    if (length == 0) {
        VASQ_ERROR(debug_logger, "Length cannot be 0");
        return NULL;
    }

    h = wordHash(word, length);
    if (hash) {
        *hash = h;
    }

    for (plAbstractRecord *traverse = table->records[h]; traverse; traverse = traverse->next) {
        if (traverse->length == length && memcmp(traverse->string, word, length) == 0) {
            if (prev) {
                *prev = p;
            }
            return traverse;
        }

        p = traverse;
    }

    return NULL;
}

static void
abstractTableFree(plAbstractTable *table)
{
    if (!table) {
        return;
    }

    for (unsigned int k = 0; k < NUM_RECORDS; k++) {
        plAbstractRecord *record;

        record = table->records[k];
        table->records[k] = NULL;

        while (record) {
            plAbstractRecord *temp;

            temp = record;
            record = record->next;
            free(temp->string);
            free(temp);
        }
    }

    free(table);
}

static void
advanceIterator(plRefTableIterator *iterator)
{
    while (iterator->idx < NUM_RECORDS && !iterator->table->records[iterator->idx]) {
        iterator->idx++;
    }

    if (iterator->idx < NUM_RECORDS) {
        iterator->opaque = iterator->table->records[iterator->idx];
    }
}

plWordTable *
plWordTableNew(void)
{
    plWordTable *table;

    table = VASQ_MALLOC(debug_logger, sizeof(*table));
    if (table) {
        *table = (plWordTable){0};
    }
    return table;
}

plRefTable *
plRefTableNew(void)
{
    plRefTable *table;

    table = VASQ_MALLOC(debug_logger, sizeof(*table));
    if (table) {
        *table = (plRefTable){0};
    }
    return table;
}

void
plWordTableFree(plWordTable *table)
{
    abstractTableFree((plAbstractTable *)table);
}

void
plRefTableFree(plRefTable *table)
{
    abstractTableFree((plAbstractTable *)table);
}

const char *
plRegisterWord(plWordTable *table, const char *word, unsigned int length)
{
    unsigned int hash = 0;
    plWordRecord *record;

    if (!table || !word) {
        VASQ_ERROR(debug_logger, "table and word cannot be NULL");
        return NULL;
    }

    record = (plWordRecord *)findRecord((plAbstractTable *)table, word, length, &hash, NULL);
    if (record) {
        record->num_references++;
    }
    else {
        record = VASQ_MALLOC(debug_logger, sizeof(*record));
        if (!record) {
            return NULL;
        }

        record->abstract.string = VASQ_MALLOC(debug_logger, length + 1);
        if (!record->abstract.string) {
            free(record);
            return NULL;
        }
        memcpy(record->abstract.string, word, length);
        record->abstract.string[length] = '\0';
        record->abstract.length = length + 1;
        record->num_references = 1;

        record->abstract.next = (plAbstractRecord *)table->records[hash];
        table->records[hash] = record;
    }

    return record->abstract.string;
}

void
plUnregisterWord(plWordTable *table, const char *word)
{
    unsigned int hash = 0;
    plWordRecord *record, *prev;

    if (!table) {
        VASQ_ERROR(debug_logger, "table cannot be NULL");
        return;
    }

    if (!word) {
        return;
    }

    record = (plWordRecord *)findRecord((plAbstractTable *)table, word, strlen(word), &hash,
                                        (plAbstractRecord **)&prev);
    if (record) {
        if (--record->num_references == 0) {
            if (prev) {
                prev->abstract.next = record->abstract.next;
            }
            else {
                table->records[hash] = (plWordRecord *)record->abstract.next;
            }

            free(record->abstract.string);
            free(record);
        }
    }
    else {
        VASQ_WARNING(debug_logger, "\"%s\" not found in table.", word);
    }
}

bool
plLookupRef(const plRefTable *table, const char *word, unsigned int length, void **ctx)
{
    plRefRecord *record;

    if (!table || !word) {
        VASQ_ERROR(debug_logger, "table and word cannot be NULL");
        return false;
    }

    record = (plRefRecord *)findRecord((const plAbstractTable *)table, word, length, NULL, NULL);
    if (!record) {
        return false;
    }

    if (ctx) {
        *ctx = record->ctx;
    }
    return true;
}

bool
plUpdateRef(plRefTable *table, const char *word, void *new_ctx)
{
    size_t len;
    unsigned int hash = 0;
    plRefRecord *record;

    if (!table || !word) {
        VASQ_ERROR(debug_logger, "table and word cannot be NULL");
        return false;
    }

    len = strlen(word);
    record = (plRefRecord *)findRecord((plAbstractTable *)table, word, len, &hash, NULL);
    if (!record) {
        record = VASQ_MALLOC(debug_logger, sizeof(*record));
        if (!record) {
            return false;
        }
        *record = (plRefRecord){0};

        record->abstract.string = strdup(word);
        if (!record->abstract.string) {
            free(record);
            return false;
        }
        record->abstract.length = len;

        record->abstract.next = (plAbstractRecord *)table->records[hash];
        table->records[hash] = record;
    }

    record->ctx = new_ctx;

    return true;
}

void
plRefTableIteratorInit(plRefTableIterator *iterator, const plRefTable *table)
{
    if (!iterator || !table) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return;
    }

    iterator->table = table;
    iterator->idx = 0;

    advanceIterator(iterator);
}

const char *
plRefTableIterate(plRefTableIterator *iterator, void **ctx)
{
    const char *ret;
    const plRefRecord *record;

    if (!iterator) {
        VASQ_ERROR(debug_logger, "iterator cannot be NULL.");
        return NULL;
    }
    if (!iterator->table) {
        VASQ_ERROR(debug_logger, "iterator->table cannot be NULL.");
        return NULL;
    }

    if (iterator->idx == NUM_RECORDS) {
        return NULL;
    }

    record = (const plRefRecord *)iterator->opaque;
    ret = record->abstract.string;
    if (ctx) {
        *ctx = record->ctx;
    }

    if (record->abstract.next) {
        iterator->opaque = record->abstract.next;
    }
    else {
        iterator->idx++;
        advanceIterator(iterator);
    }

    return ret;
}
