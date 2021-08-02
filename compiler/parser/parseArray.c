#include "parserInternal.h"

int
plParseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node, bool compilation_only)
{
    int ret;
    plLexicalLocation location;
    plAstNode *array_node;

    plGetLastLocation(scanner, &location);

    ret = plParseExpression(scanner, node, compilation_only);
    if (ret != PL_RET_OK) {
        return ret;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *second_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_BRACKET) {
            break;
        }
        else if (token.marker != PL_MARKER_COMMA) {
            PARSER_ERROR("Expected ',' or ']' following expression in array declaration.");
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        ret = plParseExpression(scanner, &second_node, compilation_only);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = plAstCreateConnection(PL_MARKER_COMMA, node, second_node);
        if (ret != PL_RET_OK) {
            plAstFree(second_node, scanner->table);
            goto error;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
    }

    array_node = plAstCreateFamily('A', *node);
    if (!array_node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&array_node->token.location, &location, sizeof(location));
    *node = array_node;

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
