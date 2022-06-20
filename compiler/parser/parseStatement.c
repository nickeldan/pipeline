#include <string.h>

#include "parserInternal.h"

static bool
isLvalue(const plAstNode *node)
{
    switch (node->header.marker) {
    case PL_MARKER_NAME: return true;

    case PL_MARKER_PERIOD:
    case PL_MARKER_LEFT_BRACKET: return isLvalue(AST_CHILD(node, 0));

    default: return false;
    }
}

static int
parseStatement(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;
    plAstNode *first_node = NULL, *receiver_node = NULL;

    *node = NULL;

    switch (PEEK_TOKEN(scanner, 0)) {
    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT:
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            return ret;
        }

        *node = plAstNew(token.header.marker, &token);

        return PL_RET_OK;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_EXIT:
        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = plParseExpression(scanner, &first_node, (token.header.marker == PL_MARKER_EXIT));
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        *node = plAstCreateFamily(token.header.marker, &token, first_node);

        return PL_RET_OK;

    case PL_MARKER_IF: return plParseIfBlock(scanner, node);

    case PL_MARKER_WHILE: return plParseWhileBlock(scanner, node);

    case PL_MARKER_NAME:
        if (PEEK_TOKEN(scanner, 1) == PL_MARKER_AS) {
            plLexicalToken as_token;
            plAstNode *name_node, *type_node;

            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                return ret;
            }
            name_node = plAstNew(PL_MARKER_NAME, &token);

            if ((ret = CONSUME_TOKEN(scanner, &as_token)) != PL_RET_OK ||
                (ret = plParseExtendedType(scanner, &type_node)) != PL_RET_OK) {
                plAstFree(name_node, scanner->table);
                return ret;
            }

            *node = plAstCreateFamily(PL_MARKER_AS, &as_token, name_node, type_node);

            ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
            if (ret != PL_RET_OK) {
                goto error;
            }

            return PL_RET_OK;
        }
        break;

    case PL_MARKER_ARITHMETIC:
        if (scanner->store[0].header.submarker == PL_SUBMARKER_MODULO) {
            plLexicalToken name_token;
            plAstNode *name_node;

            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                return ret;
            }

            ret = CONSUME_TOKEN(scanner, &name_token);
            if (ret != PL_RET_OK) {
                return ret;
            }
            if (name_token.header.marker != PL_MARKER_NAME) {
                PARSER_ERROR("Expected NAME following '%'");
                plTokenCleanup(&name_token, scanner->table);
                return PL_RET_BAD_DATA;
            }
            name_node = plAstNew(PL_MARKER_NAME, &name_token);

            *node = plAstCreateFamily('%', &token, name_node);

            return PL_RET_OK;
        }
        break;

    default: break;
    }

    ret = plParseExpression(scanner, &first_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = CONSUME_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }

    if (token.header.marker == PL_MARKER_REASSIGNMENT) {
        plAstNode *rvalue_node;

        if (!isLvalue(first_node)) {
            PARSER_ERROR("Invalid REASSIGNMENT following what is not an lvalue");
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        ret = plParseExpression(scanner, &rvalue_node, false);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            plAstFree(rvalue_node, scanner->table);
            goto error;
        }

        *node = plAstCreateFamily(PL_MARKER_REASSIGNMENT, &token, first_node, rvalue_node);

        return PL_RET_OK;
    }

    if (token.header.marker != PL_MARKER_ARROW) {
        PARSER_ERROR("Unexpected %s following expression", plLexicalMarkerName(token.header.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    ret = plParseReceiver(scanner, &receiver_node);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(receiver_node, scanner->table);
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_ARROW, &token, first_node, receiver_node);

    return PL_RET_OK;

error:

    plAstFree(first_node, scanner->table);
    plAstFree(receiver_node, scanner->table);

    return ret;
}

int
plParseStatementList(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (PEEK_TOKEN(scanner, 0) != PL_MARKER_RIGHT_BRACE) {
        plAstNode *statement_node;

        if (PEEK_TOKEN(scanner, 0) == PL_MARKER_SEMICOLON) {
            ret = CONSUME_TOKEN(scanner, NULL);
            if (ret != PL_RET_OK) {
                goto error;
            }
            continue;
        }

        ret = parseStatement(scanner, &statement_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            plAstCreateConnection(PL_MARKER_SEMICOLON, NULL, node, statement_node);
        }
        else {
            *node = statement_node;
        }
    }

    ret = CONSUME_TOKEN(scanner, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
