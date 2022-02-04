#include <string.h>

#include "parserInternal.h"

static int
parseArgList(plLexicalScanner *scanner, int function_marker, plAstNode **arg_list)
{
    int ret;
    plAstNode *name_node = NULL;

    while (true) {
        int previous_marker = scanner->last_marker;
        plLexicalToken token;
        plLexicalLocation location, comma_location;
        plAstNode *arg_node, *type_node;

        ret = NEXT_TOKEN(scanner, &token);
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

            memcpy(&comma_location, &token.header.location, sizeof(token.header.location));

            continue;
        }

        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Unexpected %s following %s in argument list.",
                         plLexicalMarkerName(token.header.marker), plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        plAstCopyTokenInfo(name_node, &token);

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &location);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        if (type_node->header.marker != PL_MARKER_TYPE) {
            ret = NEXT_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto cleanup_name_node;
            }

            if (token.header.marker == PL_MARKER_QUESTION) {
                plAstNode *temp_node;

                temp_node = plAstCreateFamily(PL_MARKER_QUESTION, type_node);
                plAstCopyTokenInfo(temp_node, &token);
                type_node = temp_node;
            }
            else {
                LOOKAHEAD_STORE(scanner, &token);
            }
        }

        arg_node = plAstCreateFamily(PL_MARKER_COLON, name_node, type_node);
        memcpy(&arg_node->header.location, &location, sizeof(location));

        if (*arg_list) {
            ret = plAstCreateConnection(PL_MARKER_COMMA, arg_list, arg_node);
            if (ret != PL_RET_OK) {
                plAstFree(arg_node, scanner->table);
                return ret;
            }
            memcpy(&(*arg_list)->header.location, &comma_location, sizeof(comma_location));
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
    int ret, function_marker;
    bool declaration;
    plLexicalLocation function_location;
    plLexicalToken token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "scanner and node cannot be NULL.");
        return PL_RET_USAGE;
    }

    function_marker = scanner->last_marker;
    plGetLastLocation(scanner, &function_location);

    if (function_marker == PL_MARKER_LOCAL && global) {
        VASQ_ERROR(debug_logger, "LOCAL not allowed in global scope.");
        return PL_RET_BAD_DATA;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (global) {
        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Anonymous %s not allowed in global scope.", plLexicalMarkerName(function_marker));
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        function_name_node = plAstNew(PL_MARKER_NAME);
        plAstCopyTokenInfo(function_name_node, &token);

        ret = NEXT_TOKEN(scanner, &token);
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

    ret = parseArgList(scanner, function_marker, &arg_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    if (function_marker != PL_MARKER_SINK && function_marker != PL_MARKER_LOCAL) {
        ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    ret = NEXT_TOKEN(scanner, &token);
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

    *node = plAstNew(function_marker);
    memcpy(&(*node)->header.location, &function_location, sizeof(function_location));
    if (declaration) {
        (*node)->header.submarker = PL_SUBMARKER_FUNC_DECL;
    }

    if (function_marker == PL_MARKER_LOCAL) {
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

        if (function_marker == PL_MARKER_SINK) {
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
