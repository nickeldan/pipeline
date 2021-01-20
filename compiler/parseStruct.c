#include <string.h>

#include "parserInternal.h"

int
parseStructDefinition(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *struct_name_node, *arg_list = NULL;

    if (node) {
        *node = NULL;
    }
    if (!scanner || !node) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Expected NAME instead of %s following STRUCT.", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }
    struct_name_node = plAstNew(PL_MARKER_NAME);
    if (!struct_name_node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&struct_name_node->token, &token, sizeof(token));

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    while (true) {
        plLexicalToken token;
        plLexicalLocation colon_location;
        plAstNode *name_node, *type_node, *arg_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_BRACE) {
            break;
        }

        if (token.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Expected NAME at beginning of struct field definition.");
            ret = PL_RET_BAD_DATA;
            goto loop_error;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &colon_location);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        if (!name_node) {
            plAstFree(type_node, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto loop_error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        arg_node = createFamily(PL_MARKER_COLON, name_node, type_node);
        if (!arg_node) {
            plAstFree(name_node, scanner->table);
            plAstFree(type_node, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&arg_node->token.location, &colon_location, sizeof(colon_location));

        if (arg_list) {
            ret = createConnection(PL_MARKER_SEMICOLON, &arg_list, arg_node);
            if (ret != PL_RET_OK) {
                plAstFree(arg_node, scanner->table);
                goto error;
            }
        }
        else {
            arg_list = arg_node;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        continue;

loop_error:

        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    if (!arg_list) {
        PARSER_ERROR("STRUCT definition cannot have zero fields.");
        return PL_RET_BAD_DATA;
    }

    *node = createFamily(PL_MARKER_STRUCT, struct_name_node, arg_list);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

    return PL_RET_OK;

error:

    plAstFree(struct_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);

    return ret;
}
