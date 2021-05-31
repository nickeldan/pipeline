#include <string.h>

#include "parserInternal.h"

int
parseFunction(plLexicalScanner *scanner, plAstNode **node, bool global)
{
    int ret, function_marker;
    bool declaration;
    plLexicalLocation function_location, comma_location = {0};
    plLexicalToken token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list = NULL;
    plAstMaxSplitNode *splitter;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR(debug_logger, "scanner and node cannot be NULL.");
        return PL_RET_USAGE;
    }

    function_marker = scanner->last_marker;
    plGetLastLocation(scanner, &function_location);

    if (function_marker == PL_MARKER_LOCAL && global) {
        VASQ_ERROR(debug_logger, "LOCAL not allowed in global scope.");
        return PL_RET_USAGE;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (global) {
        if (token.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Anonymous %s not allowed in global scope.", plLexicalMarkerName(function_marker));
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
    }

    if (token.marker != PL_MARKER_LEFT_PARENS) {
        PARSER_ERROR("Unexpected %s before argument list.", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    while (true) {
        int previous_marker = scanner->last_marker;
        plLexicalLocation location;
        plAstNode *name_node, *arg_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_PARENS) {
            if (previous_marker == PL_MARKER_COMMA) {
                PARSER_ERROR("Unexpected ')' following ',' in argument list.");
                ret = PL_RET_BAD_DATA;
                goto error;
            }
            break;
        }

        if (token.marker == PL_MARKER_COMMA) {
            if (previous_marker == PL_MARKER_LEFT_PARENS || previous_marker == PL_MARKER_COMMA) {
                PARSER_ERROR("Unexpected ',' following %s in argument list.",
                             plLexicalMarkerName(previous_marker));
                ret = PL_RET_BAD_DATA;
                goto error;
            }

            if (function_marker != PL_MARKER_SOURCE) {
                PARSER_ERROR("Multiple arguments are only allowed in a SOURCE.");
                ret = PL_RET_BAD_DATA;
                goto error;
            }

            memcpy(&comma_location, &token.location, sizeof(token.location));

            continue;
        }

        if (token.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Unexpected %s following %s in argunent list.", plLexicalMarkerName(token.marker),
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

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &location);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }

        if (type_node->token.marker != PL_MARKER_TYPE) {
            ret = NEXT_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto cleanup_name_node;
            }

            if (token.marker == PL_MARKER_QUESTION) {
                plAstNode *temp_node;

                temp_node = plAstCreateFamily(PL_MARKER_QUESTION, type_node);
                if (!temp_node) {
                    ret = PL_RET_OUT_OF_MEMORY;
                    goto cleanup_name_node;
                }
                memcpy(&temp_node->token, &token, sizeof(token));
                type_node = temp_node;
            }
            else {
                ret = LOOKAHEAD_STORE(scanner, &token);
                if (ret != PL_RET_OK) {
                    goto cleanup_name_node;
                }
            }
        }

        arg_node = plAstCreateFamily(PL_MARKER_COLON, name_node, type_node);
        if (!arg_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto cleanup_name_node;
        }
        memcpy(&arg_node->token.location, &location, sizeof(location));
        type_node = NULL;

        if (arg_list) {
            ret = plAstCreateConnection(PL_MARKER_COMMA, &arg_list, arg_node);
            if (ret != PL_RET_OK) {
                plAstFree(arg_node, scanner->table);
                goto error;
            }
            memcpy(&arg_list->token.location, &comma_location, sizeof(comma_location));
        }
        else {
            arg_list = arg_node;
        }

        continue;

cleanup_name_node:

        plAstFree(name_node, scanner->table);
        goto error;
    }

    if (function_marker != PL_MARKER_SINK && function_marker != PL_MARKER_LOCAL) {
        ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }
    if (token.marker == PL_MARKER_SEMICOLON) {
        if (!global) {
            PARSER_ERROR("Function declaration without definition only allowed in global scope.");
            ret = PL_RET_BAD_DATA;
            goto error;
        }
        declaration = true;
        statement_list = NULL;
        goto skip_statement_list;
    }
    else if (token.marker != PL_MARKER_LEFT_BRACE) {
        PARSER_ERROR("Unexpected %s after function header.", plLexicalMarkerName(token.marker));
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

skip_statement_list:

    *node = plAstNew(function_marker);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        goto error;
    }
    memcpy(&(*node)->token.location, &function_location, sizeof(function_location));
    if (declaration) {
        (*node)->token.submarker = PL_SUBMARKER_FUNC_DECL;
    }

    splitter = (plAstMaxSplitNode *)(*node);
    if (function_marker == PL_MARKER_LOCAL) {
        splitter->nodes[0] = arg_list;
        splitter->nodes[1] = statement_list;
    }
    else {
        splitter->nodes[0] = function_name_node;
        splitter->nodes[1] = arg_list;
        if (function_marker == PL_MARKER_SINK) {
            splitter->nodes[2] = statement_list;
        }
        else {
            splitter->nodes[2] = type_node;
            splitter->nodes[3] = statement_list;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(function_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);
    plAstFree(type_node, scanner->table);

    return ret;
}
