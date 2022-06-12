#include <string.h>

#include "parserInternal.h"

static int
parseArgList(plLexicalScanner *scanner, int function_marker, plAstNode **arg_list)
{
    int ret, previous_marker = function_marker;
    plLexicalToken comma_token;
    plAstNode *name_node = NULL;

    while (true) {
        plLexicalToken token, colon_token;
        plAstNode *arg_node, *type_node;

        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            return ret;
        }

        if (token.header.marker == PL_MARKER_RIGHT_PARENS) {
            if (previous_marker == PL_MARKER_COMMA) {
                PARSER_ERROR("Unexpected ')' following ',' in argument list.");
                return PL_RET_BAD_DATA;
            }
            return PL_RET_OK;
        }

        if (token.header.marker == PL_MARKER_COMMA) {
            if (previous_marker == PL_MARKER_LEFT_PARENS || previous_marker == PL_MARKER_COMMA) {
                PARSER_ERROR("Unexpected ',' following %s in argument list.",
                             plLexicalMarkerName(previous_marker));
                return PL_RET_BAD_DATA;
            }

            if (function_marker != PL_MARKER_SOURCE) {
                PARSER_ERROR("Multiple arguments are only allowed in a SOURCE.");
                return PL_RET_BAD_DATA;
            }

            memcpy(&comma_token, &token, sizeof(token));

            continue;
        }

        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Unexpected %s following %s in argument list.",
                         plLexicalMarkerName(token.header.marker), plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        name_node = plAstNew(PL_MARKER_NAME, &token);

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &colon_token);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        if (type_node->header.marker != PL_MARKER_TYPE && PEEK_TOKEN(scanner, 0) == PL_MARKER_QUESTION) {
            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto cleanup_name_node;
            }
            type_node = plAstCreateFamily(PL_MARKER_QUESTION, &token, type_node);
        }

        arg_node = plAstCreateFamily(PL_MARKER_COLON, &colon_token, name_node, type_node);

        if (*arg_list) {
            ret = plAstCreateConnection(PL_MARKER_COMMA, &comma_token, arg_list, arg_node);
            if (ret != PL_RET_OK) {
                plAstFree(arg_node, scanner->table);
                return ret;
            }
        }
        else {
            *arg_list = arg_node;
        }
    }

cleanup_name_node:

    plAstFree(name_node, scanner->table);
    return ret;
}

int
plParseFunction(plLexicalScanner *scanner, plAstNode **node, bool global)
{
    int ret;
    bool declaration;
    plLexicalToken lead_token, token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "scanner and node cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = CONSUME_TOKEN(scanner, &lead_token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (lead_token.header.marker == PL_MARKER_LOCAL && global) {
        VASQ_ERROR(debug_logger, "LOCAL not allowed in global scope.");
        return PL_RET_BAD_DATA;
    }

    ret = CONSUME_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (global) {
        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Anonymous %s not allowed in global scope.",
                         plLexicalMarkerName(lead_token.header.marker));
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        function_name_node = plAstNew(PL_MARKER_NAME, &token);

        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    if (token.header.marker != PL_MARKER_LEFT_PARENS) {
        PARSER_ERROR("Unexpected %s before argument list.", plLexicalMarkerName(token.header.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    ret = parseArgList(scanner, lead_token.header.marker, &arg_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    if (lead_token.header.marker != PL_MARKER_SINK && lead_token.header.marker != PL_MARKER_LOCAL) {
        ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    ret = CONSUME_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }
    if (token.header.marker == PL_MARKER_SEMICOLON) {
        if (!global) {
            PARSER_ERROR("Function declaration without definition only allowed in global scope.");
            ret = PL_RET_BAD_DATA;
            goto error;
        }
        declaration = true;
        statement_list = NULL;
        goto skip_statement_list;
    }
    else if (token.header.marker != PL_MARKER_LEFT_BRACE) {
        PARSER_ERROR("Unexpected %s after function header.", plLexicalMarkerName(token.header.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    ret = plParseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

skip_statement_list:

    *node = plAstNew(lead_token.header.marker, &lead_token);
    if (declaration) {
        (*node)->header.submarker = PL_SUBMARKER_FUNC_DECL;
    }

    if (lead_token.header.marker == PL_MARKER_LOCAL) {
        if (!plAstSetChild(*node, 0, arg_list) || !plAstSetChild(*node, 1, statement_list)) {
            ret = PL_RET_USAGE;
            goto error;
        }
    }
    else {
        if (!plAstSetChild(*node, 0, function_name_node) || !plAstSetChild(*node, 1, arg_list)) {
            ret = PL_RET_USAGE;
            goto error;
        }

        if (lead_token.header.marker == PL_MARKER_SINK) {
            if (!plAstSetChild(*node, 2, statement_list)) {
                ret = PL_RET_USAGE;
                goto error;
            }
        }
        else {
            if (!plAstSetChild(*node, 2, type_node) || !plAstSetChild(*node, 3, statement_list)) {
                ret = PL_RET_USAGE;
                goto error;
            }
        }
    }

    return PL_RET_OK;

error:

    plAstFree(function_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);
    plAstFree(type_node, scanner->table);

    return ret;
}
