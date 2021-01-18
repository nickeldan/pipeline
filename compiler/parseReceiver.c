#include <string.h>

#include "parserInternal.h"

int
parseReceiver(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation arrow_location;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *second_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_UNDERSCORE) {
            second_node = plAstNew(PL_MARKER_UNDERSCORE);
            if (!second_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            memcpy(&second_node->token, &token, sizeof(token));
            goto connect_nodes;
        }
        else if (token.marker == PL_MARKER_PIPE || token.marker == PL_MARKER_SINK ||
                 token.marker == PL_MARKER_LOCAL) {
            ret = parseFunction(scanner, &second_node, true);
            if (ret != PL_RET_OK) {
                goto error;
            }
            memcpy(&second_node->token, &token, sizeof(token));
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = parseExtendedType(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        if (token.marker == PL_MARKER_COLON) {
            plLexicalToken token2;
            plAstNode *store_node;

            if (second_node->token.marker == PL_MARKER_TYPE) {
                PARSER_ERROR("Unexpected ':' following TYPE.");
                ret = PL_RET_BAD_DATA;
                goto loop_error;
            }

            ret = NEXT_TOKEN(scanner, &token2);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }

            if (token2.marker != PL_MARKER_NAME) {
                PARSER_ERROR("Expected NAME following ':', not %s.", plLexicalMarkerName(token2.marker));
                plTokenCleanup(&token2, scanner->table);
                ret = PL_RET_BAD_DATA;
                goto loop_error;
            }

            store_node = plAstNew(PL_MARKER_NAME);
            if (!store_node) {
                plTokenCleanup(&token2, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto loop_error;
            }
            memcpy(&store_node->token, &token2, sizeof(token2));

            ret = createConnection(PL_MARKER_COLON, &second_node, store_node);
            if (ret != PL_RET_OK) {
                plAstFree(store_node, scanner->table);
                goto loop_error;
            }
            memcpy(&second_node->token, &token, sizeof(token));
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }
        }

connect_nodes:

        if (*node) {
            ret = createConnection(PL_MARKER_ARROW, node, second_node);
            if (ret != PL_RET_OK) {
                plAstFree(second_node, scanner->table);
                goto error;
            }
            memcpy(&(*node)->token.location, &arrow_location, sizeof(arrow_location));
        }
        else {
            *node = second_node;
        }

        if (second_node->token.marker == PL_MARKER_UNDERSCORE) {
            break;
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker != PL_MARKER_ARROW) {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
            break;
        }

        memcpy(&arrow_location, &token.location, sizeof(token.location));
        continue;

loop_error:

        plAstFree(second_node, scanner->table);
        goto error;
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
