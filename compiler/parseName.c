#include "parserInternal.h"

int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret = PL_RET_OK;
    plLexicalToken token;
    plAstNode *period_node = NULL;

    *node = NULL;

    while (true) {
        int previous_marker;
        plAstNode *name_node;

        previous_marker = scanner->last_marker;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            if (token.marker != PL_MARKER_PERIOD) {
                ret = LOOKAHEAD_STORE(scanner, &token);
                if (ret != PL_RET_OK) {
                    goto error;
                }
                break;
            }

            period_node = plAstNew(PL_MARKER_PERIOD);
            if (!period_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            period_node->token.line_no = token.line_no;

            ret = NEXT_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        else {
            period_node = NULL;
        }

        if (token.marker != PL_MARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s", plLexicalMarkerName(token.marker),
                           plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        if (period_node) {
            createFamily(period_node, *node, name_node);
            *node = period_node;
        }
        else {
            *node = name_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    plAstFree(period_node, scanner->table);

    *node = NULL;

    return ret;
}
