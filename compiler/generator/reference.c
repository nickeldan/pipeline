#include <stdlib.h>
#include <string.h>

#include "reference.h"

const char *
plRefTypeName(uint32_t flags)
{
    flags &= 0xff000000;
    flags &= ~PL_REF_FLAG_EXPORT;

    switch (flags) {
    case PL_REF_FLAG_OBJECT: return "OBJECT";
    case PL_REF_FLAG_TYPE: return "TYPE";
    case PL_REF_FLAG_SOURCE: return "SOURCE";
    case PL_REF_FLAG_PIPE: return "PIPE";
    case PL_REF_FLAG_SINK: return "SINK";
    default: return "MODULE";
    }
}

plReference *
plNewReference(void)
{
    plReference *ref;

    ref = VASQ_MALLOC(debug_logger, sizeof(*ref));
    if (ref) {
        *ref = (plReference){0};
    }
    return ref;
}

int
plStoreReference(plRefTable *table, const char *symbol, uint32_t flags, const plRefValue *value,
                 const plLexicalLocation *location)
{
    plReference *ref;

    if (UNLIKELY(!table || !symbol || !location)) {
        VASQ_ERROR(debug_logger, "table, symbol, and location cannot be NULL.");
        return PL_RET_USAGE;
    }

    ref = plNewReference();
    if (!ref) {
        return PL_RET_OUT_OF_MEMORY;
    }

    ref->flags = flags;
    if (value) {
        memcpy(&ref->value, value, sizeof(*value));
    }
    else {
        ref->value.data = NULL;
        ref->value.contains_data = true;
    }
    memcpy(&ref->location, location, sizeof(*location));

    if (!plUpdateRef(table, symbol, ref)) {
        free(ref);
        return PL_RET_OUT_OF_MEMORY;
    }

    return PL_RET_OK;
}
