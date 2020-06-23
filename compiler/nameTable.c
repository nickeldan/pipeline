#include <stdlib.h>
#include <string.h>

#include "nameTable.h"
#include "plUtil.h"

#define REF_TABLE_SIZE 9997

static void clearTable(void);
static unsigned int symbolHash(const char *name);

// Non-external variables
static char *refTable[REF_TABLE_SIZE];

void nameTableSetup(void) {
	atexit(clearTable);
}

char *registerName(const char *name) {
	unsigned int hash, k;

	k=hash=symbolHash(name);

	do {
		if ( !refTable[k] ) {
			refTable[k]=strdup(name);
			if ( !refTable[k] ) {
				ERROR_QUIT("strdup failed");
			}
		}
		else if ( strcmp(refTable[k],name) != 0 ) {
			continue;
		}

		return refTable[k];
	} while ( (++k)%REF_TABLE_SIZE != hash );

	ERROR_QUIT("Symbol table overflow");
}

static void clearTable(void) {
	for (int k=0; k<REF_TABLE_SIZE; k++) {
		free(refTable[k]);
	}
}

static unsigned int symbolHash(const char *name) {
	unsigned int hash=0, c;

	while ( c=*name++ ) {
		hash*=9;
		hash^=c;
	}

	return hash%REF_TABLE_SIZE;
}