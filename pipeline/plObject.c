#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vasq/logger.h"

#include "plObject.h"

// External variables
plObject true_object = {.flags = PL_OBJ_TYPE_BOOL | PL_OBJ_FLAG_STATIC | PL_OBJ_FLAG_TRUE};
plObject false_object = {.flags = PL_OBJ_TYPE_BOOL | PL_OBJ_FLAG_STATIC};
plObject blank_object = {.flags = PL_OBJ_TYPE_BLANK | PL_OBJ_FLAG_STATIC};
plObject null_object = {.flags = PL_OBJ_FLAG_STATIC};

static unsigned int
objectSize(uint32_t flags)
{
    switch (flags & 0x000000ff) {
    case PL_OBJ_TYPE_INT: return sizeof(plInteger);

    case PL_OBJ_TYPE_FLOAT: return sizeof(plFloat);

    case PL_OBJ_TYPE_ARRAY:
    case PL_OBJ_TYPE_BYTE_ARRAY: return sizeof(plArray);

    case PL_OBJ_TYPE_STRUCT: return sizeof(plStruct);

    default: return sizeof(plObject);
    }
}

void
plFreeObject(plObject *object)
{
    uint32_t flags;

    if (!object) {
        return;
    }

    flags = object->flags;

    if (flags & PL_OBJ_PRED_TRUE_ARRAY) {
        plArray *array;

        array = (plArray *)object;
        if (array->objects) {
            uint32_t length;

            length = array->length;
            for (unsigned int k = 0; k < length; k++) {
                plFreeObject(array->objects[k]);
            }
        }
    }

    if ( flags&PL_OBJ_PRED_GEN_ARRAY && !(flags&PL_OBJ_FLAG_STATIC_BYTES) ) {
        plGenArrayPtr ptr;

        ptr = (plGenArrayPtr)object;
        free(ptr->opaque);
    }

    if (!(flags & PL_OBJ_FLAG_STATIC)) {
        free(object);
    }
}

plObject *
copyObject(const plObject *object)
{
    plObject *new;

    new = VASQ_CALLOC(1, objectSize(object->flags));
    if (!new) {
        return NULL;
    }
    new->flags = object->flags;
    new->flags &= ~(PL_OBJ_FLAG_STATIC | PL_OBJ_FLAG_STATIC_BYTES);

    if ((object->flags) & PL_OBJ_PRED_GEN_ARRAY) {
        plGenArrayPtr array = (plGenArrayPtr) new;

        array->length = ((plGenArrayPtr)object)->length;
        array->capacity = ((plGenArrayPtr)object)->capacity;

        if ((object->flags) & PL_OBJ_PRED_BYTE_ARRAY) {
            plByteArray *bytes = (plByteArray *)new;

            bytes->bytes = malloc(bytes->capacity);
            if (!bytes->bytes) {
                goto error;
            }
            memcpy(bytes->bytes, ((plByteArray *)object)->bytes, bytes->length);
        }
        else {
            plArray *trueArray = (plArray *)new;
            uint32_t length = trueArray->length;

            if ((object->flags) & PL_OBJ_PRED_STRUCT) {
                plStruct *st = (plStruct *)new;

                st->module_id = ((plStruct *)object)->module_id;
                st->struct_id = ((plStruct *)object)->struct_id;
            }

            trueArray->objects = calloc(trueArray->length, sizeof(plObject *));
            if (!trueArray->objects) {
                goto error;
            }
            for (uint32_t k = 0; k < length; k++) {
                trueArray->objects[k] = copyObject(((plArray *)object)->objects[k]);
                if (!trueArray->objects[k]) {
                    goto error;
                }
            }
        }
    }
    else {
        memcpy(new, object, objectSize(object->flags));
    }

    return new;

error:

    plFreeObject(new);

    return NULL;
}

plObject *
plNewInteger(void)
{
    plInteger *integer;

    integer = VASQ_CALLOC(1, sizeof(*integer));
    if ( integer ) {
        integer->flags = PL_OBJ_TYPE_INT;
    }
    return (plObject *)integer;
}

plObject *
plNewFloat(void)
{
    plFloat *decimal;

    decimal = VASQ_CALLOC(1, sizeof(*decimal));
    if ( decimal ) {
        decimal->integer_part.flags = PL_OBJ_TYPE_FLOAT;
    }
    return (plObject *)decimal;
}

plObject *
plNewArray(void)
{
    plArray *array;

    array = VASQ_CALLOC(1, sizeof(*array));
    if ( array ) {
        array->flags = PL_OBJ_TYPE_ARRAY;
        array->objects = NULL;
    }
    return (plObject *)array;
}

plObject *
plNewByteArray(void)
{
    plByteArray *array;

    array = VASQ_CALLOC(1, sizeof(*array));
    if ( array ) {
        array->flags = PL_OBJ_TYPE_BYTE_ARRAY;
        array->bytes = NULL;
    }
    return (plObject *)array;
}

