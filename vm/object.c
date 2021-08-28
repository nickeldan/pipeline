#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#include "vasq/logger.h"

#include "object.h"

#define CAPACITY_EXPANSION(capacity) ((capacity)*5 / 4)

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

    if (flags & PL_OBJ_FLAG_DYNAMIC) {
        free(handle->as.array);
        handle->as.array = NULL;
    }
}

plObjectHandle
plBoolLiteral(bool value)
{
    return (plObjectHandle){.flags = PL_OBJ_TYPE_BOOL | PL_OBJ_FLAG_OWNED, .as.boolean = value};
}

plObjectHandle
plNullLiteral(void)
{
    return (plObjectHandle){.flags = PL_OBJ_TYPE_NULL | PL_OBJ_FLAG_OWNED};
}

plObjectHandle
plBlankLiteral(void)
{
    return (plObjectHandle){.flags = PL_OBJ_TYPE_BLANK | PL_OBJ_FLAG_OWNED};
}

plByteArray *
plNewByteArray(void)
{
    plByteArray *array;

    array = VASQ_MALLOC(debug_logger, sizeof(*array));
    if (array) {
        *array = (plByteArray){0};
    }
    return array;
}

int
plPopulateIntegerFromString(const char *string, unsigned int length, plInteger_t *integer)
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

    if (!string || !integer) {
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

    if (!string || !decimal) {
        VASQ_ERROR(debug_logger, "string and decimal cannot be NULL");
        return PL_RET_USAGE;
    }

    if (length == 0) {
        VASQ_ERROR(debug_logger, "length cannot be 0");
        return PL_RET_BAD_DATA;
    }

    array = alloca(length + 1);
    memcpy(array, string, length);
    array[length] = '\0';

    errno = 0;
    *decimal = strtod(array, &temp);
    if (*temp != '\0' || errno != 0) {
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;
}

int
plConcatenateByteArrays(plByteArray *first, const plByteArray *second)
{
    uint32_t new_length;

    new_length = first->length + second->length;
    if (new_length < first->length) {
        VASQ_ERROR(debug_logger, "Integer overflow detected");
        return PL_RET_OVERFLOW;
    }

    if (new_length > first->capacity) {
        uint32_t new_capacity;
        uint8_t *success;

        new_capacity = CAPACITY_EXPANSION(new_length);
        if (new_capacity < new_length) {
            VASQ_ERROR(debug_logger, "Integer overflow detected");
            return PL_RET_OVERFLOW;
        }

        success = VASQ_REALLOC(debug_logger, first->bytes, new_capacity);
        if (!success) {
            return PL_RET_OUT_OF_MEMORY;
        }

        first->bytes = success;
        first->capacity = new_capacity;
    }

    memcpy(first->bytes + first->length, second->bytes, second->length);
    first->length = new_length;

    return PL_RET_OK;
}
