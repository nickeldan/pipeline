#include <alloca.h>
#include <errno.h>`
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plObject.h"
#include "plUtil.h"

// External variables
plObject trueObject = {.flags = PL_OBJ_TYPE_BOOL | PL_OBJ_FLAG_TRUTHY | PL_OBJ_FLAG_STATIC};
plObject falseObject = {.flags = PL_OBJ_TYPE_BOOL | PL_OBJ_FLAG_STATIC};
plObject blankObject = {.flags = PL_OBJ_TYPE_BLANK | PL_OBJ_FLAG_STATIC};
plObject nullObject = {.flags = PL_OBJ_FLAG_STATIC};

static size_t
objectSize(uint32_t flags) __attribute__((pure));

void
freeObject(plObject* object) {
    uint32_t flags;

    if (!object) {
        return;
    }

    flags = object->flags;

    if (flags & PL_OBJ_PRED_TRUE_ARRAY) {
        plArray* array;

        array = (plArray*)object;
        if (array->objects) {
            uint32_t length;

            length = array->length;
            for (unsigned int k = 0; k < length; k++) {
                freeObject(array->objects[k]);
            }

            free(array->objects);
        }
    } else if (flags & PL_OBJ_PRED_BYTE_ARRAY) {
        if (!(flags & PL_OBJ_FLAG_STATIC_BYTES)) {
            free(((plByteArray*)object)->bytes);
        }
    }

    if (!(flags & PL_OBJ_FLAG_STATIC)) {
        free(object);
    } else if (flags & (PL_OBJ_PRED_NUM | PL_OBJ_PRED_GEN_ARRAY)) {
        BZERO((void*)object + sizeof(plObject), objectSize(flags) - sizeof(plObject));
    }
}

plObject*
copyObject(const plObject* object) {
    plObject* new;

    new = calloc(1, objectSize(object->flags));
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
            plByteArray* bytes = (plByteArray*)new;

            bytes->bytes = malloc(bytes->capacity);
            if (!bytes->bytes) {
                goto error;
            }
            memcpy(bytes->bytes, ((plByteArray*)object)->bytes, bytes->length);
        } else {
            plArray* trueArray = (plArray*)new;
            uint32_t length = trueArray->length;

            if ((object->flags) & PL_OBJ_PRED_STRUCT) {
                plStruct* st = (plStruct*)new;

                st->structId = ((plStruct*)object)->structId;
            }

            trueArray->objects = calloc(trueArray->length, sizeof(plObject*));
            if (!trueArray->objects) {
                goto error;
            }
            for (uint32_t k = 0; k < length; k++) {
                trueArray->objects[k] = copyObject(((plArray*)object)->objects[k]);
                if (!trueArray->objects[k]) {
                    goto error;
                }
            }
        }
    } else {
        memcpy(new, object, objectSize(object->flags));
    }

    return new;

error:

    freeObject(new);

    return NULL;
}

int
plNumFromString(plInteger** num, const char* string, size_t len) {
    int ret;
    char *terminatedString, *period, *temp;

    terminatedString = alloca(len + 1);
    sprintf(terminatedString, "%.*s", len, string);

    period = strstr(terminatedString, ".");
    *num = malloc(period ? sizeof(plFlot) : sizeof(plInteger));
    if (!*num) {
        return PL_ERROR_OUT_OF_MEMORY;
    }

    if (period) {
        period[0] = '\0';
    }
    errno = 0;
    *num->value = strtoll(terminatedString, &temp, 10);
    if (errno != 0 || temp[0] != '\0') {
        goto error;
    }
    if (period) {
        period[0] = '.';

        errno = 0;
        ((plFloat*)*num)->decimal = strtod(period, &temp);
        if (errno != 0 || temp[0] != '\0') {
            goto error;
        }
    }

    *num->flags = period ? PL_OBJ_TYPE_FLOAT : PL_OBJ_TYPE_INT;
    return PL_ERROR_OK;

error:

    ret = (errno == ERANGE) ? PL_ERROR_INTEGER_OVERFLOW : PL_ERROR_BAD_INPUT;
    free(*num);
    return ret;
}

static size_t
objectSize(uint32_t flags) {
    switch (flags & 0x000000ff) {
    case PL_OBJ_TYPE_INT: return sizeof(plInteger);

    case PL_OBJ_TYPE_FLOAT: return sizeof(plFloat);

    case PL_OBJ_TYPE_ARRAY:
    case PL_OBJ_TYPE_BYTE_ARRAY: return sizeof(plArray);

    case PL_OBJ_TYPE_STRUCT: return sizeof(plStruct);

    default: return sizeof(plObject);
    }
}
