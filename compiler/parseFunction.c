#include "parserInternal.h"

int
parseFunction(plLexicalScanner *scanner, plAstNode **node)
{
    int ret, function_marker = scanner->last_marker;
    unsigned int line_no;
    plLexicalToken token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list;

    *node = NULL;
    line_no = plLastLineNo(scanner);

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != PL_MARKER_NAME) {
        COMPILER_ERROR("Anonymous %s not allowed in this context", plLexicalMarkerName(function_marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    function_name_node = plAstNew(PL_MARKER_NAME);
    if (!function_name_node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&function_name_node->token, &token, sizeof(token));

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }

    if (token.marker != PL_MARKER_LEFT_PARENS) {
        COMPILER_ERROR("Unexpected %s before argument list", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    while (true) {
        int previous_marker = scanner->last_marker;
        unsigned int line_no;
        plAstNode *name_node, *arg_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_PARENS) {
            if (previous_marker == PL_MARKER_COMMA) {
                COMPILER_ERROR("Unexpected ')' following ',' in argument list");
                ret = PL_RET_BAD_DATA;
                goto error;
            }
            break;
        }

        if (token.marker == PL_MARKER_COMMA) {
            if (previous_marker == PL_MARKER_LEFT_PARENS || previous_marker == PL_MARKER_COMMA) {
                COMPILER_ERROR("Unexpected ',' following %s in argument list",
                               plLexicalMarkerName(previous_marker));
                ret = PL_RET_BAD_DATA;
                goto error;
            }

            if (function_marker != PL_MARKER_SOURCE) {
                COMPILER_ERROR("Multiple arguments are only allowed in a SOURCE");
                ret = PL_RET_BAD_DATA;
                goto error;
            }

            continue;
        }

        if (token.marker != PL_MARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s in argunent list", plLexicalMarkerName(token.marker),
                           plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_BAD_DATA;
            goto error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        ret = expectMarker(scanner, PL_MARKER_COLON, &line_no);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        arg_node = plAstNew(PL_MARKER_COLON);
        if (!arg_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto cleanup_name_node;
        }
        arg_node->token.line_no = line_no;
        createFamily(arg_node, name_node, type_node);
        type_node = NULL;

        if (arg_list) {
            plAstNode *comma_node;

            comma_node = plAstNew(PL_MARKER_COMMA);
            if (!comma_node) {
                plAstFree(arg_node, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            createFamily(comma_node, arg_list, arg_node);
            arg_list = comma_node;
        }
        else {
            arg_list = arg_node;
        }

        continue;

cleanup_name_node:

        plAstFree(name_node, scanner->table);
        goto error;
    }

    if (function_marker != PL_MARKER_SINK) {
        ret = expectMarker(scanner, PL_MARKER_ARROW, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = plAstNew(function_marker);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        goto error;
    }
    (*node)->token.line_no = line_no;

    switch (function_marker) {
    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE: createFamily(*node, function_name_node, arg_list, type_node, statement_list); break;

    default:
        createFamily(*node, function_name_node, arg_list, statement_list);
        break;  // SINK

        return PL_RET_OK;

error:

        plAstFree(function_name_node, scanner->table);
        plAstFree(arg_list, scanner->table);
        plAstFree(type_node, scanner->table);

        return ret;
    }

    int parseInlineFunction(plLexicalScanner * scanner, plAstNode * *node)
    {
        (void)scanner;

        *node = NULL;
        VASQ_ERROR("This function has not yet been implemented.");
        return PL_RET_BAD_DATA;
    }
