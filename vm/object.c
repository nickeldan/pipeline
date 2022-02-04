#include <stdlib.h>
#include <string.h>

#include "vasq/logger.h"

#include "object.h"

#define CAPACITY_EXPANSION(capacity) ((capacity)*5 / 4)

const char *
plObjectTypeName(uint32_t flags)
{
    switch (flags & PL_OBJ_MASK) {
    case PL_OBJ_TYPE_NULL: return "Null";
    case PL_OBJ_TYPE_INT: return "Int";
    case PL_OBJ_TYPE_FLOAT: return "Float";
    case PL_OBJ_TYPE_BOOL: return "Bool";
    case PL_OBJ_TYPE_ARRAY: return "Array";
    case PL_OBJ_TYPE_STRUCT: return "Struct";
    case PL_OBJ_TYPE_BYTE_ARRAY: return "Bytes";
    case PL_OBJ_TYPE_BLANK: return "Blank";
    case PL_OBJ_TYPE_OPAQUE: return "Opaque";
    default: __builtin_unreachable();
    }
}

void
plFreeObject(plObjectHandle *handle)
{
    uint32_t flags;

    if (!handle || !(handle->flags & PL_OBJ_FLAG_OWNED)) {
        return;
    }

    flags = handle->flags;

    if (flags & PL_OBJ_FLAG_SUBH_OWNED) {
        plObjectHandle **subhandles_loc;
        uint32_t length;

        if (flags & PL_OBJ_PRED_STRUCT) {
            subhandles_loc = &handle->as.instance->handles;
            length = plLengthOfStruct(handle->as.instance->type);
        }
        else {
            subhandles_loc = &handle->as.array->handles;
            length = handle->as.array->length;
        }

        for (uint32_t k = 0; k < length; k++) {
            plFreeObject(*subhandles_loc + k);
        }

        if (flags & PL_OBJ_FLAG_DYNAMIC_DATA) {
            free(*subhandles_loc);
            *subhandles_loc = NULL;
        }
    }
    else if (flags & PL_OBJ_FLAG_DYNAMIC_DATA) {
        free(handle->as.bytes->bytes);
        handle->as.bytes->bytes = NULL;
    }
    else if (flags & PL_OBJ_TYPE_OPAQUE) {
        plOpaqueHeader *header = handle->as.opaque;

        if (handle->as.opaque) {
            header->cleanup_func(handle->as.opaque);
            handle->as.opaque = NULL;
        }
    }

    if (flags & PL_OBJ_FLAG_DYNAMIC) {
        free(handle->as.array);
        handle->as.array = NULL;
    }
}

plByteArray *
plNewByteArray(void)
{
    plByteArray *array;

    array = plSafeMalloc(sizeof(*array));
    *array = (plByteArray){0};
    return array;
}

int
plPopulateIntegerFromString(const char *string, unsigned int length, plInteger_t *integer)
{
    long long value;
    char array[PL_INTEGER_MAX_LENGTH + 1];
    char *temp;

    if (UNLIKELY(!string || !integer)) {
        VASQ_ERROR(debug_logger, "string and integer cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0 || string[0] == '\0') {
        VASQ_ERROR(debug_logger, "length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if (length >= sizeof(array)) {
        VASQ_ERROR(debug_logger, "String is too long");
        return PL_RET_BAD_DATA;
    }

    memcpy(array, string, length);
    array[length] = '\0';

    value = strtoll(array, &temp, 10);
    if (*temp != '\0') {
        return PL_RET_BAD_DATA;
    }

#if __LONG_LONG_WIDTH__ > PL_INTEGER_BIT_SIZE
    if (value < PL_MIN_INTEGER || value > PL_MAX_INTEGER) {
        VASQ_ERROR(debug_logger, "Integer is out of bounds");
        return PL_RET_BAD_DATA;
    }
#endif

    *integer = value;

    return PL_RET_OK;
}

int
plPopulateIntegerFromHexString(const char *string, unsigned int length, plInteger_t *integer)
{
    long long value;
    char array[PL_INTEGER_BIT_SIZE / 4 + 1];
    char *temp;

    if (UNLIKELY(!string || !integer)) {
        VASQ_ERROR(debug_logger, "string and integer cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0) {
        VASQ_ERROR(debug_logger, "Length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    if (length >= sizeof(array)) {
        VASQ_ERROR(debug_logger, "String is too long");
        return PL_RET_BAD_DATA;
    }

    memcpy(array, string, length);
    array[length] = '\0';

    value = strtoll(array, &temp, 16);
    if (*temp != '\0') {
        return PL_RET_BAD_DATA;
    }

#if __LONG_LONG_WIDTH__ > PL_INTEGER_BIT_SIZE
    if (value < 0 || value > PL_MAX_INTEGER) {
        VASQ_ERROR(debug_logger, "Integer is out of bounds");
        return PL_RET_BAD_DATA;
    }
#endif

    *integer = value;

    return PL_RET_OK;
}

int
plPopulateFloatFromString(const char *string, unsigned int length, plFloat_t *decimal)
{
    char *array, *temp;

    if (UNLIKELY(!string || !decimal)) {
        VASQ_ERROR(debug_logger, "string and decimal cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0) {
        VASQ_ERROR(debug_logger, "length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    array = VASQ_MALLOC(debug_logger, length + 1);
    if (!array) {
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(array, string, length);
    array[length] = '\0';

    errno = 0;
    *decimal = strtod(array, &temp);
    free(array);
    if (*temp != '\0' || errno != 0) {
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;
}

int
plConcatenateByteArrays(plByteArray *first, const plByteArray *second)
{
    uint32_t new_length;

    if (UNLIKELY(!first || !second)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL");
        return PL_RET_USAGE;
    }

    new_length = first->length + second->length;
    if (new_length < first->length) {
        VASQ_ERROR(debug_logger, "Integer overflow detected");
        return PL_RET_OVERFLOW;
    }

    if (new_length > first->capacity) {
        uint32_t new_capacity;

        new_capacity = CAPACITY_EXPANSION(new_length);
        if (new_capacity < new_length) {
            VASQ_ERROR(debug_logger, "Integer overflow detected");
            return PL_RET_OVERFLOW;
        }

        first->bytes = plSafeRealloc(first->bytes, new_capacity);
        first->capacity = new_capacity;
    }

    memcpy(first->bytes + first->length, second->bytes, second->length);
    first->length = new_length;

    return PL_RET_OK;
}
