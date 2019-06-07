#include <stdlib.h>
#include <string.h>

#include "object.h"

void free_object(plObject_t *object) {
	if ( object->type&PL_PRED_ARRAY ) {
		plObjectArrayPtr array=(plObjectArrayPtr)object;

		if ( object->type != PL_TYPE_BYTE_ARRAY ) {
			plObject_t **items=array->opaque;
			size_t length;

			if ( object->type == PL_TYPE_ARRAY ) {
				length=((plObjectArray_t*)object)->length;
			}
			else {
				length=array->size;
			}

			for (size_t k=0; k<length; k++) {
				pl_free_object(items[k]);
			}
		}

		if ( array->size > 0 ) {
			free(array->opaque);
		}
	}

	free(object);
}