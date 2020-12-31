#include "parseInternal.h"

int
parseStructDeclaration(plLexicalScanner *scanner, plAstNode **node)
{
    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    VASQ_ERROR("This funciton has not yet been implemented.");
    return PL_RET_BAD_DATA;
}
