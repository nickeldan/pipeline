#include "parserInternal.h"

int
plParseStructDefinition(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken lead_token, token;
    plAstNode *struct_name_node, *arg_list = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = CONSUME_TOKEN(scanner, &lead_token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_NAME, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }
    struct_name_node = plAstNew(PL_MARKER_NAME, &token);

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    while (PEEK_TOKEN(scanner, 0) != PL_MARKER_RIGHT_BRACE) {
        plLexicalToken colon_token;
        plAstNode *name_node, *type_node, *arg_node;

        ret = CONSUME_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }
        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Expected NAME at beginning of struct field definition.");
            ret = PL_RET_BAD_DATA;
            goto loop_error;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &colon_token);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        name_node = plAstNew(PL_MARKER_NAME, &token);
        arg_node = plAstCreateFamily(PL_MARKER_COLON, &colon_token, name_node, type_node);

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            plAstFree(arg_node, scanner->table);
            goto error;
        }

        if (arg_list) {
            plAstCreateConnection(PL_MARKER_SEMICOLON, NULL, &arg_list, arg_node);
        }
        else {
            arg_list = arg_node;
        }
        continue;

loop_error:

        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    if (!arg_list) {
        PARSER_ERROR("STRUCT definition cannot have zero fields.");
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_STRUCT, &lead_token, struct_name_node, arg_list);

    return PL_RET_OK;

error:

    plAstFree(struct_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);

    return ret;
}
