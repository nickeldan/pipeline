#include "generateInternal.h"

int
plCompileStructDefinition(plSemanticContext *sem, plAstNode *node)
{
    plReference *ref;
    plStructRef *sref;

    if (!sem || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    PLACEHOLDER();
    (void)ref;
    (void)sref;
    return PL_RET_USAGE;
}
