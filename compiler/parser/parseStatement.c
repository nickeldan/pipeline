#include <string.h>

#include "parserInternal.h"

static bool
isLvalue(const plAstNode *node)
{
    switch (node->header.marker) {
    case PL_MARKER_NAME: return true;

    case PL_MARKER_PERIOD:
    case PL_MARKER_LEFT_BRACKET: return isLvalue(plAstGetChild(node, 0));

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

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    switch (token.header.marker) {
    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT:
        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            return ret;
        }

        *node = plAstNew(token.header.marker);
        if (!*node) {
            return PL_RET_OUT_OF_MEMORY;
        }
        plAstCopyTokenInfo(*node, &token);

        return PL_RET_OK;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
        ret = plParseExpression(scanner, &first_node, (token.header.marker == PL_MARKER_ABORT));
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        *node = plAstCreateFamily(token.header.marker, first_node);
        if (!*node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        plAstCopyTokenInfo(*node, &token);

        return PL_RET_OK;

    case PL_MARKER_IF: return plParseIfBlock(scanner, node);

    case PL_MARKER_WHILE: return plParseWhileBlock(scanner, node);

    default: break;
    }

    if (token.header.marker == PL_MARKER_NAME) {
        plLexicalToken next_token;

        ret = NEXT_TOKEN(scanner, &next_token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            return ret;
        }

        if (next_token.header.marker == PL_MARKER_AS) {
            plAstNode *name_node, *type_node;

            ret = plParseExtendedType(scanner, &type_node);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                return ret;
            }

            ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                plAstFree(type_node, scanner->table);
                return ret;
            }

            name_node = plAstNew(PL_MARKER_NAME);
            if (!name_node) {
                plTokenCleanup(&token, scanner->table);
                plAstFree(type_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            plAstCopyTokenInfo(name_node, &token);

            *node = plAstCreateFamily(next_token.header.marker, name_node, type_node);
            if (*node) {
                plAstFree(name_node, scanner->table);
                plAstFree(type_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            plAstCopyTokenInfo(*node, &next_token);

            return PL_RET_OK;
        }
        else {
            LOOKAHEAD_STORE(scanner, &next_token);
        }
    }
    else if (token.header.marker == PL_MARKER_ARITHMETIC && token.header.submarker == PL_SUBMARKER_MODULO) {
        plLexicalToken next_token;

        ret = NEXT_TOKEN(scanner, &next_token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (next_token.header.marker == PL_MARKER_NAME) {
            plAstNode *name_node;

            name_node = plAstNew(PL_MARKER_NAME);
            if (!name_node) {
                plTokenCleanup(&next_token, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            plAstCopyTokenInfo(name_node, &next_token);

            *node = plAstCreateFamily('%', name_node);
            if (!*node) {
                plAstFree(name_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            memcpy(&(*node)->header.location, &token.header.location, sizeof(token.header.location));

            return PL_RET_OK;
        }
        else {
            LOOKAHEAD_STORE(scanner, &next_token);
        }
    }

    LOOKAHEAD_STORE(scanner, &token);

    ret = plParseExpression(scanner, &first_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = NEXT_TOKEN(scanner, &token);
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

        *node = plAstCreateFamily(PL_MARKER_REASSIGNMENT, first_node, rvalue_node);
        if (!*node) {
            plAstFree(rvalue_node, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        plAstCopyTokenInfo(*node, &token);

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

    *node = plAstCreateFamily(PL_MARKER_ARROW, first_node, receiver_node);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    plAstCopyTokenInfo(*node, &token);

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

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    while (true) {
        plLexicalToken token;
        plAstNode *statement_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_RIGHT_BRACE) {
            break;
        }

        LOOKAHEAD_STORE(scanner, &token);

        ret = parseStatement(scanner, &statement_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            ret = plAstCreateConnection(PL_MARKER_SEMICOLON, node, statement_node);
            if (ret != PL_RET_OK) {
                plAstFree(statement_node, scanner->table);
                goto error;
            }
        }
        else {
            *node = statement_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}
