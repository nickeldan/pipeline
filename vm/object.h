#ifndef PIPELINE_OBJECT_H
#define PIPELINE_OBJECT_H

#include <stdint.h>

// util header files
#include "util.h"

#include "struct.h"

typedef struct plObjectHandle plObjectHandle;

typedef int64_t plInteger_t;
typedef double plFloat_t;
typedef unsigned char plBool_t;

#define PL_INTEGER_BIT_SIZE   64
#define PL_INTEGER_MAX_LENGTH 20  // The length of -1 * (1 << 63).
#define PL_MAX_INTEGER        9223372036854775807L
#define PL_MIN_INTEGER        -9223372036854775808L

typedef struct plArray {
    plObjectHandle *handles;
    uint32_t length;
    uint32_t capacity;
} plArray;

typedef struct plByteArray {
    uint8_t *bytes;
    uint32_t length;
    uint32_t capacity;
} plByteArray;

typedef struct plStructInstance {
    plObjectHandle *handles;
    plStruct *type;
} plStructInstance;

typedef union plObjectValue {
    plInteger_t integer;
    plFloat_t decimal;
    plBool_t boolean;
    plArray *array;
    plByteArray *bytes;
    plStructInstance *instance;
} plObjectValue;

struct plObjectHandle {
    plObjectValue as;
    uint32_t flags;
};

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

#define PL_OBJ_FLAG_OWNED        0x00000100
#define PL_OBJ_FLAG_SUBH_OWNED   0x00000200
#define PL_OBJ_FLAG_DYNAMIC      0x00000400
#define PL_OBJ_FLAG_DYNAMIC_DATA 0x00000800

#define OBJ_TYPE(handle) ((handle)->flags & 0x000000ff)

void
plFreeObject(plObjectHandle *handle);

plObjectHandle
plBoolLiteral(bool value);

plObjectHandle
plNullLiteral(void);

plObjectHandle
plBlankLiteral(void);

plByteArray *
plNewByteArray(void);

int
plPopulateIntegerFromString(const char *string, unsigned int length, plInteger_t *integer);

int
plPopulateIntegerFromHexString(const char *string, unsigned int length, plInteger_t *integer);

int
plPopulateFloatFromString(const char *string, unsigned int length, plFloat_t *decimal);

int
plConcatenateByteArrays(plByteArray *first, const plByteArray *second);

#endif  // PIPELINE_OBJECT_H
