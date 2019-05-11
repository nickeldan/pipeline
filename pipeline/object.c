#include <stdlib.h>
#include <string.h>

#include "object.h"

void plFreeObject(plObject *object) {
	if ( object->type&PL_PRED_ARRAY ) {
		plObjectArrayBase *array=(plObjectArrayBase*)object;

		if ( object->type != PL_TYPE_BYTE_ARRAY ) {
			plObject **items=array->opaque;
			size_t length;

			if ( object->type == PL_TYPE_ARRAY ) {
				length=((plObjectArray*)object)->length;
			}
			else {
				length=array->size;
			}

			for (size_t k=0; k<length; k++) {
				plFreeObject(items[k]);
			}
		}

		if ( array->size > 0 ) {
			free(array->opaque);
		}
	}

	free(object);
}