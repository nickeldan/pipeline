#include <string.h>

#include "parserInternal.h"

int
parseExtendedType(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker == PL_MARKER_TYPE) {
        *node = plAstNew(PL_MARKER_TYPE);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
    }
    else {
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = parseExtendedName(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_QUESTION) {
            plAstNode *question_node;

            question_node = createFamily(PL_MARKER_QUESTION, *node);
            if (!question_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            memcpy(&question_node->token, &token, sizeof(token));
            *node = question_node;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
