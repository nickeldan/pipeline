#pragma once

#include <stdint.h>

#include "plUtil.h"

#define PL_OBJECT_HEADER uint32_t flags;

#define PL_OBJ_TYPE_NULL       0x00000000
#define PL_OBJ_TYPE_INT        0x00000001
#define PL_OBJ_TYPE_FLOAT      0x00000002
#define PL_OBJ_PRED_NUM        (PL_OBJ_TYPE_INT | PL_OBJ_TYPE_FLOAT)
#define PL_OBJ_TYPE_BOOL       0x00000004
#define PL_OBJ_PRED_GEN_ARRAY  0x00000008
#define PL_OBJ_PRED_TRUE_ARRAY 0x00000010
#define PL_OBJ_TYPE_ARRAY      (PL_OBJ_PRED_GEN_ARRAY | PL_OBJ_PRED_TRUE_ARRAY)
#define PL_OBJ_PRED_STRUCT     0x00000020
#define PL_OBJ_TYPE_STRUCT     (PL_OBJ_TYPE_ARRAY | PL_OBJ_PRED_STRUCT)
#define PL_OBJ_PRED_BYTE_ARRAY 0x00000040
#define PL_OBJ_TYPE_BYTE_ARRAY (PL_OBJ_PRED_GEN_ARRAY | PL_OBJ_PRED_BYTE_ARRAY)
#define PL_OBJ_TYPE_BLANK      0x00000080

#define PL_OBJ_FLAG_TRUE         0x00000100
#define PL_OBJ_FLAG_ORPHAN       0x00000200
#define PL_OBJ_FLAG_STATIC       0x00000400
#define PL_OBJ_FLAG_STATIC_BYTES 0x00000800

typedef struct plObject {
    PL_OBJECT_HEADER
} plObject;

#define OBJ_TYPE(ptr) (((plObject *)ptr)->flags & 0x000000ff)

typedef struct plInteger {
    PL_OBJECT_HEADER
    int64_t value;
} plInteger;

#define PL_INTEGER_BIT_SIZE   64
#define PL_INTEGER_MAX_LENGTH 20  // The length of -1 * (1 << 63).
#define PL_MAX_INTEGER        9223372036854775807L
#define PL_MIN_INTEGER        -9223372036854775808L

typedef struct plFloat {
    plInteger integer_part;
    double decimal_part;
} plFloat;

#define PL_ARRAY_HEADER \
    PL_OBJECT_HEADER    \
    uint32_t length;    \
    plObject **objects; \
    uint32_t capacity;

typedef struct plArray {
    PL_ARRAY_HEADER
} plArray;

typedef uintptr_t plModuleId_t;
typedef uint16_t plStructId_t;

typedef struct plStruct {
    PL_ARRAY_HEADER
    plModuleId_t module_id;
    plStructId_t struct_id;
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
} * plGenArrayPtr;

void
plFreeObject(plObject *object);

plObject *
plCopyObject(const plObject *object);

plObject *
plNewInteger(void);

plObject *
plNewFloat(void);

plObject *
plNewArray(void);

plObject *
plNewByteArray(void);

int
plIntegerFromString(const char *string, unsigned int length, plObject **object);

int
plPopulateIntegerFromString(const char *string, unsigned int length, plInteger *integer);

int
plFloatFromString(const char *string, unsigned int length, plObject **object);

int
plIntegerFromHexString(const char *string, unsigned int length, plObject **object);

extern plObject true_object;
extern plObject false_object;
extern plObject blank_object;
extern plObject null_object;
