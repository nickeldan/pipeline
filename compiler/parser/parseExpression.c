#include <string.h>

#include "parserInternal.h"

typedef enum plOperatorOrder {
    PL_ORDER_BIT = 0,
    PL_ORDER_MULTIPLY,
    PL_ORDER_ADD,
    PL_ORDER_COMPARISON,
    PL_ORDER_LOGICAL,
    PL_ORDER_COMMA,
    PL_ORDER_ARROW,

    PL_ORDER_NA,
} plOperatorOrder_t;
#define PL_ORDER_START   PL_ORDER_LOGICAL
#define PL_ORDER_NUMERIC PL_ORDER_ADD

static int
parseMonomial(plLexicalScanner *scanner, plAstNode **node, bool allow_negation, bool compilation_only);

static int
parseExpressionRecurse(plLexicalScanner *scanner, plAstNode **node, plOperatorOrder_t order,
                       bool compilation_only);

static plOperatorOrder_t
operatorOrder(const plLexicalToken *token)
{
    switch (token->header.marker) {
    case PL_MARKER_ARITHMETIC:
        switch (token->header.submarker) {
        case PL_SUBMARKER_AND:
        case PL_SUBMARKER_OR: return PL_ORDER_BIT;

        case PL_SUBMARKER_MULTIPLY:
        case PL_SUBMARKER_DIVIDE:
        case PL_SUBMARKER_MODULO: return PL_ORDER_MULTIPLY;

        case PL_SUBMARKER_PLUS:
        case PL_SUBMARKER_MINUS:
        case PL_SUBMARKER_LSHIFT:
        case PL_SUBMARKER_RSHIFT: return PL_ORDER_ADD;

        default: return PL_ORDER_NA;  // This should never happen.
        }

    case PL_MARKER_COMPARISON:
    case PL_MARKER_IS: return PL_ORDER_COMPARISON;

    case PL_MARKER_LOGICAL: return PL_ORDER_LOGICAL;

    case PL_MARKER_COMMA: return PL_ORDER_COMMA;

    case PL_MARKER_ARROW: return PL_ORDER_ARROW;

    default: return PL_ORDER_NA;
    }
}

static int
parseExpressionStart(plLexicalScanner *scanner, plAstNode **node, plOperatorOrder_t order,
                     bool compilation_only)
{
    int ret;

    ret = parseMonomial(scanner, node, (order > PL_ORDER_NUMERIC), compilation_only);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = parseExpressionRecurse(scanner, node, order, compilation_only);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table);
        *node = NULL;
    }
    return ret;
}

