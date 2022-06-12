#include "parserInternal.h"

int
plParseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node, bool compilation_only)
{
    int ret;
    plLexicalToken lead_token;
    plAstNode *array_node;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = CONSUME_TOKEN(scanner, &lead_token);
    if (ret != PL_RET_OK) {
        return ret;
    }
    if (UNLIKELY(lead_token.header.marker != PL_MARKER_LEFT_BRACKET)) {
        VASQ_ERROR(debug_logger, "Invalid token consumed (%s).",
                   plLexicalMarkerName(lead_token.header.marker));
        plTokenCleanup(&lead_token, scanner->table);
        return PL_RET_USAGE;
    }

    ret = plParseExpression(scanner, node, compilation_only);
    if (ret != PL_RET_OK) {
        return ret;
    }

    while (PEEK_TOKEN(scanner, 0) == PL_MARKER_COMMA) {
        plLexicalToken comma_token;
        plAstNode *next_node;

        ret = CONSUME_TOKEN(scanner, &comma_token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = plParseExpression(scanner, &next_node, compilation_only);
        if (ret != PL_RET_OK) {
            goto error;
        }

        plAstCreateConnection(PL_MARKER_COMMA, &comma_token, node, next_node);
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_RIGHT_BRACKET, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }
    array_node = plAstCreateFamily('A', &lead_token, *node);
    *node = array_node;

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
