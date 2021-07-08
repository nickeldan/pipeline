#include "generateInternal.h"

int
compileTypeDecl(plSemanticContext *sem, plAstNode *node)
{
    const char *name;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;
    plReference *ref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    name = splitter->nodes[0]->token.ctx.name;
    ref = findReference(sem, name, NULL);
    if (ref) {
        CONTEXT_ERROR(node, "%s was already defined as a %s on line %u.", name, plRefTypeName(ref->flags));
        return PL_RET_BAD_DATA;
    }

    node = splitter->nodes[1];
    if (node->token.marker == PL_MARKER_TYPE) {
        plRefValue value = {
            .marker = PL_MARKER_TYPE, .submarker = node->token.submarker, .contains_data = false};

        return storeReference(sem->stack[1], name, PL_REF_FLAG_TYPE, &value, &node->token.location);
    }

    return PL_RET_USAGE; // placeholder
}
