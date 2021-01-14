#include "parserInternal.h"

int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *condition_node, *statement_list = NULL, *else_node = NULL;

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

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }

    switch (token.marker) {
    case PL_MARKER_EIF: ret = parseIfBlock(scanner, &else_node); break;

    case PL_MARKER_ELSE:
        ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }
        ret = parseStatementList(scanner, &else_node);
        break;

    default: ret = LOOKAHEAD_STORE(scanner, &token); break;
    }

    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = createFamily(PL_MARKER_IF, condition_node, statement_list, else_node);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    plAstSetLocation(*node, &location);

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);
    plAstFree(else_node, scanner->table);

    return ret;
}
