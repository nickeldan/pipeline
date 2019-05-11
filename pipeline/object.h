#ifndef __PIPELINE_PIPELINE_H__
#define __PIPELINE_PIPELINE_H__

#include <stdint.h>

#include "../util.h"

typedef uint8_t plObjectType_t;

#define PL_TYPE_NULL 0x01
#define PL_TYPE_TRUE 0x02
#define PL_TYPE_FALSE 0x04
#define PL_TYPE_INT 0x08
#define PL_TYPE_FLOAT 0x10
#define PL_TYPE_BYTE_ARRAY 0x20
#define PL_TYPE_ARRAY 0x40
#define PL_TYPE_STRUCT 0x80

#define PL_PRED_BOOL (PL_TYPE_TRUE|PL_TYPE_FALSE)
#define PL_PRED_NUM (PL_TYPE_INT|PL_TYPE_FLOAT)
#define PL_PRED_ARRAY (PL_TYPE_BYTE_ARRAY|PL_TYPE_ARRAY|PL_TYPE_STRUCT)

#define PL_OBJECT_HEADER \
	plObjectType_t type;

typedef struct {
	PL_OBJECT_HEADER
} plObject;

typedef int64_t plInt_t;
typedef double plFloat_t;

typedef struct {
	PL_OBJECT_HEADER
	union {
		plInt_t integer;
		plFloat_t decimal;
	} value;
} plObjectNumber;

typedef struct {
	PL_OBJECT_HEADER
	void *opaque;
	uint32_t size;
} plObjectArrayBase;

typedef struct {
	PL_OBJECT_HEADER
	uint8_t *bytes;
	uint32_t size, length;
} plObjectByteArray;

#define PL_OBJECT_ARRAY_HEADER \
	plObject **values; \
	uint32_t size;

typedef struct {
	PL_OBJECT_HEADER
	PL_OBJECT_ARRAY_HEADER
	uint32_t length;
} plObjectArray;

typedef uint16_t plModuleId_t;
typedef uint16_t plStructId_t;

typedef struct {
	PL_OBJECT_HEADER
	PL_OBJECT_ARRAY_HEADER
	plModuleId_t moduleId;
	plStructId_t structId;
} plObjectStruct;

void plFreeObject(plObject *object);

#endif // __PIPELINE_PIPELINE_H__