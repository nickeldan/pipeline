#include <string.h>

#include "parserInternal.h"

int
plParseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret = PL_RET_OK;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *period_node, *name_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            if (token.header.marker != PL_MARKER_PERIOD) {
                LOOKAHEAD_STORE(scanner, &token);
                break;
            }

            period_node = plAstNew(PL_MARKER_PERIOD);
            if (!period_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            plAstCopyTokenInfo(period_node, &token);

            ret = NEXT_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        else {
            period_node = NULL;
        }

        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Unexpected %s where NAME was expected.", plLexicalMarkerName(token.header.marker));
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_BAD_DATA;
            goto loop_error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto loop_error;
        }
        plAstCopyTokenInfo(name_node, &token);

        if (period_node) {
            if (!plAstSetChild(period_node, 0, *node) || !plAstSetChild(period_node, 1, name_node)) {
                ret = PL_RET_USAGE;
                goto loop_error;
            }
            *node = period_node;
        }
        else {
            *node = name_node;
        }

        continue;

loop_error:

        plAstFree(period_node, scanner->table);
        goto error;
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

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.header.marker == PL_MARKER_TYPE) {
        *node = plAstNew(PL_MARKER_TYPE);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        plAstCopyTokenInfo(*node, &token);
    }
    else {
        LOOKAHEAD_STORE(scanner, &token);

        ret = plParseExtendedName(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_QUESTION) {
            plAstNode *question_node;

            question_node = plAstCreateFamily(PL_MARKER_QUESTION, *node);
            if (!question_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            plAstCopyTokenInfo(question_node, &token);
            *node = question_node;
        }
        else {
            LOOKAHEAD_STORE(scanner, &token);
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
