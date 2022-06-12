#include "parserInternal.h"

int
plParseIfBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken if_token;
    plAstNode *condition_node, *statement_list = NULL, *else_node = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = CONSUME_TOKEN(scanner, &if_token);
    if (ret != PL_RET_OK) {
        return ret;
    }

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

    switch (PEEK_TOKEN(scanner, 0)) {
    case PL_MARKER_EIF: ret = plParseIfBlock(scanner, &else_node); break;

    case PL_MARKER_ELSE:
        if ((ret = CONSUME_TOKEN(scanner, NULL)) != PL_RET_OK ||
            (ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL)) != PL_RET_OK) {
            goto error;
        }
        ret = plParseStatementList(scanner, &else_node);
        break;

    default: goto skip_over_check;
    }

    if (ret != PL_RET_OK) {
        goto error;
    }

skip_over_check:

    *node = plAstCreateFamily(PL_MARKER_IF, &if_token, condition_node, statement_list, else_node);

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);
    plAstFree(else_node, scanner->table);

    return ret;
}
