#include "parserInternal.h"

int
parseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node)
{
    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    VASQ_ERROR("This function has not yet been implemented.");
    return PL_RET_BAD_DATA;
}
