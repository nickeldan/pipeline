#include <stdlib.h>
#include <sys/types.h>

#include "plObject.h"
#include "util.h"

// External variables
plObject trueObject={.flags=PL_OBJ_TYPE_BOOL|PL_OBJ_FLAG_TRUTHY|PL_OBJ_FLAG_STATIC};
plObject falseObject={.flags=PL_OBJ_TYPE_BOOL|PL_OBJ_FLAG_STATIC};
plObject blankObject={.flags=PL_OBJ_TYPE_BLANK|PL_OBJ_FLAG_STATIC};
plObject nullObject={.flags=PL_OBJ_FLAG_STATIC};

static size_t objectSize(uint32_t flags) __attribute__((pure));

void freeObject(plObject *object) {
	uint32_t flags;

	flags=object->flags;

	if ( flags&PL_OBJ_PRED_NUM ) {
		plInteger *integer;

		integer=(plInteger*)object;
		free(integer->blocks);
	}
	else if ( flags&PL_OBJ_PRED_TRUE_ARRAY ) {
		plArray *array;

		array=(plArray*)object;
		if ( array->objects ) {
			uint32_t length;

			length=array->length;
			for (uint32_t k=0; k<length; k++) {
				freeObject(array->objects+k);
			}

			free(array->objects);
		}
	}
	else if ( flags&PL_OBJ_PRED_BYTE_ARRAY ) {
		plByteArray *array;

		array=(plByteArray*)object;
		free(array->bytes);
	}

	if ( !(flags&PL_OBJ_FLAG_STATIC) ) {
		free(object);
	}
	else if ( flags&(PL_OBJ_PRED_NUM|PL_OBJ_PRED_GEN_ARRAY) ) {
		BZERO((void*)object+sizeof(plObject),objectSize(flags)-sizeof(plObject));
	}
}

plInteger *newInteger(void) {
	plInteger *integer;

	integer=calloc(1,sizeof(plInteger));
	if ( integer ) {
		integer->flags=PL_OBJ_TYPE_INT|PL_OBJ_FLAG_TRUTHY;
	}

	return integer;
}

int addValueToInt(plInteger **integer, uint32_t value, unsigned int bitShift) {

}

static size_t objectSize(uint32_t flags) {
	switch ( flags&0x000000ff ) {
		case PL_OBJ_TYPE_INT:
		return sizeof(plInteger);

		case PL_OBJ_TYPE_FLOAT:
		return sizeof(plFloat);

		case PL_OBJ_TYPE_ARRAY:
		case PL_OBJ_TYPE_BYTE_ARRAY:
		return sizeof(plArray);

		case PL_OBJ_TYPE_STRUCT:
		return sizeof(plStruct);

		default:
		return sizeof(plObject);
	}
}