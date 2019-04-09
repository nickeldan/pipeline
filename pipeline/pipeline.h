#ifndef __PIPELINE_PIPELINE_H__
#define __PIPELINE_PIPELINE_H__

#include <stdint.h>
#include <stddef.h>

#ifndef MAX
#define MAX(a,b) ( ( (a) > (b) )? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a,b) ( ( (a) < (b) )? (a) : (b) )
#endif

typedef uint8_t bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef uint8_t plObj_type_t;

#define PL_TYPE_NULL 0x01
#define PL_TYPE_SENTINEL 0x02
#define PL_TYPE_BOOL 0x04
#define PL_TYPE_INT 0x08
#define PL_TYPE_FLOAT 0x10
#define PL_TYPE_GEN_ARRAY 0x20
#define PL_TYPE_BYTE_ARRAY 0x40
#define PL_TYPE_STRUCT 0x80

typedef uint32_t plStruct_id_t;

#define PL_PRED_VOID (PL_TYPE_NULL|PL_TYPE_SENTINEL)
#define PL_PRED_NUM (PL_TYPE_INT|PL_TYPE_FLOAT)
#define PL_PRED_ARRAY (PL_TYPE_GEN_ARRAY|PL_TYPE_BYTE_ARRAY|PL_TYPE_STRUCT)

typedef uint8_t plBool_t;

#define PL_BOOL_TRUE TRUE
#define PL_BOOL_FALSE FALSE

typedef int64_t plInt_t;
typedef double plFloat_t;

union _plObj_value_t;

typedef struct {
	union _plObj_value_t *values;
	uint32_t length, space;
} plArray_t;

typedef union _plObj_value_t {
	plInt_t integer;
	plFloat_t decimal;
	plBool_t boolValue;
	plArray_t array;
} plObj_value_t;

typedef struct {
	plObj_value_t value;
	plStruct_id_t structId;
	plObj_type_t type;
} plObject;

#endif // __PIPELINE_PIPELINE_H__