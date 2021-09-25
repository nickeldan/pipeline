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
    const char *name;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;
    plModule *module;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = NODE_EXTRACT_NAME(splitter->nodes[0]);
    ref = plFindReference(sem, name, &idx);
    if (ref) {
        if (idx == 0) {
            CONTEXT_ERROR(node, "%s is a built-in.", name);
            return PL_RET_BAD_DATA;
        }
        else if (ref->flags & PL_REF_FLAG_MODULE) {
            CONTEXT_WARNING(node, "%s was already imported on line %u.", name, ref->location.line_no);
            return PL_RET_OK;
        }
        else if (ref->flags != PL_REF_FLAG_EXPORT) {
            CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name,
                          plRefTypeName(ref->flags), ref->location.line_no);
            return PL_RET_BAD_DATA;
        }
    }

    ret = addModuleToSemantics(sem, &node->header.location, name, &module);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (ref) {
        ref->value.data = module;
        ref->flags |= PL_REF_FLAG_MODULE;
        return PL_RET_OK;
    }
    else {
        plRefValue value = {.data = module, .contains_data = true};

        return plStoreReference(sem->stack[1], name, PL_REF_FLAG_MODULE, &value, &node->header.location);
    }
}

int
plCompileExport(plSemanticContext *sem, plAstNode *node)
{
    size_t idx;
    const char *name;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = NODE_EXTRACT_NAME(splitter->nodes[0]);
    ref = plFindReference(sem, name, &idx);
    if (ref) {
        if (idx == 0) {
            CONTEXT_ERROR(node, "%s is a built-in.", name);
            return PL_RET_BAD_DATA;
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
