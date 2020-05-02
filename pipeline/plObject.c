#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include "plObject.h"
#include "plUtil.h"

// External variables
plObject trueObject={.flags=PL_OBJ_TYPE_BOOL|PL_OBJ_FLAG_TRUTHY|PL_OBJ_FLAG_STATIC};
plObject falseObject={.flags=PL_OBJ_TYPE_BOOL|PL_OBJ_FLAG_STATIC};
plObject blankObject={.flags=PL_OBJ_TYPE_BLANK|PL_OBJ_FLAG_STATIC};
plObject nullObject={.flags=PL_OBJ_FLAG_STATIC};

static size_t objectSize(uint32_t flags) __attribute__((pure));
static void plIntegerSafeLeftShiftInPlace(plInteger *integer, unsigned int shift);

void freeObject(plObject *object) {
	uint32_t flags;

	if ( !object ) {
		return;
	}

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
			for (unsigned int k=0; k<length; k++) {
				freeObject(array->objects[k]);
			}

			free(array->objects);
		}
	}
	else if ( flags&PL_OBJ_PRED_BYTE_ARRAY ) {
		if ( !(flags&PL_OBJ_FLAG_STATIC_BYTES) ) {
			free(((plByteArray*)object)->bytes);
		}
	}

	if ( !(flags&PL_OBJ_FLAG_STATIC) ) {
		free(object);
	}
	else if ( flags&(PL_OBJ_PRED_NUM|PL_OBJ_PRED_GEN_ARRAY) ) {
		BZERO((void*)object+sizeof(plObject),objectSize(flags)-sizeof(plObject));
	}
}

plObject *copyObject(const plObject *object) {
	plObject *new;

	new=calloc(1,objectSize(object->flags));
	if ( !new ) {
		return NULL;
	}
	new->flags=object->flags;
	new->flags&=~(PL_OBJ_FLAG_STATIC|PL_OBJ_FLAG_STATIC_BYTES);

	if ( (object->flags)&PL_OBJ_PRED_NUM ) {
		plInteger *integer=(plInteger*)new;

		integer->firstBlock=((plInteger*)object)->firstBlock;
		integer->numExtraBlocks=((plInteger*)object)->numExtraBlocks;

		integer->blocks=malloc(sizeof(uint32_t)*integer->numExtraBlocks);
		if ( !integer->blocks ) {
			goto error;
		}
		memcpy(integer->blocks,((plInteger*)object)->blocks,sizeof(uint32_t)*integer->numExtraBlocks);

		if ( (object->flags)&PL_OBJ_TYPE_FLOAT ) {
			((plFloat*)integer)->decimal=((plFloat*)object)->decimal;
		}
	}
	else if ( (object->flags)&PL_OBJ_PRED_GEN_ARRAY ) {
		plGenArrayPtr array=(plGenArrayPtr)new;

		array->length=((plGenArrayPtr)object)->length;
		array->capacity=((plGenArrayPtr)object)->capacity;

		if ( (object->flags)&PL_OBJ_PRED_BYTE_ARRAY ) {
			plByteArray *bytes=(plByteArray*)new;

			bytes->bytes=malloc(bytes->capacity);
			if ( !bytes->bytes ) {
				goto error;
			}
			memcpy(bytes->bytes,((plByteArray*)object)->bytes,bytes->length);
		}
		else {
			plArray *trueArray=(plArray*)new;
			uint32_t length=trueArray->length;

			if ( (object->flags)&PL_OBJ_PRED_STRUCT ) {
				plStruct *st=(plStruct*)new;

				st->moduleId=((plStruct*)object)->moduleId;
				st->structId=((plStruct*)object)->structId;
			}

			trueArray->objects=calloc(trueArray->length,sizeof(plObject*));
			if ( !trueArray->objects ) {
				goto error;
			}
			for (uint32_t k=0; k<length; k++) {
				trueArray->objects[k]=copyObject(((plArray*)object)->objects[k]);
				if ( !trueArray->objects[k] ) {
					goto error;
				}
			}
		}
	}

	return new;

	error:

	freeObject(new);

	return NULL;
}

