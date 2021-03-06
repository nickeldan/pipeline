#include <string.h>

#include "parserInternal.h"

int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plAstNode *condition_node, *statement_list = NULL;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = parseExpression(scanner, &condition_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_WHILE, condition_node, statement_list);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);

    return ret;
}
