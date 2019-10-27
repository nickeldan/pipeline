#ifndef __PIPELINE_OBJECT_H__
#define __PIPELINE_OBJECT_H__

#include <stdint.h>

typedef uint8_t plObjectType_t;

#define PL_TYPE_FLAG_DYNAMIC 0x80
#define PL_TYPE_FLAG_ARRAY 0x40

#define PL_PRED_ANY 0xff
#define PL_TYPE_NULL 0x00

#define PL_PRED_BOOL 0x20
#define PL_TYPE_FALSE PL_PRED_BOOL
#define PL_TYPE_TRUE (PL_PRED_BOOL|0x10)

#define PL_PRED_NUM (PL_TYPE_FLAG_DYNAMIC|0x20)
#define PL_TYPE_INT PL_PRED_NUM
#define PL_TYPE_FLOAT (PL_PRED_NUM|0x10)

#define PL_PRED_ARRAY (PL_TYPE_FLAG_DYNAMIC|PL_TYPE_FLAG_ARRAY)
#define PL_TYPE_BYTE_ARRAY PL_PRED_ARRAY
#define PL_PRED_OBJECT_ARRAY (PL_PRED_ARRAY|0x20)
#define PL_TYPE_ARRAY PL_PRED_OBJECT_ARRAY
#define PL_TYPE_STRUCT (PL_PRED_OBJECT_ARRAY|0x10)

#define PL_OBJECT_HEADER \
	plObjectType_t type;

typedef struct {
	PL_OBJECT_HEADER
} plObject_t;

typedef int64_t plInt_t;
typedef double plFloat_t;

typedef struct {
	PL_OBJECT_HEADER
	union {
		plInt_t integer;
		plFloat_t decimal;
	} value;
} plObjectNumber_t;

typedef uint32_t plArraySize_t;
#define PL_MAX_ARRAY_SIZE 0xffffffff

typedef struct {
	PL_OBJECT_HEADER
	uint8_t *bytes;
	plArraySize_t size, length;
} plObjectByteArray_t;

#define PL_OBJECT_ARRAY_HEADER \
	plObject_t **values; \
	plArraySize_t size;

typedef struct {
	PL_OBJECT_HEADER
	PL_OBJECT_ARRAY_HEADER
	plArraySize_t length;
} plObjectArray_t;

typedef uint64_t plModuleId_t;
#define PL_BUILTIN_MODULE 0
typedef uint16_t plStructId_t;
#define PL_NOT_STRUCT 0

typedef struct {
	PL_OBJECT_HEADER
	PL_OBJECT_ARRAY_HEADER
	plStructId_t structId;
	plModuleId_t moduleId;
} plObjectStruct_t;

typedef struct {
	PL_OBJECT_HEADER
	void *opaque;
	plArraySize_t size;
} *plObjectArrayPtr;

void free_object(plObject_t *object);

#endif // __PIPELINE_OBJECT_H__