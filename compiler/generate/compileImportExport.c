#include "generateInternal.h"

static int
addModuleToSemantics(plSemanticContext *sem, const plLexicalLocation *location, const char *name,
                     plModule **module)
{
    (void)sem;
    (void)location;
    (void)name;
    (void)module;
    return PL_RET_USAGE;  // placeholder
}

int
compileImportExport(plSemanticContext *sem, plAstNode *node)
{
    int ret;
    size_t idx;
    const char *name;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;
    plModule *module;
    plReference *ref;

    name = splitter->nodes[0]->token.ctx.name;
    ref = findReference(sem, name, &idx);

    if (node->token.marker == PL_MARKER_IMPORT) {
        if (ref) {
            if (idx == 0) {
                CONTEXT_ERROR(node, "%s is a built-in.", name);
                return PL_RET_BAD_DATA;
            }
            else if (ref->flags & PL_REF_FLAG_MODULE) {
                CONTEXT_WARNING(node, "%s was already imported on line %u.", name, ref->location.line_no);
            }
            else if (ref->flags != PL_REF_FLAG_EXPORT) {
                CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name,
                              plRefTypeName(ref->flags), ref->location.line_no);
                return PL_RET_BAD_DATA;
            }
        }

        ret = addModuleToSemantics(sem, &node->token.location, name, &module);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (ref) {
            ref->data = module;
            ref->flags |= PL_REF_FLAG_MODULE;
        }
        else {
            return storeReference(sem->stack[1], name, PL_REF_FLAG_MODULE, module, &node->token.location);
        }
    }
    else {
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
        }
        else {
            return storeReference(sem->stack[1], name, PL_REF_FLAG_EXPORT, NULL, &node->token.location);
        }
    }

    return PL_RET_OK;
}