static int
parseMonomial(plLexicalScanner *scanner, plAstNode **node, bool allow_negation, bool compilation_only)
{
    int ret;
    bool negation = false;
    plLexicalToken token;
    plLexicalLocation negation_location = {0};
    plAstNode *second_node;

    *node = NULL;

start:

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    switch (token.header.marker) {
    case PL_MARKER_LEFT_PARENS:
        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (token.header.marker == PL_MARKER_SOURCE) {
            plLexicalLocation arrow_location;

            if (compilation_only) {
                PARSER_ERROR(
                    "SOURCE is not allowed in an expression which must be resolved at compile time.");
                return PL_RET_BAD_DATA;
            }

            ret = plParseFunction(scanner, node, false);
            if (ret != PL_RET_OK) {
                return ret;
            }

            ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, &arrow_location);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = plParseReceiver(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = plAstCreateConnection(PL_MARKER_ARROW, node, second_node);
            if (ret != PL_RET_OK) {
                plAstFree(second_node, scanner->table);
                goto error;
            }
            memcpy(&(*node)->header.location, &arrow_location, sizeof(arrow_location));
        }
        else {
            plOperatorOrder_t new_order;

            LOOKAHEAD_STORE(scanner, &token);

            new_order = compilation_only ? PL_ORDER_START : PL_ORDER_ARROW;
            ret = parseExpressionStart(scanner, node, new_order, compilation_only);
            if (ret != PL_RET_OK) {
                return ret;
            }
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_RIGHT_PARENS, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }
        break;

    case PL_MARKER_NOT:
        if (!allow_negation) {
            PARSER_ERROR("NOT not allowed in this context.");
            return PL_RET_BAD_DATA;
        }

        negation = !negation;
        if (negation) {
            memcpy(&negation_location, &token.header.location, sizeof(token.header.location));
        }
        goto start;

    case PL_MARKER_NAME:
        LOOKAHEAD_STORE(scanner, &token);

        ret = plParseExtendedName(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_LEFT_PARENS) {
            plOperatorOrder_t new_order = compilation_only ? PL_ORDER_START : PL_ORDER_COMMA;

            ret = parseExpressionStart(scanner, &second_node, new_order, compilation_only);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = plAstCreateConnection(PL_MARKER_LEFT_PARENS, node, second_node);
            if (ret != PL_RET_OK) {
                plAstFree(second_node, scanner->table);
                goto error;
            }
            memcpy(&(*node)->header.location, &token.header.location, sizeof(token.header.location));

            ret = EXPECT_MARKER(scanner, PL_MARKER_RIGHT_PARENS, NULL);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        else {
            LOOKAHEAD_STORE(scanner, &token);
        }
        break;

    case PL_MARKER_OBJECT:
    case PL_MARKER_TYPE:
    case PL_MARKER_CONTEXT:
        *node = plAstNew(token.header.marker);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        plAstCopyTokenInfo(*node, &token);

        if (token.header.marker == PL_MARKER_OBJECT &&
            OBJ_TYPE(&token.data.handle) == PL_OBJ_TYPE_BYTE_ARRAY) {
            while (true) {
                plLexicalToken token2;

                ret = NEXT_TOKEN(scanner, &token2);
                if (ret != PL_RET_OK) {
                    goto error;
                }

                if (token2.header.marker == PL_MARKER_OBJECT &&
                    OBJ_TYPE(&token2.data.handle) == PL_OBJ_TYPE_BYTE_ARRAY) {
                    ret = plConcatenateByteArrays(plAstGetData(*node)->handle.as.bytes,
                                                  token2.data.handle.as.bytes);
                    plTokenCleanup(&token2, scanner->table);
                    if (ret != PL_RET_OK) {
                        goto error;
                    }
                }
                else {
                    LOOKAHEAD_STORE(scanner, &token2);
                    break;
                }
            }
        }

        break;

    case PL_MARKER_LEFT_BRACKET:
        ret = plParseArrayDeclaration(scanner, node, compilation_only);
        if (ret != PL_RET_OK) {
            return ret;
        }
        break;

    default:
        PARSER_ERROR("Unexpected %s in place of expression.", plLexicalMarkerName(token.header.marker));
        // I don't need to call plTokenCleanup since I know that it's neither a NAME nor an OBJECT.
        return PL_RET_BAD_DATA;
    }

    // Check for attributes and array indexing.
    while (true) {
        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_LEFT_BRACKET) {
            ret = parseExpressionStart(scanner, &second_node, PL_ORDER_NUMERIC, compilation_only);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = EXPECT_MARKER(scanner, PL_MARKER_RIGHT_BRACKET, NULL);
        }
        else if (token.header.marker == PL_MARKER_PERIOD) {
            ret = plParseExtendedName(scanner, &second_node);
        }
        else {
            LOOKAHEAD_STORE(scanner, &token);
            break;
        }

        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = plAstCreateConnection(token.header.marker, node, second_node);
        if (ret != PL_RET_OK) {
            plAstFree(second_node, scanner->table);
            goto error;
        }
        plAstCopyTokenInfo(*node, &token);
    }

    if (negation_location.line_no > 0) {
        // Even if negation is false (meaning that the NOT token appeared an even number of times), the
        // value of the expression must still be converted into a Bool.
        for (int k = 0; k <= 1 - (int)negation; k++) {  // negation, as an integer, is either 0 or 1.
            plAstNode *connector_node;

            connector_node = plAstCreateFamily(PL_MARKER_NOT, *node);
            if (!connector_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            memcpy(&connector_node->header.location, &negation_location, sizeof(negation_location));
            *node = connector_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}

static int
parseExpressionRecurse(plLexicalScanner *scanner, plAstNode **current, plOperatorOrder_t order,
                       bool compilation_only)
{
    int ret;
    plLexicalToken token1;
    plAstNode *second_node = NULL;

    while (true) {
        plOperatorOrder_t new_order;
        plLexicalToken token2;

        ret = NEXT_TOKEN(scanner, &token2);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if ((*current)->header.marker == PL_MARKER_COMMA && order < PL_ORDER_COMMA &&
            token2.header.marker != PL_MARKER_ARROW) {
            PARSER_ERROR("Expected ARROW after comma-delimited source list.");
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        new_order = operatorOrder(&token2);
        if (new_order != order) {
            LOOKAHEAD_STORE(scanner, &token2);

            if (new_order > order) {
                break;
            }

            ret = parseExpressionRecurse(scanner, second_node ? &second_node : current, new_order,
                                         compilation_only);
            if (ret != PL_RET_OK) {
                goto error;
            }

            continue;
        }

        if (token2.header.marker == PL_MARKER_ARROW) {
            // second_node must be NULL here.
            ret = plParseReceiver(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = plAstCreateConnection(PL_MARKER_ARROW, current, second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }
            plAstCopyTokenInfo(*current, &token2);

            return PL_RET_OK;
        }

        if (second_node) {
            ret = plAstCreateConnection(token1.header.marker, current, second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }
            plAstCopyTokenInfo(*current, &token1);
            second_node = NULL;
        }
        else {
            memcpy(&token1, &token2, sizeof(token2));
        }

        ret = parseMonomial(scanner, &second_node, (order > PL_ORDER_NUMERIC), compilation_only);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    if (second_node) {
        ret = plAstCreateConnection(token1.header.marker, current, second_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
        plAstCopyTokenInfo(*current, &token1);
    }

    return PL_RET_OK;

error:

    plAstFree(second_node, scanner->table);
    return ret;
}

int
plParseExpression(plLexicalScanner *scanner, plAstNode **node, bool compilation_only)
{
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        if (node) {
            *node = NULL;
        }
        return PL_RET_USAGE;
    }

    return parseExpressionStart(scanner, node, PL_ORDER_START, compilation_only);
}
