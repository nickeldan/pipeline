#include "generateInternal.h"

static int
addModuleToSemantics(plSemanticContext *sem, const plLexicalLocation *location, const char *name,
                     plModule **module)
{
    (void)sem;
    (void)location;
    (void)name;
    (void)module;
    PLACEHOLDER();
    return PL_RET_USAGE;
}

int
plCompileImport(plSemanticContext *sem, plAstNode *node)
{
    int ret;
    size_t idx;
    uint32_t flags = PL_REF_FLAG_MODULE;
    const char *name;
    plModule *module;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = plAstGetData(node)->name;
    ref = plFindReference(sem, name, &idx);
    if (ref) {
        if (idx == 0) {
            CONTEXT_ERROR(node, "%s is a built-in.", name);
        }
        else if (ref->flags & PL_REF_FLAG_MODULE) {
            CONTEXT_WARNING(node, "%s was already imported on line %u.", name, ref->location.line_no);
        }
        else if (ref->flags != PL_REF_FLAG_EXPORT) {
            CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name,
                          plRefTypeName(ref->flags), ref->location.line_no);
        }
        else if (!(ref->flags & PL_REF_FLAG_ERROR)) {
            goto add_module;
        }

        return PL_RET_OK;
    }

add_module:

    ret = addModuleToSemantics(sem, &node->header.location, name, &module);
    if (ret == PL_RET_OUT_OF_MEMORY) {
        return ret;
    }
    else if (ret != PL_RET_OK) {
        flags |= PL_REF_FLAG_ERROR;
    }

    if (ref) {
        ref->value.data = module;
        ref->flags = flags;
        return PL_RET_OK;
    }
    else {
        plRefValue value = {.data = module, .contains_data = true};

        return plStoreReference(sem->stack[1], name, flags, &value, &node->header.location);
    }
}

int
plCompileExport(plSemanticContext *sem, plAstNode *node)
{
    size_t idx;
    const char *name;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = plAstGetData(node)->name;
    ref = plFindReference(sem, name, &idx);
    if (ref) {
        if (idx == 0) {
            CONTEXT_ERROR(node, "%s is a built-in.", name);
            ref->flags |= PL_REF_FLAG_ERROR;
        }
        else if (ref->flags & PL_REF_FLAG_EXPORT) {
            CONTEXT_WARNING(node, "%s was already exported on line %u.", name, ref->location.line_no);
        }
        else {
            ref->flags |= PL_REF_FLAG_EXPORT;
        }

        return PL_RET_OK;
    }
    else {
        return plStoreReference(sem->stack[1], name, PL_REF_FLAG_EXPORT, NULL, &node->header.location);
    }
}
