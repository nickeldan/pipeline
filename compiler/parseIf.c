#include "parserInternal.h"

int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *condition_node, *statement_list = NULL, *eif_node = NULL, *last_eif_node = NULL,
                               *else_node = NULL;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = parseExpression(scanner, &condition_node);
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

    while (true) {
        plAstNode *eif2_node, *eif_condition_node, *eif_statement_list = NULL;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
        if (token.marker != PL_MARKER_EIF) {
            break;
        }

        ret = parseExpression(scanner, &eif_condition_node);
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

        eif2_node = createFamily(PL_MARKER_EIF, eif_condition_node, eif_statement_list, NULL);
        if (!eif2_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto eif_loop_error;
        }
        memcpy(&eif2_node->token, &token, sizeof(token));

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

        else_node = createFamily(PL_MARKER_ELSE, else_statement_list);
        if (!else_node) {
            plAstFree(else_statement_list, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&else_node->token, &token, sizeof(token));
    }
    else {
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    *node = createFamily(PL_MARKER_IF, condition_node, statement_list, eif_node, else_node);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    plAstSetLocation(*node, &location);

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);
    plAstFree(eif_node, scanner->table);
    plAstFree(else_node, scanner->table);

    return ret;
}
