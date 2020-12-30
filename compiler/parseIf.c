#include "parserInternal.h"

int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    unsigned int line_no;
    plLexicalToken token;
    plAstNode *condition_node, *statement_list = NULL, *eif_node = NULL, *last_eif_node = NULL,
                               *else_node = NULL;

    line_no = plLastLineNo(scanner);
    *node = NULL;

    ret = parseExpression(scanner, &condition_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    while (TOKEN_READ(scanner, &token) == PL_MARKER_EIF) {
        plAstNode *eif2_node, *eif_condition_node, *eif_statement_list = NULL;

        ret = parseExpression(scanner, &eif_condition_node, false);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
        if (ret != PL_RET_OK) {
            goto eif_loop_error;
        }

        ret = parseStatementList(scanner, &eif_statement_list);
        if (ret != PL_RET_OK) {
            goto eif_loop_error;
        }

        eif2_node = plAstNew(PL_MARKER_EIF);
        if (!eif2_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto eif_loop_error;
        }
        eif2_node->token.line_no = token.line_no;
        createFamily(eif2_node, eif_condition_node, eif_statement_list, NULL);

        if (last_eif_node) {
            ((plAstThreeSplitNode *)last_eif_node)->nodes[2] = eif2_node;
        }
        else {
            eif_node = eif2_node;
        }
        last_eif_node = eif2_node;

        continue;

eif_loop_error:

        plAstFree(eif_condition_node, scanner->table);
        plAstFree(eif_statement_list, scanner->table);
        goto error;
    }
    if (TERMINAL_MARKER(token.marker)) {
        goto error;
    }

    if (token.marker == PL_MARKER_ELSE) {
        plAstNode *else_statement_list;

        ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseStatementList(scanner, &else_statement_list);
        if (ret != PL_RET_OK) {
            goto error;
        }

        else_node = plAstNew(PL_MARKER_ELSE);
        if (!else_node) {
            plAstFree(else_statement_list, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        else_node->token.line_no = token.line_no;
        createFamily(else_node, else_statement_list);
    }
    else {
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            goto error;
        }
    }

    *node = plAstNew(PL_MARKER_IF);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    (*node)->token.line_no = line_no;
    createFamily(*node, condition_node, statement_list, eif_node, else_node);

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);
    plAstFree(eif_node, scanner->table);
    plAstFree(else_node, scanner->table);

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}
