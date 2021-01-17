#include <stdlib.h>
#include <string.h>

#include "nameTable.h"

#define REF_TABLE_SIZE 9997

typedef struct nameRecord {
    struct nameRecord *next;
    char *string;
    void *ctx;
    unsigned int length;  // Includes the '\0'.
    unsigned int num_references;
} nameRecord;

struct plNameTable {
    nameRecord *records[REF_TABLE_SIZE];
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

    return hash % REF_TABLE_SIZE;
}

static nameRecord *
findRecord(const plNameTable *table, const char *name, unsigned int length, unsigned int *hash,
           nameRecord **prev)
{
    unsigned int h;
    nameRecord *p = NULL;

    if (length == 0) {
        VASQ_ERROR("Length cannot be 0");
        return NULL;
    }

    h = nameHash(name, length);
    if (hash) {
        *hash = h;
    }

    for (nameRecord *traverse = table->records[h]; traverse; traverse = traverse->next) {
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
    for (unsigned int k = 0; k < REF_TABLE_SIZE; k++) {
        nameRecord *record;

        record = table->records[k];
        table->records[k] = NULL;
        while (record) {
            nameRecord *temp;

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
    nameRecord *record;

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
    nameRecord *record, *prev;

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
    nameRecord *record;

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
    nameRecord *record;

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