plInteger *plIntegerNew(void) {
	plInteger *integer;

	integer=calloc(1,sizeof(plInteger));
	if ( integer ) {
		integer->flags=PL_OBJ_TYPE_INT|PL_OBJ_FLAG_TRUTHY;
	}

	return integer;
}

void plIntegerClear(plInteger *integer) {
	free(integer->blocks);

	BZERO(integer,sizeof(plInteger));
	integer->flags=PL_OBJ_TYPE_INT|PL_OBJ_FLAG_TRUTHY;
}

int plIntegerFromString(plInteger **integer, const char *string, size_t len) {
	int ret;
	plInteger *new;

	if ( len > PL_INTEGER_MAX_DIGITS || ( string[0] == '-' && len >= PL_INTEGER_MAX_DIGITS ) ) {
		return PL_ERROR_BAD_INPUT;
	}

	new=plIntegerNew();
	if ( !new ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}

	for (size_t k=1; k<=len; k++) {
		int digit;
		plInteger temp;

		if ( !isdigit(string[len-k]) ) {
			if ( k == len && string[0] == '-' ) {
				new->flags|=PL_OBJ_FLAG_NEGATIVE;
				break;
			}

			ret=PL_ERROR_BAD_INPUT;
			goto error;
		}

		digit=string[len-k]-'0';
		if ( digit == 0 ) {
			continue;
		}

		temp.blocks=NULL;
		plIntegerClear(&temp);
		temp.firstBlock=digit;

		for (size_t j=0; j<k-1; j++) {
			plInteger temp2;

			memcpy(&temp2,&temp,sizeof(plInteger));
			if ( temp2.numExtraBlocks > 0 ) {
				temp2.blocks=malloc(sizeof(uint32_t)*temp2.numExtraBlocks);
				if ( !temp2.blocks ) {
					ret=PL_ERROR_OUT_OF_MEMORY;
					goto loop_error;
				}

				memcpy(temp2.blocks,temp.blocks,sizeof(uint32_t)*temp2.numExtraBlocks);
			}

			ret=plIntegerLeftShiftInPlace(&temp,2);
			if ( ret != PL_ERROR_OK ) {
				free(temp2.blocks);
				goto loop_error;
			}

			ret=plIntegerAddInPlace(&temp,&temp2);
			free(temp2.blocks);
			if ( ret != PL_ERROR_OK ) {
				goto loop_error;
			}
		}

		ret=plIntegerLeftShiftInPlace(&temp,k-1);
		if ( ret != PL_ERROR_OK ) {
			goto loop_error;
		}

		ret=plIntegerAddInPlace(new,&temp);
		free(temp.blocks);
		if ( ret != PL_ERROR_OK ) {
			goto error;
		}

		continue;

		loop_error:

		free(temp.blocks);
		goto error;
	}

	*integer=new;
	return PL_ERROR_OK;

	error:

	freeObject((plObject*)new);
	return ret;
}

int plIntegerSmartAddInPlace(plInteger **original, const plInteger *summand) {
	if ( summand->numExtraBlocks > (*original)->numExtraBlocks ) {
		int ret;
		plInteger *new;

		new=(plInteger*)copyObject((plObject*)summand);
		if ( !new ) {
			return PL_ERROR_OUT_OF_MEMORY;
		}

		ret=plIntegerAddInPlace(new,*original);
		if ( ret == PL_ERROR_OK ) {
			freeObject((plObject*)(*original));
			*original=new;
		}
		else {
			freeObject((plObject*)new);
		}
		return ret;
	}
	else {
		return plIntegerAddInPlace(*original,summand);
	}
}

int plIntegerAddInPlace(plInteger *original, const plInteger *summand) {
	int ret;

	ret=plIntegerQuickAddInPlace(original,summand->firstBlock,0);
	for (unsigned int k=0; ret==PL_ERROR_OK && k<summand->numExtraBlocks; k++) {
		ret=plIntegerQuickAddInPlace(original,summand->blocks[k],k+1);
	}

	return ret;
}

