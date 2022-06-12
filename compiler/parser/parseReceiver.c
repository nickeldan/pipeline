#include "parserInternal.h"

int
plParseReceiver(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken arrow_token;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *second_node;

        switch (PEEK_TOKEN(scanner, 0)) {
        case PL_MARKER_UNDERSCORE:
            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
            second_node = plAstNew(PL_MARKER_UNDERSCORE, &token);
            break;

        case PL_MARKER_PIPE:
        case PL_MARKER_SINK:
        case PL_MARKER_LOCAL:
            ret = plParseFunction(scanner, &second_node, false);
            if (ret != PL_RET_OK) {
                goto error;
            }
            break;

        default:
            ret = plParseExtendedType(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }
            break;
        }

        if (PEEK_TOKEN(scanner, 0) == PL_MARKER_COLON) {
            plLexicalToken colon_token, name_token;
            plAstNode *name_node;

            if (second_node->header.marker == PL_MARKER_TYPE ||
                second_node->header.marker == PL_MARKER_UNDERSCORE) {
                PARSER_ERROR("Unexpected ':' following %s.",
                             plLexicalMarkerName(second_node->header.marker));
                ret = PL_RET_BAD_DATA;
                goto loop_error;
            }

            ret = CONSUME_TOKEN(scanner, &colon_token);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }

            ret = EXPECT_MARKER(scanner, PL_MARKER_NAME, &name_token);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }
            name_node = plAstNew(PL_MARKER_NAME, &name_token);

            plAstCreateConnection(PL_MARKER_COLON, &colon_token, &second_node, name_node);
        }

        if (*node) {
            ret = plAstCreateConnection(PL_MARKER_ARROW, &arrow_token, node, second_node);
            if (ret != PL_RET_OK) {
                goto loop_error;
            }
        }
        else {
            *node = second_node;
        }

        if (second_node->header.marker == PL_MARKER_UNDERSCORE ||
            PEEK_TOKEN(scanner, 0) != PL_MARKER_ARROW) {
            break;
        }

        ret = CONSUME_TOKEN(scanner, &arrow_token);
        if (ret != PL_RET_OK) {
            goto error;
        }

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