int
plIntegerFromString(const char *string, unsigned int length, plObject **object)
{
    int ret;
    plInteger *integer;

    if ( !object ) { // plPopulateIntegerFromString will check string and length.
        VASQ_ERROR("object cannot be NULL");
        return PL_RET_BAD_ARGS;
    }

    integer = (plInteger *)plNewInteger();
    if ( !integer ) {
        return PL_RET_OUT_OF_MEMORY;
    }

    ret = plPopulateIntegerFromString(string, length, integer);
    if ( ret == PL_RET_OK ) {
        *object = (plObject*)integer;
    }
    else {
        free(integer);
    }

    return ret;
}

int
plPopulateIntegerFromString(const char *string, unsigned int length, plInteger *integer)
{
    long long value;
    char array[PL_INTEGER_MAX_LENGTH+1];
    char *temp;

    if ( !string || !integer ) {
        VASQ_ERROR("string and integer cannot be NULL");
        return PL_RET_BAD_ARGS;
    }

    if ( length == 0 || string[0] == '\0' ) {
        VASQ_ERROR("length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if ( length > PL_INTEGER_MAX_LENGTH ) {
        VASQ_ERROR("String is too long");
        return PL_RET_BAD_DATA;
    }

    memcpy(array, string, length);
    array[length] = '\0';

    errno = 0;
    value = strtoll(array, &temp, 10);
    if ( *temp != '\0' || errno != 0 ) {
        return PL_RET_BAD_DATA;
    }

#if __LONG_LONG_WIDTH__ > PL_INTEGER_BYTE_SIZE*8
    if ( value < PL_MIN_INTEGER || value > PL_MAX_INTEGER ) {
        return PL_RET_BAD_DATA;
    }
#endif

    integer->value = value;

    return PL_RET_OK;
}

int
plFloatFromString(const char *string, unsigned int length, plObject **object)
{
    int ret;
    plFloat *decimal;
    const char *decimal_place;

    if ( !string || !object ) {
        VASQ_ERROR("string and object cannot be NULL");
        return PL_RET_BAD_ARGS;
    }

    if ( length == 0 ) {
        VASQ_ERROR("length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    decimal = (plFloat *)plNewFloat();
    if ( !decimal ) {
        return PL_RET_OUT_OF_MEMORY;
    }

    decimal_place = string;
    for (unsigned int k=0; k<length; k++) {
        if ( string[k] == '.' ) {
            break;
        }
        decimal_place++;
    }

    if ( decimal_place > string ) {
        ret = plPopulateIntegerFromString(string, decimal_place-string, &decimal->integer_part);
        if ( ret != PL_RET_OK ) {
            goto error;
        }
    }
    else {
        decimal->integer_part.value = 0;
    }

    if ( decimal_place < string + length ) {
        if ( decimal_place[1] == '\0' ) {
            decimal->decimal_part = 0;
        }
        else if ( !isdigit(decimal_place[1]) ) {
            VASQ_ERROR("Invalid decimal part");
            ret = PL_RET_BAD_DATA;
            goto error;
        }
        else {
            char array[20];
            char *temp;
            unsigned int length2;

            length2 = MIN(length - (unsigned long)(decimal_place-string), sizeof(array)-1);
            memcpy(array, decimal_place, length2);
            array[length2] = '\0';

            temp = array;
            decimal->decimal_part = strtod(array, &temp);
            if ( temp == array ) {
                VASQ_ERROR("Invalid decimal part");
                ret = PL_RET_BAD_DATA;
                goto error;
            }
        }
    }
    else {
        decimal->decimal_part = 0;
    }

    *object = (plObject*)decimal;
    return PL_RET_OK;

error:

    free(decimal);
    return ret;
}

int
plIntegerFromHexString(const char *string, unsigned int length, plObject **object)
{
    int ret = PL_RET_BAD_DATA;
    unsigned int array_length;
    plByteArray *array;

    if ( !string || !object ) {
        VASQ_ERROR("string and object cannot be NULL");
        return PL_RET_BAD_ARGS;
    }

    if ( length == 0 ) {
        VASQ_ERROR("Length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if ( length > PL_INTEGER_BYTE_SIZE*2 ) {
        VASQ_ERROR("String is too long");
        return PL_RET_BAD_DATA;
    }

    if ( string[0] == '-' ) {
        VASQ_ERROR("This function cannot process negative values");
        return PL_RET_BAD_DATA;
    }

    array = (plByteArray *)plNewByteArray();
    if ( !array ) {
        return PL_RET_OUT_OF_MEMORY;
    }

    array_length = length/2;
    if ( length%2 ) {
        array_length++;
    }
    array->length = array->capacity = array_length;
    array->bytes = VASQ_CALLOC(array->capacity, 1);
    if ( !array->bytes ) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }

    for (unsigned int k=0; k<length; k++) {
        char c;
        unsigned char value;

        c = string[length-k-1];
        if ( !isxdigit(c) ) {
            goto error;
        }

        if ( isdigit(c) ) {
            value = c - '0';
        }
        else if ( isupper(c) ) {
            value = c - 'A' + 10;
        }
        else {
            value = c - 'a' + 10;
        }

        array->bytes[array_length-k/2-1] |= ( value << 4*(k%2) );
    }

    return PL_RET_OK;

error:

    plFreeObject((plObject *)array);
    return ret;
}
