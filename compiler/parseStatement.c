#include "parserInternal.h"

static bool
isLvalue(const plAstNode *node)
{
    const plAstMaxSplitNode *splitter = (const plAstMaxSplitNode *)node;

    switch (node->token.marker) {
    case PL_MARKER_NAME: return true;

    case PL_MARKER_PERIOD:
    case PL_MARKER_LEFT_BRACKET: return isLvalue(splitter->nodes[0]);

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

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    switch (token.marker) {
    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT:
        ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            return ret;
        }

        *node = plAstNew(token.marker);
        if (!*node) {
            return PL_RET_OUT_OF_MEMORY;
        }
        (*node)->token.line_no = token.line_no;

        return PL_RET_OK;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
        ret = parseExpression(scanner, &first_node, false);
        if (ret != PL_RET_OK) {
            return ret;
        }

        ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        *node = plAstNew(token.marker);
        if (!*node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        (*node)->token.line_no = token.line_no;
        createFamily(*node, first_node);

        return PL_RET_OK;

    case PL_MARKER_IF: return parseIfBlock(scanner, node);

    case PL_MARKER_WHILE: return parseWhileBlock(scanner, node);

    default: break;
    }

    if (token.marker == PL_MARKER_NAME) {
        plLexicalToken next_token;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &next_token))) {
            plTokenCleanup(&token, scanner->table);
            return translateTerminalMarker(scanner->last_marker);
        }

        if (next_token.marker == PL_MARKER_AS) {
            plAstNode *name_node, *type_node;

            ret = parseExtendedType(scanner, &type_node);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                return ret;
            }

            ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
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
            memcpy(&name_node->token, &token, sizeof(token));

            *node = plAstNew(next_token.marker);
            if (*node) {
                plAstFree(name_node, scanner->table);
                plAstFree(type_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            (*node)->token.line_no = next_token.line_no;
            createFamily(*node, name_node, type_node);
            return PL_RET_OK;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &next_token);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                plTokenCleanup(&next_token, scanner->table);
                return ret;
            }
        }
    }
    else if (token.marker == PL_MARKER_ARITHMETIC && token.submarker == PL_SUBMARKER_MODULO) {
        plLexicalToken next_token;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &next_token))) {
            return translateTerminalMarker(scanner->last_marker);
        }

        if (next_token.marker == PL_MARKER_NAME) {
            plAstNode *name_node;

            name_node = plAstNew(PL_MARKER_NAME);
            if (!name_node) {
                plTokenCleanup(&next_token, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            memcpy(&name_node->token, &next_token, sizeof(next_token));

            *node = plAstNew('%');
            if (!*node) {
                plAstFree(name_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            (*node)->token.line_no = token.line_no;
            createFamily(*node, name_node);

            return PL_RET_OK;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &next_token);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&next_token, scanner->table);
                return ret;
            }
        }
    }

    ret = LOOKAHEAD_STORE(scanner, &token);
    if (ret != PL_RET_OK) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OK;
    }

    ret = parseExpression(scanner, &first_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        goto error;
    }

    if (token.marker == PL_MARKER_REASSIGNMENT) {
        plAstNode *rvalue_node;

        if (!isLvalue(first_node)) {
            COMPILER_ERROR("Invalid REASSIGNMENT following what is not an lvalue");
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        ret = parseExpression(scanner, &rvalue_node, false);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            plAstFree(rvalue_node, scanner->table);
            goto error;
        }

        *node = plAstNew(PL_MARKER_REASSIGNMENT);
        if (!*node) {
            plAstFree(rvalue_node, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
        createFamily(*node, first_node, rvalue_node);

        return PL_RET_OK;
    }

    if (token.marker != PL_MARKER_ARROW) {
        COMPILER_ERROR("Unexpected %s following expression", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    ret = parseReceiver(scanner, &receiver_node);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(receiver_node, scanner->table);
        goto error;
    }

    *node = plAstNew(PL_MARKER_ARROW);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    (*node)->token.line_no = token.line_no;
    createFamily(*node, first_node, receiver_node);

    return PL_RET_OK;

error:

    plAstFree(first_node, scanner->table);
    plAstFree(receiver_node, scanner->table);

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

int
parseStatementList(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    *node = NULL;

    while (true) {
        plAstNode *statement_node;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_BRACE) {
            break;
        }

        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        ret = parseStatement(scanner, &statement_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            plAstNode *semicolon_node;

            semicolon_node = plAstNew(PL_MARKER_SEMICOLON);
            if (!semicolon_node) {
                plAstFree(statement_node, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            semicolon_node->token.line_no = token.line_no;
            createFamily(semicolon_node, *node, statement_node);
            *node = semicolon_node;
        }
        else {
            *node = statement_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}
