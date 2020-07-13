#ifndef __PIPELINE_PLOBJECT_H__
#define __PIPELINE_PLOBJECT_H__

#include <stdint.h>

#define PL_OBJECT_HEADER \
    uint32_t flags;

#define PL_OBJ_TYPE_NULL 0x00000000
#define PL_OBJ_TYPE_INT 0x00000001
#define PL_OBJ_TYPE_FLOAT 0x00000002
#define PL_OBJ_PRED_NUM (PL_OBJ_TYPE_INT|PL_OBJ_TYPE_FLOAT)
#define PL_OBJ_TYPE_BOOL 0x00000004
#define PL_OBJ_PRED_GEN_ARRAY 0x00000008
#define PL_OBJ_PRED_TRUE_ARRAY 0x00000010
#define PL_OBJ_TYPE_ARRAY (PL_OBJ_PRED_GEN_ARRAY|PL_OBJ_PRED_TRUE_ARRAY)
#define PL_OBJ_PRED_STRUCT 0x00000020
#define PL_OBJ_TYPE_STRUCT (PL_OBJ_TYPE_ARRAY|PL_OBJ_PRED_STRUCT)
#define PL_OBJ_PRED_BYTE_ARRAY 0x00000040
#define PL_OBJ_TYPE_BYTE_ARRAY (PL_OBJ_PRED_GEN_ARRAY|PL_OBJ_PRED_BYTE_ARRAY)
#define PL_OBJ_TYPE_BLANK 0x00000080

#define PL_OBJ_FLAG_TRUTHY 0x00000100
#define PL_OBJ_FLAG_ORPHAN 0x00000200
#define PL_OBJ_FLAG_STATIC 0x00000400

#define PL_OBJ_FLAG_NEGATIVE 0x00010000
#define PL_OBJ_FLAG_STATIC_BYTES 0x00020000

typedef struct plObject {
    PL_OBJECT_HEADER
} plObject;

#define OBJ_TYPE(ptr) (((plObject*)ptr)->flags&0x000000ff)
#define TRUTHY(ptr) (((plObject*)ptr)->flags&PL_OBJ_FLAG_TRUTHY)

#define PL_INTEGER_MAX_DIGITS 2467

typedef struct plInteger {
    PL_OBJECT_HEADER
    uint32_t firstBlock;
    uint32_t *blocks;
    uint8_t numExtraBlocks;
} plInteger;

typedef struct plFloat {
    plInteger intPart;
    double decimal;
} plFloat;

#define PL_ARRAY_HEADER \
    PL_OBJECT_HEADER \
    uint32_t length; \
    plObject **objects; \
    uint32_t capacity;

typedef struct plArray {
    PL_ARRAY_HEADER
} plArray;

typedef uint32_t plStructId_t;

typedef struct plStruct {
    PL_ARRAY_HEADER
    plStructId_t structId;
} plStruct;

typedef struct plByteArray {
    PL_OBJECT_HEADER
    uint32_t length;
    uint8_t *bytes;
    uint32_t capacity;
} plByteArray;

typedef struct plGenArray {
    PL_OBJECT_HEADER
    uint32_t length;
    void *opaque;
    uint32_t capacity;
} *plGenArrayPtr;

void freeObject(plObject *object);
plObject *copyObject(const plObject *object);

plInteger *plIntegerNew(void);
void plIntegerClear(plInteger *integer);
int plIntegerFromString(plInteger **integer, const char *string, size_t len);
int plIntegerSmartAddInPlace(plInteger **original, const plInteger *summand);
int plIntegerAddInPlace(plInteger *original, const plInteger *summand);
int plIntegerQuickAddInPlace(plInteger *integer, uint32_t value, uint8_t blockShift);
int plIntegerLeftShiftInPlace(plInteger *integer, unsigned int shift);

extern plObject trueObject;
extern plObject falseObject;
extern plObject blankObject;
extern plObject nullObject;

#endif // __PIPELINE_OBJECT_H__
