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

typedef unsigned char bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef uint8_t plObj_type_t;

#define PL_TYPE_NULL 0x01
#define PL_TYPE_BOOL 0x02
#define PL_TYPE_INT 0x04
#define PL_TYPE_FLOAT 0x08
#define PL_TYPE_ARRAY 0x10

#define PL_PRED_NUM (PL_TYPE_INT|PL_TYPE_FLOAT)

typedef enum {
	PL_STATIC_FALSE = 0,
	PL_STATIC_TRUE,
} plStatic_t;

typedef int64_t plInt_t;
typedef double plFloat_t;

#ifdef ARRAY_IMPLEMENTED
union _plObj_value_t;

typedef struct {
	union _plObj_value *values;
	size_t size, space;
} plArray_t;
#endif // ARRAY_IMPLEMENTED

typedef union _plObj_value_t {
	plInt_t integer;
	plFloat_t decimal;
	plStatic_t staticValue;
#ifdef ARRAY_IMPLEMENTED
	plArray_t array;
#endif
} plObj_value_t;

typedef struct {
	plObj_value_t value;
	plObj_type_t type;
} plObject;

#endif // __PIPELINE_PIPELINE_H__