#include "parserInternal.h"

int
plParseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_NAME, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }
    *node = plAstNew(PL_MARKER_NAME, &token);

    while (PEEK_TOKEN(scanner, 0) == PL_MARKER_PERIOD) {
        plLexicalToken period_token;
        plAstNode *name_node;

        ret = CONSUME_TOKEN(scanner, &period_token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_NAME, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
        name_node = plAstNew(PL_MARKER_NAME, &token);

        plAstCreateConnection(PL_MARKER_PERIOD, &period_token, node, name_node);
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}

int
plParseExtendedType(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (PEEK_TOKEN(scanner, 0) == PL_MARKER_TYPE) {
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }
        *node = plAstNew(PL_MARKER_TYPE, &token);
    }
    else {
        ret = plParseExtendedName(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (PEEK_TOKEN(scanner, 0) == PL_MARKER_QUESTION) {
            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
            *node = plAstCreateFamily(PL_MARKER_QUESTION, &token, *node);
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;
    return ret;
}
