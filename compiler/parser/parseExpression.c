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
    plLexicalToken token, negation_token = {0};
    plAstNode *second_node;

    *node = NULL;

start:

    switch (PEEK_TOKEN(scanner, 0)) {
    case PL_MARKER_LEFT_PARENS:
        ret = CONSUME_TOKEN(scanner, NULL);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (PEEK_TOKEN(scanner, 0) == PL_MARKER_SOURCE) {
            plLexicalToken arrow_token;

            if (compilation_only) {
                PARSER_ERROR(
                    "SOURCE is not allowed in an expression which must be resolved at compile time.");
                return PL_RET_BAD_DATA;
            }

            ret = plParseFunction(scanner, node, false);
            if (ret != PL_RET_OK) {
                return ret;
            }

            ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, &arrow_token);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = plParseReceiver(scanner, &second_node);
            if (ret != PL_RET_OK) {
                goto error;
            }

            plAstCreateConnection(PL_MARKER_ARROW, &arrow_token, node, second_node);
        }
        else {
            plOperatorOrder_t new_order = compilation_only ? PL_ORDER_START : PL_ORDER_ARROW;

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
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        negation = !negation;
        if (negation) {
            memcpy(&negation_token, &token, sizeof(token));
        }
        goto start;

    case PL_MARKER_NAME:
        ret = plParseExtendedName(scanner, node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (PEEK_TOKEN(scanner, 0) == PL_MARKER_LEFT_PARENS) {
            plOperatorOrder_t new_order = compilation_only ? PL_ORDER_START : PL_ORDER_COMMA;

            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }

            ret = parseExpressionStart(scanner, &second_node, new_order, compilation_only);
            if (ret != PL_RET_OK) {
                goto error;
            }

            plAstCreateConnection(PL_MARKER_LEFT_PARENS, &token, node, second_node);

            ret = EXPECT_MARKER(scanner, PL_MARKER_RIGHT_PARENS, NULL);
            if (ret != PL_RET_OK) {
                goto error;
            }
        }
        break;

    case PL_MARKER_OBJECT:
    case PL_MARKER_TYPE:
    case PL_MARKER_CONTEXT:
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }
        *node = plAstNew(token.header.marker, &token);

        if (token.header.marker == PL_MARKER_OBJECT &&
            OBJ_TYPE(&token.data.handle) == PL_OBJ_TYPE_BYTE_ARRAY) {
            while (PEEK_TOKEN(scanner, 0) == PL_MARKER_OBJECT &&
                   OBJ_TYPE(&scanner->store[0].data.handle) == PL_OBJ_TYPE_BYTE_ARRAY) {
                ret = CONSUME_TOKEN(scanner, &token);
                if (ret != PL_RET_OK) {
                    goto error;
                }
                ret = plConcatenateByteArrays(AST_DATA(*node)->handle.as.bytes, token.data.handle.as.bytes);
                plTokenCleanup(&token, scanner->table);
                if (ret != PL_RET_OK) {
                    goto error;
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

    case PL_MARKER_UNDERSCORE:
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret == PL_RET_OK) {
            *node = plAstNew(PL_MARKER_UNDERSCORE, &token);
        }
        return ret;

    default:
        scanner->error_on_peek = 0;
        PARSER_ERROR("Unexpected %s in place of expression.", plLexicalMarkerName(PEEK_TOKEN(scanner, 0)));
        // I don't need to call plTokenCleanup since I know that it's neither a NAME nor an OBJECT.
        return PL_RET_BAD_DATA;
    }

    // Check for attributes and array indexing.
    while (PEEK_TOKEN(scanner, 0) == PL_MARKER_LEFT_BRACKET || PEEK_TOKEN(scanner, 0) == PL_MARKER_PERIOD) {
        ret = CONSUME_TOKEN(scanner, &token);
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
        else {
            ret = plParseExtendedName(scanner, &second_node);
        }

        if (ret != PL_RET_OK) {
            if (token.header.marker == PL_MARKER_LEFT_BRACKET) {
                plAstFree(second_node, scanner->table);
            }
            goto error;
        }

        plAstCreateConnection(token.header.marker, &token, node, second_node);
    }

    if (negation_token.header.location.line_no > 0) {
        // Even if negation is false (meaning that the NOT token appeared an even number of times), the
        // value of the expression must still be converted into a Bool.
        for (int k = 0; k <= 1 - negation; k++) {  // negation is either 0 or 1.
            *node = plAstCreateFamily(PL_MARKER_NOT, &negation_token, *node);
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
    plOperatorOrder_t new_order;

    while ((new_order = operatorOrder(&scanner->store[0])) <= order) {
        plLexicalToken connector_token;
        plAstNode *second_node;

        if ((*current)->header.marker == PL_MARKER_COMMA && order < PL_ORDER_COMMA &&
            PEEK_TOKEN(scanner, 0) != PL_MARKER_ARROW) {
            scanner->error_on_peek = 0;
            PARSER_ERROR("Expected ARROW after comma-delimited source list.");
            return PL_RET_BAD_DATA;
        }

        if (new_order < order) {
            ret = parseExpressionRecurse(scanner, current, new_order, compilation_only);
            if (ret != PL_RET_OK) {
                return ret;
            }
            continue;
        }

        ret = CONSUME_TOKEN(scanner, &connector_token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if ( connector_token.header.marker == PL_MARKER_ARROW ) {
            ret = plParseReceiver(scanner, &second_node);
            if ( ret == PL_RET_OK ) {
                plAstCreateConnection(PL_MARKER_ARROW, &connector_token, current, second_node);
            }
            return ret;
        }

        ret = parseMonomial(scanner, &second_node, (order > PL_ORDER_NUMERIC), compilation_only);
        if (ret != PL_RET_OK) {
            return ret;
        }

        while ((new_order = operatorOrder(&scanner->store[0])) < order) {
            ret = parseExpressionRecurse(scanner, &second_node, new_order, compilation_only);
            if (ret != PL_RET_OK) {
                plAstFree(second_node, scanner->table);
                return ret;
            }
        }

        plAstCreateConnection(connector_token.header.marker, &connector_token, current, second_node);
    }

    return PL_RET_OK;
}

int
plParseExpression(plLexicalScanner *scanner, plAstNode **node, bool compilation_only)
{
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        if (node) {
            *node = NULL;
        }
        return PL_RET_USAGE;
    }

    return parseExpressionStart(scanner, node, PL_ORDER_START, compilation_only);
}
