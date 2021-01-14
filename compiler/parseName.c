#include <string.h>

#include "parserInternal.h"

int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret = PL_RET_OK;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
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
            plAstSetLocation(period_node, &token.location);

            ret = NEXT_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        else {
            period_node = NULL;
        }

        if (token.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Unexpected %s where NAME was expected.", plLexicalMarkerName(token.marker));
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
        memcpy(&name_node->token, &token, sizeof(token));

        if (period_node) {
            plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)period_node;

            splitter->nodes[0] = *node;
            splitter->nodes[1] = name_node;
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
