#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "vasq/logger.h"

#include "object.h"

#define CAPACITY_EXPANSION(capacity) ((capacity)*5 / 4)

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
        plArray *array = (plArray *)object;

        if (array->objects) {
            uint32_t length;

            length = array->length;
            for (unsigned int k = 0; k < length; k++) {
                plFreeObject(array->objects[k]);
            }
        }
    }

    if (flags & PL_OBJ_PRED_GEN_ARRAY && !(flags & PL_OBJ_FLAG_STATIC_BYTES)) {
        plGenArrayPtr ptr = (plGenArrayPtr)object;

        free(ptr->opaque);
    }

    if (!(flags & PL_OBJ_FLAG_STATIC)) {
        free(object);
    }
}

plObject *
plCopyObject(const plObject *object)
{
    plObject *new;

    if (!object) {
        VASQ_ERROR(debug_logger, "object cannot be NULL");
        return NULL;
    }

    new = VASQ_CALLOC(debug_logger, 1, objectSize(object->flags));
    if (!new) {
        return NULL;
    }
    new->flags = object->flags;
    new->flags &= ~(PL_OBJ_FLAG_STATIC | PL_OBJ_FLAG_STATIC_BYTES | PL_OBJ_FLAG_ORPHAN);

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
                trueArray->objects[k] = plCopyObject(((plArray *)object)->objects[k]);
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

    integer = VASQ_CALLOC(debug_logger, 1, sizeof(*integer));
    if (integer) {
        integer->flags = PL_OBJ_TYPE_INT;
    }
    return (plObject *)integer;
}

plObject *
plNewFloat(void)
{
    plFloat *decimal;

    decimal = VASQ_CALLOC(debug_logger, 1, sizeof(*decimal));
    if (decimal) {
        decimal->flags = PL_OBJ_TYPE_FLOAT;
    }
    return (plObject *)decimal;
}

plObject *
plNewArray(void)
{
    plArray *array;

    array = VASQ_CALLOC(debug_logger, 1, sizeof(*array));
    if (array) {
        array->flags = PL_OBJ_TYPE_ARRAY;
        array->objects = NULL;
    }
    return (plObject *)array;
}

