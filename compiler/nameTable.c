#include <stdlib.h>
#include <string.h>

#include "plUtil.h"

#define REF_TABLE_SIZE 9997

typedef struct nameRecord {
    char *string;
    unsigned int numReferences;
} nameRecord;

static void
clearTable(void);
static unsigned int
symbolHash(const char *name);

// Non-external variables
static nameRecord refTable[REF_TABLE_SIZE];

void
nameTableSetup(void)
{
    atexit(clearTable);
}

char *
registerName(const char *name)
{
    unsigned int hash, k;

    k = hash = symbolHash(name);

    do {
        if (!refTable[k].string) {
            refTable[k].string = strdup(name);
            if (!refTable[k].string) {
                ERROR_QUIT("strdup failed");
            }
            refTable[k].numReferences = 1;
        }
        else if (strcmp(refTable[k].string, name) == 0) {
            refTable[k].numReferences++;
        }
        else {
            continue;
        }

        return refTable[k].string;
    } while ((++k) % REF_TABLE_SIZE != hash);

    ERROR_QUIT("Symbol table overflow");
}

void
removeReference(const char *name)
{
    unsigned int hash, k;

    k = hash = symbolHash(name);

    do {
        if (!refTable[k].string) {
            ERROR_QUIT("String was not registered: %s", name);
        }

        if (strcmp(refTable[k].string, name) == 0) {
            if (refTable[k].numReferences == 0) {
                ERROR_QUIT("String has no references remaining to it: %s", name);
            }

            refTable[k].numReferences--;
            return;
        }
    } while ((++k) % REF_TABLE_SIZE != hash);

    ERROR_QUIT("String was not registered: %s", name);
}

static void
clearTable(void)
{
    for (int k = 0; k < REF_TABLE_SIZE; k++) {
        free(refTable[k].string);
    }
}

static unsigned int
symbolHash(const char *name)
{
    unsigned int hash = 0, c;

    while ((c = *name++)) {
        hash *= 9;
        hash ^= c;
    }

    return hash % REF_TABLE_SIZE;
}
