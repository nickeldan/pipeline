#include <string.h>

#include "parserInternal.h"

int
plParseWhileBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plAstNode *condition_node, *statement_list = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = plParseExpression(scanner, &condition_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = plParseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_WHILE, condition_node, statement_list);
    memcpy(&(*node)->header.location, &location, sizeof(location));

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);

    return ret;
}
