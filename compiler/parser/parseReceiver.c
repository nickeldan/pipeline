#include <string.h>

#include "parserInternal.h"

int
plParseReceiver(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation arrow_location;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *second_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_UNDERSCORE) {
            second_node = plAstNew(PL_MARKER_UNDERSCORE);
            if (!second_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            plAstCopyTokenInfo(second_node, &token);
            goto connect_nodes;
        }
        else if (token.header.marker == PL_MARKER_PIPE || token.header.marker == PL_MARKER_SINK ||
                 token.header.marker == PL_MARKER_LOCAL) {
            ret = plParseFunction(scanner, &second_node, false);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        else {
            LOOKAHEAD_STORE(scanner, &token);

            ret = plParseExtendedType(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }

            if (second_node->header.marker != PL_MARKER_TYPE) {
                ret = NEXT_TOKEN(scanner, &token);
                if (ret != PL_RET_OK) {
                    goto loop_error;
                }

                if (token.header.marker == PL_MARKER_LEFT_PARENS) {
                    plAstNode *attached_node;

                    ret = plParseExpression(scanner, &attached_node, false);
                    if (ret != PL_RET_OK) {
                        goto loop_error;
                    }

                    ret = plAstCreateConnection(PL_MARKER_LEFT_PARENS, &second_node, attached_node);
                    if (ret != PL_RET_OK) {
                        plAstFree(attached_node, scanner->table);
                        goto loop_error;
                    }
                    plAstCopyTokenInfo(second_node, &token);
                }
                else {
                    LOOKAHEAD_STORE(scanner, &token);
                }
            }
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        if (token.header.marker == PL_MARKER_COLON) {
            plLexicalToken token2;
            plAstNode *store_node;

            if (second_node->header.marker == PL_MARKER_TYPE) {
                PARSER_ERROR("Unexpected ':' following TYPE.");
                ret = PL_RET_BAD_DATA;
                goto loop_error;
            }

            ret = NEXT_TOKEN(scanner, &token2);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }

            if (token2.header.marker != PL_MARKER_NAME) {
                PARSER_ERROR("Expected NAME following ':', not %s.",
                             plLexicalMarkerName(token2.header.marker));
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
            plAstCopyTokenInfo(store_node, &token2);

            ret = plAstCreateConnection(PL_MARKER_COLON, &second_node, store_node);
            if (ret != PL_RET_OK) {
                plAstFree(store_node, scanner->table);
                goto loop_error;
            }
            plAstCopyTokenInfo(second_node, &token);
        }
        else {
            LOOKAHEAD_STORE(scanner, &token);
        }

connect_nodes:

        if (*node) {
            ret = plAstCreateConnection(PL_MARKER_ARROW, node, second_node);
            if (ret != PL_RET_OK) {
                plAstFree(second_node, scanner->table);
                goto error;
            }
            memcpy(&(*node)->header.location, &arrow_location, sizeof(arrow_location));
        }
        else {
            *node = second_node;
        }

        if (second_node->header.marker == PL_MARKER_UNDERSCORE) {
            break;
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker != PL_MARKER_ARROW) {
            LOOKAHEAD_STORE(scanner, &token);
            break;
        }

        memcpy(&arrow_location, &token.header.location, sizeof(token.header.location));
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
