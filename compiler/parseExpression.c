#include "parserInternal.h"

int
parseExpression(plLexicalScanner *scanner, plAstNode **node, bool inside_parentheses)
{
    int ret;
    unsigned int negation_line_no = 0;
    bool negation = false;
    plLexicalToken token;
    plAstNode *connector_node = NULL, *second_node = NULL;

    *node = NULL;

start:

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    switch (token.marker) {
        unsigned int arrow_line_no;

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

        if (negation_line_no > 0) {
            COMPILER_ERROR("SOURCE cannot be negated.");
            return PL_RET_BAD_DATA;
        }

        ret = parseInlineFunction(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = expectMarker(scanner, PL_MARKER_ARROW, &arrow_line_no);
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
        connector_node->token.line_no = arrow_line_no;
        createFamily(connector_node, *node, second_node);
        *node = connector_node;
        connector_node = NULL;
        second_node = NULL;
        goto done;

    case PL_MARKER_NOT:
        negation = !negation;
        if (negation) {
            negation_line_no = token.line_no;
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
            goto error;
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
            connector_node->token.line_no = token.line_no;
            createFamily(connector_node, *node, second_node);
            *node = connector_node;
            connector_node = NULL;
            second_node = NULL;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        break;

    case PL_MARKER_OBJECT:
    case PL_MARKER_OPTION:
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

    case PL_MARKER_RIGHT_PARENS:
        if (inside_parentheses) {
            if (negation_line_no > 0) {
                COMPILER_ERROR("There is nothing to negate.");
            }
            else {
                COMPILER_ERROR("Empty parentheses.");
            }
            return PL_RET_BAD_DATA;
        }
        /* FALLTHROUGH */
    default:
        COMPILER_ERROR("Unexpected %s in place of expression.", plLexicalMarkerName(token.marker));
        // I don't need to call plTokenCleanup since I know that it's neither a NAME nor an OBJECT.
        return PL_RET_BAD_DATA;
    }

    if (negation_line_no == 0) {
        // Check for attributes and array indexing.
        while (true) {
            if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
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
            connector_node->token.line_no = token.line_no;
            createFamily(connector_node, *node, second_node);
            *node = connector_node;
            connector_node = NULL;
            second_node = NULL;
        }
    }
    else if ((*node)->token.marker == PL_MARKER_COMMA) {
        COMPILER_ERROR("COMMA-delimited list cannot be negated.");
        ret = PL_RET_BAD_DATA;
        goto error;
    }
    else if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        goto error;
    }
    else {
        // Even if negation is false (meaning that the NOT token appeared an even number of times), the
        // value of the expression must still be converted into a Bool.
        for (int k = 0; k <= 1 - (int)negation; k++) {  // negation, as an integer, is either 0 or 1.
            connector_node = plAstNew(PL_MARKER_NOT);
            if (!connector_node) {
                plTokenCleanup(&token, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            connector_node->token.line_no = negation_line_no;
            createFamily(connector_node, *node);
            *node = connector_node;
            connector_node = NULL;
        }
    }

    if ((*node)->token.marker == PL_MARKER_COMMA && token.marker != PL_MARKER_ARROW) {
        COMPILER_ERROR("COMMA-delimited list must be followed by ARROW and not %s.",
                       plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    switch (token.marker) {
    case PL_MARKER_ARROW:
        if (!inside_parentheses) {
            COMPILER_ERROR("Invalid ARROW in expression without parentheses.");
            ret = PL_RET_BAD_DATA;
            goto error;
        }
        ret = parseReceiver(scanner, &second_node);
        break;

    case PL_MARKER_IS:
        ret = parseExtendedType(scanner, &second_node);
        break;
        ;

    case PL_MARKER_ARITHMETIC:
        switch (token.submarker) {
        case PL_SUBMARKER_PLUS:
            // continue coding here
        }
        break;

    case PL_MARKER_LOGICAL:
        // continue coding here
        break;

    case PL_MARKER_COMPARISON:
        // continue coding here
        break;

    case PL_MARKER_COMMA:
        if (inside_parentheses) {
            return PL_RET_OK;
        }
        /* FALLTHROUGH */
    default:
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
        goto done;
    }

    if (ret != PL_RET_OK) {
        goto error;
    }

    connector_node = plAstNew(token.marker);
    if (!connector_node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&connector_node->token, &token, sizeof(token));
    createFamily(connector_node, *node, second_node);
    *node = connector_node;
    connector_node = NULL;
    second_node = NULL;

done:

    if (inside_parentheses) {
        ret = expectMarker(scanner, PL_MARKER_RIGHT_PARENS, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    plAstFree(connector_node, scanner->table);
    plAstFree(second_node, scanner->table);

    *node = NULL;

    return ret;
}
