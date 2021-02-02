#include <stdlib.h>
#include <string.h>

#include "nameTable.h"

#define NUM_RECORDS 97

typedef struct plNameRecord {
    struct plNameRecord *next;
    char *string;
    void *ctx;
    unsigned int length;  // Includes the '\0'.
    unsigned int num_references;
} plNameRecord;

struct plNameTable {
    plNameRecord *records[NUM_RECORDS];
};

static unsigned int
nameHash(const char *name, unsigned int length)
{
    unsigned int hash = 0;

    for (int k = 0; k < 5; k++) {
        for (unsigned int j = 0; j < length; j++) {
            unsigned char c = name[j];

            hash *= 9;
            hash ^= c;
        }
    }

    return hash % NUM_RECORDS;
}

static plNameRecord *
findRecord(const plNameTable *table, const char *name, unsigned int length, unsigned int *hash,
           plNameRecord **prev)
{
    unsigned int h;
    plNameRecord *p = NULL;

    if (length == 0) {
        VASQ_ERROR("Length cannot be 0");
        return NULL;
    }

    h = nameHash(name, length);
    if (hash) {
        *hash = h;
    }

    for (plNameRecord *traverse = table->records[h]; traverse; traverse = traverse->next) {
        if (traverse->length == length + 1 && memcmp(traverse->string, name, length) == 0) {
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
advanceIterator(plNameTableIterator *iterator)
{
    while (iterator->idx < NUM_RECORDS && !iterator->table->records[iterator->idx]) {
        iterator->idx++;
    }

    if (iterator->idx < NUM_RECORDS) {
        iterator->opaque = iterator->table->records[k];
    }
}

plNameTable *
plNameTableNew(void)
{
    plNameTable *table;

    table = VASQ_MALLOC(sizeof(*table));
    if (table) {
        *table = (plNameTable){0};
    }
    return table;
}

void
plNameTableFree(plNameTable *table)
{
    if (!table) {
        return;
    }
    for (unsigned int k = 0; k < NUM_RECORDS; k++) {
        plNameRecord *record;

        record = table->records[k];
        table->records[k] = NULL;
        while (record) {
            plNameRecord *temp;

            temp = record->next;
            free(record->string);
            free(record);
            record = temp;
        }
    }

    free(table);
}

const char *
plRegisterName(plNameTable *table, const char *name, unsigned int length, void *ctx)
{
    unsigned int hash = 0;
    plNameRecord *record;

    if (!table || !name) {
        VASQ_ERROR("table and name cannot be NULL");
        return NULL;
    }

    record = findRecord(table, name, length, &hash, NULL);
    if (record) {
        if (record->num_references + 1 == 0) {
            VASQ_ERROR("Integer overflow detected when trying to store '%.*s' in table <0x%p>.", length,
                       name, table);
            return NULL;
        }
        record->num_references++;
        VASQ_DEBUG("'%.*s' has %u references in table <0x%p>.", length, name, record->num_references, table);
    }
    else {
        record = VASQ_MALLOC(sizeof(*record));
        if (!record) {
            return NULL;
        }

        record->string = VASQ_MALLOC(length + 1);
        if (!record->string) {
            free(record);
            return NULL;
        }
        memcpy(record->string, name, length);
        record->string[length] = '\0';
        record->length = length + 1;
        record->num_references = 1;
        record->ctx = ctx;

        record->next = table->records[hash];
        table->records[hash] = record;
        VASQ_DEBUG("'%.*s' has 1 reference in table <0x%p>.", length, name, table);
    }

    return record->string;
}

void
plUnregisterName(plNameTable *table, const char *name)
{
    unsigned int hash;
    plNameRecord *record, *prev;

    if (!table) {
        VASQ_ERROR("table cannot be NULL");
        return;
    }

    if (!name) {
        return;
    }

    record = findRecord(table, name, strlen(name), &hash, &prev);
    if (record) {
        if (--record->num_references == 0) {
            VASQ_DEBUG("Removing '%s' from table <0x%p>.", name, table);

            if (prev) {
                prev->next = record->next;
            }
            else {
                table->records[hash] = record->next;
            }

            free(record->string);
            free(record);
        }
        else {
            VASQ_DEBUG("'%s' has %u reference%s in table <0x%p>.", name, record->num_references,
                       (record->num_references == 1) ? "" : "s", table);
        }
    }
    else {
        VASQ_WARNING("'%s' not found in table <0x%p>.", name, table);
    }
}

bool
plLookupName(const plNameTable *table, const char *name, void **ctx)
{
    plNameRecord *record;

    if (!table || !name) {
        VASQ_ERROR("table and name cannot be NULL");
        return false;
    }

    record = findRecord(table, name, strlen(name), NULL, NULL);
    if (!record) {
        return false;
    }

    if (ctx) {
        *ctx = record->ctx;
    }
    return true;
}

bool
plUpdateNameContext(const plNameTable *table, const char *name, void *new_ctx)
{
    plNameRecord *record;

    if (!table || !name) {
        VASQ_ERROR("table and name cannot be NULL");
        return false;
    }

    record = findRecord(table, name, strlen(name), NULL, NULL);
    if (!record) {
        return false;
    }

    record->ctx = new_ctx;
    return true;
}

void
plNameTableIteratorInit(plNameTableIterator *iterator, const plnameTable *table) if (!iterator || !itable)
{
    VASQ_ERROR("The arguments cannot be NULL.");
    return;
}

iterator->table = table;
iterator->idx = 0;

advanceIterator(iterator);
}

const char *
plNameTableIterator(plNameTableIterator *iterator, void **ctx)
{
    const char *ret;
    const plNameRecord *record;

    if (!iterator) {
        VASQ_ERROR("iterator cannot be NULL.");
        return NULL;
    }
    if (!iterator->table) {
        VASQ_ERROR("iterator->table cannot be NULL.");
        return NULL;
    }

    if (iterator->idx == NUM_RECORDS) {
        return NULL;
    }

    record = (const plNameRecord *)iterator->opaque;
    ret = record->string;
    if (ctx) {
        *ctx = record->ctx;
    }

    if (record->next) {
        iterator->opaque = record->next;
    }
    else {
        iterator->idx++;
        advanceIterator(iterator);
    }

    return ret;
}
