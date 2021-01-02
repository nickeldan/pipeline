#include "parserInternal.h"

static int
parseMonomial(plLexicalScanner *scanner, plAstNode **node, bool inside_parentheses)
{
    int ret;
    plLexicalLocation negation_location = {0};
    bool negation = false;
    plLexicalToken token;
    plAstNode *connector_node, *second_node;

    *node = NULL;

start:

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    switch (token.marker) {
        plLexicalLocation arrow_location;

    case PL_MARKER_LEFT_PARENS:
        ret = parseExpression(scanner, node, true);
        if (ret != PL_RET_OK) {
            return ret;
        }
        break;

    case PL_MARKER_SOURCE:
        if (!inside_parentheses) {
            COMPILER_ERROR("Invalid SOURCE in expression without parentheses.");
            return PL_RET_BAD_DATA;
        }

        if (negation_location.line_no > 0) {
            COMPILER_ERROR("SOURCE cannot be negated.");
            return PL_RET_BAD_DATA;
        }

        ret = parseInlineFunction(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = expectMarker(scanner, PL_MARKER_ARROW, &arrow_location);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseReceiver(scanner, &second_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        connector_node = plAstNew(PL_MARKER_ARROW);
        if (!connector_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        plAstSetLocation(connector_node, &arrow_location);
        createFamily(connector_node, *node, second_node);
        *node = connector_node;
        return PL_RET_OK;

    case PL_MARKER_NOT:
        negation = !negation;
        if (negation) {
            memcpy(&negation_location, &token.location, sizeof(token.location));
        }
        goto start;

    case PL_MARKER_NAME:
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
            return ret;
        }

        if (token.marker == PL_MARKER_LEFT_PARENS) {
            ret = parseStructDeclaration(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }

            connector_node = plAstNew('C');
            if (!connector_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            plAstSetLocation(connector_node, &token.location);
            createFamily(connector_node, *node, second_node);
            *node = connector_node;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        break;

    case PL_MARKER_OBJECT:
    case PL_MARKER_CONTEXT:
        *node = plAstNew(token.marker);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
        break;

    case PL_MARKER_LEFT_BRACKET:
        ret = parseArrayDeclaration(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }
        break;

    default:
        COMPILER_ERROR("Unexpected %s in place of expression.", plLexicalMarkerName(token.marker));
        // I don't need to call plTokenCleanup since I know that it's neither a NAME nor an OBJECT.
        return PL_RET_BAD_DATA;
    }

    // Check for attributes and array indexing.
    while (true) {
        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_LEFT_BRACKET) {
            ret = parseExpression(scanner, &second_node, false);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = expectMarker(scanner, PL_MARKER_RIGHT_BRACKET, NULL);
        }
        else if (token.marker == PL_MARKER_PERIOD) {
            ret = parseExtendedName(scanner, &second_node);
        }
        else {
            break;
        }

        if (ret != PL_RET_OK) {
            goto error;
        }

        connector_node = plAstNew(token.marker);
        if (!connector_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        plAstSetLocation(connector_node, &token.location);
        createFamily(connector_node, *node, second_node);
        *node = connector_node;
    }

    if (negation_location.line_no > 0) {
        // Even if negation is false (meaning that the NOT token appeared an even number of times), the
        // value of the expression must still be converted into a Bool.
        for (int k = 0; k <= 1 - (int)negation; k++) {  // negation, as an integer, is either 0 or 1.
            connector_node = plAstNew(PL_MARKER_NOT);
            if (!connector_node) {
                plTokenCleanup(&token, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            plAstSetLocation(connector_node, &negation_location);
            createFamily(connector_node, *node);
            *node = connector_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}

int
parseExpression(plLexicalScanner *scanner, plAstNode **node, bool inside_parentheses)
{
    //    int ret;
    //    plLexicalToken token;
    (void)inside_parentheses;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("scanner and node cannot be NULL.");

        return PL_RET_USAGE;
    }

    VASQ_ERROR("This function has not yet been implemented.");
    return PL_RET_BAD_DATA;
}
