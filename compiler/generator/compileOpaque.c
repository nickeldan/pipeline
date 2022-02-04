#include <string.h>

#include "generateInternal.h"

int
plCompileOpaque(plSemanticContext *sem, plAstNode *node)
{
    size_t idx;
    const char *name;
    plReference *ref;

    if (UNLIKELY(!sem || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = plAstGetData(node)->name;
    ref = plFindReference(sem, name, &idx);
    if (ref) {
        if (ref->flags & PL_REF_FLAG_ERROR) {}
        else if (ref->flags == PL_REF_FLAG_EXPORT) {
            ref->flags |= PL_REF_FLAG_OBJECT | PL_OBJ_TYPE_OPAQUE;
        }
        else if (idx == 0) {
            CONTEXT_ERROR(node, "%s is a built-in.", name);
        }
        else if (ref->flags & PL_REF_FLAG_OBJECT) {
            if ((ref->flags & PL_OBJ_MASK) == PL_OBJ_TYPE_OPAQUE) {
                CONTEXT_WARNING(node, "%s was already defined as opaque on line %u.", name,
                                ref->location.line_no);
            }
            else {
                CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name,
                              plObjectTypeName(ref->flags), ref->location.line_no);
            }
        }
        else {
            CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name,
                          plRefTypeName(ref->flags), ref->location.line_no);
        }

        return PL_RET_OK;
    }
    else {
        return plStoreReference(sem->stack[1], name, PL_REF_FLAG_TYPE | PL_OBJ_TYPE_OPAQUE, NULL,
                                &node->header.location);
    }
}
