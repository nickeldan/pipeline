#ifndef __PIPELINE_PIPELINE_H__
#define __PIPELINE_PIPELINE_H__

#include <stdint.h>

typedef int64_t plInt_t;
typedef double plFloat_t;

typedef enum {
	PL_STATIC_NULL = -1,
	PL_STATIC_FALSE,
	PL_STATIC_TRUE,
} plStatic_t;

typedef union {
	plInt_t integer;
	plFloat_t decimal;
	plStatic_t staticValue;
} plObj_value;

typedef enum {
	PL_TYPE_NULL = 0,
	PL_TYPE_BOOL,
	PL_TYPE_INT,
	PL_TYPE_FLOAT,
} plObj_type;

typedef struct {
	plObj_value value;
	plObj_type type;
} plObject;

#endif // __PIPELINE_PIPELINE_H__