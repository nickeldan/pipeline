#include "generateInternal.h"

int
plCompileTypeDecl(plSemanticContext *sem, plAstNode *node)
{
    const char *name;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = NODE_EXTRACT_NAME(splitter->nodes[0]);
    ref = plFindReference(sem, name, NULL);
    if (ref) {
        CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name, plRefTypeName(ref->flags));
        return PL_RET_BAD_DATA;
    }

    node = splitter->nodes[1];
    if (node->header.marker == PL_MARKER_TYPE) {
        plRefValue value = {
            .marker = PL_MARKER_TYPE, .submarker = node->header.submarker, .contains_data = false};

        return plStoreReference(sem->stack[1], name, PL_REF_FLAG_TYPE, &value, &node->header.location);
    }

    PLACEHOLDER();
    return PL_RET_USAGE;
}
