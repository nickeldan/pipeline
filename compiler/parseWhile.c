#include "parserInternal.h"

int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plAstNode *condition_node, *statement_list;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = parseExpression(scanner, &condition_node);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(condition_node, scanner->table);
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        plAstFree(condition_node, scanner->table);
        return ret;
    }

    *node = createFamily(PL_MARKER_WHILE, condition_node, statement_list);
    if (!*node) {
        plAstFree(condition_node, scanner->table);
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    plAstSetLocation(*node, &location);

    return PL_RET_OK;
}