int plIntegerQuickAddInPlace(plInteger *integer, uint32_t value, uint8_t blockShift) {
	uint64_t carry=value;

	if ( blockShift == 0 ) {
		carry+=integer->firstBlock;
		integer->firstBlock=carry&0xffffffff;
		carry=( carry >> 32 );
		blockShift=1;
	}

	if ( blockShift > integer->numExtraBlocks ) {
		uint32_t *success;

		success=realloc(integer->blocks,sizeof(uint32_t)*blockShift);
		if ( !success ) {
			return PL_ERROR_OUT_OF_MEMORY;
		}
		integer->blocks=success;
		if ( integer->numExtraBlocks == 0 ) {
			BZERO(integer->blocks,sizeof(uint32_t)*blockShift);
		}
		integer->numExtraBlocks=blockShift;
	}

	blockShift--;
	for (; carry>0; carry=(carry >> 32)) {
		if ( blockShift == integer->numExtraBlocks ) {
			uint32_t *success;

			success=realloc(integer->blocks,sizeof(uint32_t)*(blockShift+1));
			if ( !success ) {
				return PL_ERROR_OUT_OF_MEMORY;
			}
			integer->blocks=success;
			integer->numExtraBlocks++;
		}

		carry+=integer->blocks[blockShift];
		integer->blocks[blockShift]=carry&0xffffffff;
	}

	return PL_ERROR_OK;
}

int plIntegerLeftShiftInPlace(plInteger *integer, unsigned int shift) {
	unsigned int remainingBits;
	uint32_t lastBlock, newBlocks;
	uint32_t *success;

	if ( shift == 0 || ( integer->numExtraBlocks == 0 && integer->firstBlock == 0 ) ) {
		return PL_ERROR_OK;
	}

	remainingBits=0;
	lastBlock=( integer->numExtraBlocks == 0 )? integer->firstBlock : integer->blocks[integer->numExtraBlocks-1];
	for (uint32_t i=0x80000000; i>0; i=(i >> 1)) {
		if ( lastBlock&i ) {
			break;
		}

		remainingBits++;
	}

	remainingBits=MIN(shift,remainingBits);

	if ( remainingBits > 0 ) {
		plIntegerSafeLeftShiftInPlace(integer,remainingBits);

		shift-=remainingBits;
	}

	if ( shift == 0 ) {
		return PL_ERROR_OK;
	}

	newBlocks=(shift+31)/32;

	success=realloc(integer->blocks,sizeof(uint32_t)*(integer->numExtraBlocks+newBlocks));
	if ( !success ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}
	integer->blocks=success;
	BZERO(integer->blocks+integer->numExtraBlocks,sizeof(uint32_t)*newBlocks);

	if ( shift >= 32 ) {
		if ( integer->numExtraBlocks > 0 ) {
			memmove(integer->blocks+shift/32,integer->blocks,sizeof(uint32_t)*(shift/32));
			BZERO(integer->blocks,sizeof(uint32_t)*(shift/32));
		}

		integer->blocks[integer->numExtraBlocks]=integer->firstBlock;
		integer->firstBlock=0;

		shift%=32;
	}

	integer->numExtraBlocks+=newBlocks;
	plIntegerSafeLeftShiftInPlace(integer,shift);

	return PL_ERROR_OK;
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

static void plIntegerSafeLeftShiftInPlace(plInteger *integer, unsigned int shift) {
	if ( integer->numExtraBlocks > 0 ) {
		for (unsigned int k=integer->numExtraBlocks-1; k>0; k--) {
			integer->blocks[k]=( integer->blocks[k] << shift );
			integer->blocks[k]|=( integer->blocks[k-1] >> (32-shift) );
		}

		integer->blocks[0]=( integer->blocks[0] << shift );
		integer->blocks[0]=( integer->firstBlock >> (32-shift) );
	}

	integer->firstBlock=( integer->firstBlock << shift );
}
