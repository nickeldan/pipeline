#ifndef PIPELINE_COMPILER_REFERENCE_H
#define PIPELINE_COMPILER_REFERENCE_H

#include "definitions.h"
#include "token.h"

typedef struct plReference {
    void *data;
    plLexicalLocation location;
    uint32_t flags;
} plReference;

#define PL_REF_FLAG_OBJECT 0x80000000
#define PL_REF_FLAG_TYPE   0x40000000
#define PL_REF_FLAG_SOURCE 0x20000000
#define PL_REF_FLAG_PIPE   0x10000000
#define PL_REF_FLAG_SINK   0x08000000
#define PL_REF_FLAG_MODULE 0x04000000
#define PL_REF_FLAG_EXPORT 0x02000000

#define PL_REF_FLAG_CONST     0x00800000
#define PL_REF_FLAG_NULLABLE  0x00400000
#define PL_REF_FLAG_LOCALABLE 0x00200000
#define PL_REF_FLAG_NULL      0x00100000
#define PL_REF_FLAG_BLANK     0x00080000
#define PL_REF_FLAG_POS       0x00040000
#define PL_REF_FLAG_NEG       0x00020000
#define PL_REF_FLAG_ZERO      0x00010000
#define PL_REF_FLAG_TRUE      0x00008000
#define PL_REF_FLAG_FALSE     0x00004000

const char *
plRefTypeName(uint32_t flags);

#endif  // PIPELINE_COMPILER_REFERENCE_H