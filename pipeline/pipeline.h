#ifndef __PIPELINE_PIPELINE_H__
#define __PIPELINE_PIPELINE_H__

#include <stdint.h>

typedef unsigned char bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef uint8_t plObj_type;

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

#define PL_STATIC_NULL 0

typedef int64_t plInt_t;
typedef double plFloat_t;

struct _plObj_value;

typedef struct _plArray_t {
	struct _plObj_value *values;
	size_t size, space;
} plArray_t;

typedef union {
	plInt_t integer;
	plFloat_t decimal;
	plStatic_t staticValue;
	plArray_t array;
} plObj_value;

typedef struct {
	plObj_value value;
	plObj_type type;
} plObject;

#endif // __PIPELINE_PIPELINE_H__