#ifndef PIPELINE_COMPILER_REFERENCE_H
#define PIPELINE_COMPILER_REFERENCE_H

// util header files
#include "table.h"
#include "util.h"

// compiler/parser header files
#include "token.h"

typedef struct plRefValue {
    union {
        void *data;
        struct {
            int marker;
            int submarker;
        };
    };
    unsigned int contains_data : 1;
} plRefValue;

typedef struct plReference {
    plRefValue value;
    plLexicalLocation location;
    uint32_t flags;
} plReference;

#define PL_REF_FLAG_OBJECT 0x80000000
#define PL_REF_FLAG_TYPE   0x40000000
#define PL_REF_FLAG_SOURCE 0x20000000
#define PL_REF_FLAG_PIPE   0x10000000
#define PL_REF_FLAG_SINK   0x08000000
#define PL_REF_FLAG_MODULE 0x04000000
#define PL_REF_FLAG_ERROR  0x02000000

#define PL_REF_FLAG_EXPORT 0x00800000
#define PL_REF_FLAG_CONST  0x00400000

const char *
plRefTypeName(uint32_t flags);

plReference *
plNewReference(void);

int
plStoreReference(plRefTable *table, const char *symbol, uint32_t flags, const plRefValue *value,
                 const plLexicalLocation *location);

#endif  // PIPELINE_COMPILER_REFERENCE_H