plObject *
plNewByteArray(void)
{
    plByteArray *array;

    array = VASQ_CALLOC(debug_logger, 1, sizeof(*array));
    if (array) {
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

    if (!object) {  // plPopulateIntegerFromString will check string and length.
        VASQ_ERROR(debug_logger, "object cannot be NULL");
        return PL_RET_USAGE;
    }

    integer = (plInteger *)plNewInteger();
    if (!integer) {
        return PL_RET_OUT_OF_MEMORY;
    }

    ret = plPopulateIntegerFromString(string, length, integer);
    if (ret == PL_RET_OK) {
        *object = (plObject *)integer;
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
    char array[PL_INTEGER_MAX_LENGTH + 1];
    char *temp;

    if (!string || !integer) {
        VASQ_ERROR(debug_logger, "string and integer cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0 || string[0] == '\0') {
        VASQ_ERROR(debug_logger, "length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if (length > PL_INTEGER_MAX_LENGTH) {
        VASQ_ERROR(debug_logger, "String is too long");
        return PL_RET_BAD_DATA;
    }

    memcpy(array, string, length);
    array[length] = '\0';

    errno = 0;
    value = strtoll(array, &temp, 10);
    if (*temp != '\0' || errno != 0) {
        return PL_RET_BAD_DATA;
    }

#if __LONG_LONG_WIDTH__ > PL_INTEGER_BIT_SIZE
    if (value < PL_MIN_INTEGER || value > PL_MAX_INTEGER) {
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

    if (!string || !object) {
        VASQ_ERROR(debug_logger, "string and object cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0) {
        VASQ_ERROR(debug_logger, "length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    decimal = (plFloat *)plNewFloat();
    if (!decimal) {
        return PL_RET_OUT_OF_MEMORY;
    }

    decimal_place = string;
    for (unsigned int k = 0; k < length; k++) {
        if (string[k] == '.') {
            break;
        }
        decimal_place++;
    }

    if (decimal_place > string) {
        plInteger integer;

        ret = plPopulateIntegerFromString(string, decimal_place - string, &integer);
        if (ret != PL_RET_OK) {
            goto error;
        }
        memcpy(&decimal->ipart, &integer.value, sizeof(integer.value));
    }
    else {
        decimal->ipart = 0;
    }

    if (decimal_place < string + length) {
        if (decimal_place[1] == '\0') {
            decimal->fpart = 0;
        }
        else if (!isdigit(decimal_place[1])) {
            VASQ_ERROR(debug_logger, "Invalid decimal part");
            ret = PL_RET_BAD_DATA;
            goto error;
        }
        else {
            char array[20];
            char *temp;
            unsigned int length2;

            length2 = MIN(length - (unsigned long)(decimal_place - string), sizeof(array) - 1);
            memcpy(array, decimal_place, length2);
            array[length2] = '\0';

            temp = array;
            decimal->fpart = strtod(array, &temp);
            if (temp == array) {
                VASQ_ERROR(debug_logger, "Invalid decimal part");
                ret = PL_RET_BAD_DATA;
                goto error;
            }
        }
    }
    else {
        decimal->fpart = 0;
    }

    *object = (plObject *)decimal;
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

    if (!string || !object) {
        VASQ_ERROR(debug_logger, "string and object cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0) {
        VASQ_ERROR(debug_logger, "Length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if (length > PL_INTEGER_BIT_SIZE / 4) {
        VASQ_ERROR(debug_logger, "String is too long");
        return PL_RET_BAD_DATA;
    }

    if (string[0] == '-') {
        VASQ_ERROR(debug_logger, "This function cannot process negative values");
        return PL_RET_BAD_DATA;
    }

    array = (plByteArray *)plNewByteArray();
    if (!array) {
        return PL_RET_OUT_OF_MEMORY;
    }

    array_length = length / 2;
    if (length % 2) {
        array_length++;
    }
    array->length = array->capacity = array_length;
    array->bytes = VASQ_CALLOC(debug_logger, array->capacity, 1);
    if (!array->bytes) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }

    for (unsigned int k = 0; k < length; k++) {
        char c;
        unsigned char value;

        c = string[length - k - 1];
        if (!isxdigit(c)) {
            goto error;
        }

        if (isdigit(c)) {
            value = c - '0';
        }
        else if (isupper(c)) {
            value = c - 'A' + 10;
        }
        else {
            value = c - 'a' + 10;
        }

        array->bytes[array_length - k / 2 - 1] |= (value << 4 * (k % 2));
    }

    return PL_RET_OK;

error:

    plFreeObject((plObject *)array);
    return ret;
}

int
plConcatenateByteArrays(plObject *first, const plObject *second)
{
    uint32_t new_length;
    plByteArray *array1 = (plByteArray *)first;
    const plByteArray *array2 = (const plByteArray *)second;

    new_length = array1->length + array2->length;
    if (new_length < array1->length) {
        VASQ_ERROR(debug_logger, "Integer overflow detected.");
        return PL_RET_OVERFLOW;
    }

    if (new_length > array1->capacity) {
        uint32_t new_capacity;
        uint8_t *success;

        new_capacity = CAPACITY_EXPANSION(new_length);
        if (new_capacity < new_length) {
            VASQ_ERROR(debug_logger, "Integer overflow detected.");
            return PL_RET_OVERFLOW;
        }

        if (array1->flags & PL_OBJ_FLAG_STATIC_BYTES) {
            success = VASQ_MALLOC(debug_logger, new_capacity);
        }
        else {
            success = VASQ_REALLOC(debug_logger, array1->bytes, new_capacity);
        }

        if (!success) {
            return PL_RET_OUT_OF_MEMORY;
        }

        array1->bytes = success;
        array1->capacity = new_capacity;
        array1->flags &= ~PL_OBJ_FLAG_STATIC_BYTES;
    }

    memcpy(array1->bytes + array1->length, array2->bytes, array2->length);
    array1->length = new_length;

    return PL_RET_OK;
}
